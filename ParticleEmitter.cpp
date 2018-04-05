#include "ParticleEmitter.h"
#include <iostream>

bool CheckCompileStatus(GLuint shaderId)
{
	GLint result = GL_FALSE;
	int logLength = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE) {
		char* logBuffer = NULL;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &logLength);
		logBuffer = new char[logLength];
		glGetShaderInfoLog(shaderId, logLength, NULL, logBuffer);
		std::cerr << "Compile Error: " << logBuffer << std::endl;
		delete[] logBuffer;
		return false;
	}
	return true;

}

bool CheckLinkStatus(GLuint programId)
{
	GLint result = GL_FALSE;
	int logLength = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	if (result != GL_TRUE) {
		char* logBuffer = NULL;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &logLength);
		logBuffer = new char[logLength];
		glGetProgramInfoLog(programId, logLength, NULL, logBuffer);
		std::cerr << "Link Error: " << logBuffer << std::endl;
		delete[] logBuffer;
		return false;
	}
	return true;
}


ParticleEmitter::ParticleEmitter() : m_particles(nullptr),
									 m_firstDead(0),
									 m_maxParticles(0),
									 m_position(0, 0, 0),
									 m_VAO(0), m_VBO(0), m_IBO(0),
									 m_vertexData(nullptr)
{
}



ParticleEmitter::~ParticleEmitter()
{
	delete[] m_particles;
	delete[] m_vertexData;
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_IBO);
}

bool ParticleEmitter::Init(unsigned int a_maxParticles,
						   unsigned int a_emitRate,
						   float a_lifetimeMin, float a_lifetimeMax,
						   float a_velocityMin, float a_velocityMax,
						   float a_startSize, float a_endSize,
						   const glm::vec4& a_startColour, const glm::vec4& a_endColour)
{

	// set up emit timers
	m_emitTimer = 0;
	m_emitRate = 1.0f / a_emitRate;

	// store all variables passed in
	m_startColour = a_startColour;
	m_endColour = a_endColour;
	m_startSize = a_startSize;
	m_endSize = a_endSize;
	m_velocityMin = a_velocityMin;
	m_velocityMax = a_velocityMax;
	m_lifespanMin = a_lifetimeMin;
	m_lifespanMax = a_lifetimeMax;
	m_maxParticles = a_maxParticles;

	const char* vsSource = "#version 410\n \
							in vec4 Position; \
							in vec4 Colour; \
							out vec4 colour; \
							uniform mat4 projectionView; \
							void main() { \
							colour = Colour; \
							gl_Position = projectionView * Position;}";

	const char* fsSource = "#version 410\n \
							in vec4 colour; \
							out vec4 FragColour; \
							void main() { \
							FragColour = colour;}";

	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vs, 1, &vsSource, nullptr);
	glShaderSource(fs, 1, &fsSource, nullptr);
	glCompileShader(fs);
	glCompileShader(vs);

	CheckCompileStatus(fs);
	CheckCompileStatus(vs);

	m_programID = glCreateProgram();
	glAttachShader(m_programID, vs);
	glAttachShader(m_programID, fs);
	glLinkProgram(m_programID);

	CheckLinkStatus(m_programID);

	glDeleteShader(fs);
	glDeleteShader(vs);

	// create particle array
	m_particles = new Particles[m_maxParticles];
	m_firstDead = 0;

	// create the array of vertices for the particles
	// 4 vertices per particle for a quad.
	// will be filled during update
	m_vertexData = new ParticleVertex[m_maxParticles * 4];

	// create the index buffeer data for the particles
	// 6 indices per quad of 2 triangles
	// fill it now as it never changes
	unsigned int* indexData = new unsigned int[m_maxParticles * 6];
	for (unsigned int i = 0; i < m_maxParticles; ++i)
	{
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;
		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	// create opengl buffers
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * 4 *
				 sizeof(ParticleVertex), m_vertexData,
				 GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_maxParticles * 6 *
				 sizeof(unsigned int), indexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // colour
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE,
						  sizeof(ParticleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
						  sizeof(ParticleVertex), ((char*)0) + 16);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	delete[] indexData;

	return false;
}

void ParticleEmitter::Emit()
{
	// only emit if there is a dead particle to use
	if (m_firstDead >= m_maxParticles)
		return;

	// resurrect the first dead particle
	Particles& particle = m_particles[m_firstDead++];

	// assign its starting position
	particle.position = m_position;

	// randomise its lifespan
	particle.lifetime = 0;
	particle.lifespan = (rand() / (float)RAND_MAX) *
		(m_lifespanMax - m_lifespanMin) + m_lifespanMin;

	// set starting size and colour
	particle.colour = m_startColour;
	particle.size = m_startSize;

	// randomise velocity direction and strength
	float velocity = (rand() / (float)RAND_MAX) * (m_velocityMax - m_velocityMin) + m_velocityMin;
	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity = glm::normalize(particle.velocity) * velocity;

}

bool ParticleEmitter::Update(float deltaTime, const glm::mat4& a_cameraTransform)
{
	using glm::vec3;
	using glm::vec4;
	// spawn particles
	m_emitTimer += deltaTime;
	while (m_emitTimer > m_emitRate)
	{
		Emit();
		m_emitTimer -= m_emitRate;
	}

	unsigned int quad = 0;
	// update particles and turn live particles into billboard quads
	for (unsigned int i = 0; i < m_firstDead; i++) {
		Particles* particle = &m_particles[i];

		particle->lifetime += deltaTime;
		if (particle->lifetime >= particle->lifespan)
		{
			// swap last alive with this one
			*particle = m_particles[m_firstDead - 1];
			m_firstDead--;
		}
		else
		{
			// move particle
			particle->position += particle->velocity * deltaTime;
			// size particle
			particle->size = glm::mix(m_startSize, m_endSize,
				particle->lifetime / particle->lifespan);
			// colour particle
			particle->colour = glm::mix(m_startColour, m_endColour,
				particle->lifetime / particle->lifespan);
			// make a quad the correct size and colour
			float halfSize = particle->size * 0.5f;
			m_vertexData[quad * 4 + 0].position = vec4(halfSize,
													   halfSize, 0, 1);
			m_vertexData[quad * 4 + 0].colour = particle->colour;
			m_vertexData[quad * 4 + 1].position = vec4(-halfSize,
													   halfSize, 0, 1);
			m_vertexData[quad * 4 + 1].colour = particle->colour;
			m_vertexData[quad * 4 + 2].position = vec4(-halfSize,
													   -halfSize, 0, 1);
			m_vertexData[quad * 4 + 2].colour = particle->colour;
			m_vertexData[quad * 4 + 3].position = vec4(halfSize,
													   -halfSize, 0, 1);
			m_vertexData[quad * 4 + 3].colour = particle->colour;

			// create billboard transform
			vec3 zAxis = glm::normalize(vec3(a_cameraTransform[3]) -
											 particle->position);
			vec3 xAxis = glm::cross(vec3(a_cameraTransform[1]), zAxis);
			vec3 yAxis = glm::cross(zAxis, xAxis);
			glm::mat4 billboard(vec4(xAxis, 0),
								vec4(yAxis, 0),
								vec4(zAxis, 0),
								vec4(0, 0, 0, 1));

			m_vertexData[quad * 4 + 0].position = billboard *
						m_vertexData[quad * 4 + 0].position +
						vec4(particle->position, 0);
			m_vertexData[quad * 4 + 1].position = billboard *
						m_vertexData[quad * 4 + 1].position +
						vec4(particle->position, 0);
			m_vertexData[quad * 4 + 2].position = billboard *
						m_vertexData[quad * 4 + 2].position +
						vec4(particle->position, 0);
			m_vertexData[quad * 4 + 3].position = billboard *
						m_vertexData[quad * 4 + 3].position +
						vec4(particle->position, 0);
			++quad;
		}
	}


	return false;
}

void ParticleEmitter::Draw(const mat4& m_projectionViewMatrix)
{
	glUseProgram(m_programID);
	
	unsigned int projectionViewUniform =
		glGetUniformLocation(m_programID, "projectionView");
	glUniformMatrix4fv(projectionViewUniform, 1, false,
		glm::value_ptr(m_projectionViewMatrix));

	// sync the particle vertex buffer
	// based on how many alive particles there are
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_firstDead * 4 *
		sizeof(ParticleVertex), m_vertexData);

	// draw particles
	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_firstDead * 6, GL_UNSIGNED_INT, 0);
}

void ParticleEmitter::Destroy()
{
	delete m_particles;
	glDeleteProgram(m_programID);
}
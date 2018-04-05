#pragma once

#define GLM_SWIZZLE

#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>

#include <Gizmos.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;

struct Particles
{
	vec3 position;
	vec3 velocity;
	vec4 colour;

	float size;
	float lifetime;
	float lifespan;
};

struct ParticleVertex
{
	glm::vec4 position;
	glm::vec4 colour;
};

class ParticleEmitter
{
public:
	ParticleEmitter();
	virtual ~ParticleEmitter();

	bool Init(unsigned int a_maxParticles,
			  unsigned int a_emitRate,
			  float a_lifetimeMin, float a_lifetimeMax,
			  float a_velocityMin, float a_velocityMax,
			  float a_startSize, float a_endSize,
			  const glm::vec4& a_startColour, const glm::vec4& a_endColour);
	
	void Emit();
	
	bool Update(float deltaTime, const glm::mat4& a_cameraTransform);
	void Draw(const mat4& m_projectionViewMatrix);
	void Destroy();

	unsigned int m_programID;

protected:
	Particles* m_particles;
	unsigned int m_firstDead;
	unsigned int m_maxParticles;

	unsigned int m_VAO;	//vertex array object
	unsigned int m_VBO; //vertex buffer object
	unsigned int m_IBO;	//index buffer object
	
	ParticleVertex* m_vertexData;
	
	glm::vec3 m_position;

	float m_emitTimer;
	float m_emitRate;

	float m_lifespanMin;
	float m_lifespanMax;

	float m_velocityMin;
	float m_velocityMax;

	float m_startSize;
	float m_endSize;

	glm::vec4 m_startColour;
	glm::vec4 m_endColour;


};
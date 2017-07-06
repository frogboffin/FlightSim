#pragma once
#include "OpenGL\OGLTexture.h"
#include "OpenGL\OGLMesh.h"
#include <GLM\glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdlib.h>
#include <time.h>

class Target : public OGLMesh
{
private: 
	Renderable *m_mesh;
	OGLTexture *m_texture;
	glm::vec3 m_pos;
	float m_size;
	float m_tilt;
	float worldSpinLocal;

	glm::mat4 rotateMat;
	glm::mat4 translationMat;
	glm::mat4 scaleMat;
	glm::mat4 model;
public:
	Target();
	Target(Renderable* mesh, OGLTexture* texture);
	~Target();

	void Render();
	bool HitCheck(glm::vec3 start, glm::vec3 ray);

	inline const glm::mat4* GetModelMat() const
	{
		return &model;
	}
};


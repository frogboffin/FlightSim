#include "Target.h"
#include "GLEW/include/glew.h"
#include <GLM\gtx\rotate_vector.hpp>
#include <GLM\gtx\norm.hpp>
#include <iostream>

Target::Target()
{

}

Target::Target(Renderable* mesh, OGLTexture* texture)
{
	m_mesh = mesh;
	m_texture = texture;
	m_size = rand() % 50 + 100;
	m_pos = glm::vec3((rand() % 10000 + 1000) - 5000, (rand() % 2000 + 500) / 10, (rand() % 10000 + 1000) - 5000);
	m_tilt = (float)(rand() % 100);
	m_mesh->SetTexture(m_texture);
}


Target::~Target()
{

}

void Target::Render()
{
	Renderable* prenderable = static_cast<Renderable*>(m_mesh);

	rotateMat = glm::rotate(glm::mat4(1.0f), m_tilt, glm::vec3(1, 1, 1));
	rotateMat *= glm::rotate(glm::mat4(1.0f), worldSpinLocal += 0.0003f, glm::vec3(0.0f, 1.0f, 0.0f));
	translationMat = glm::translate(glm::mat4(1.0f), m_pos);
	scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(m_size, m_size, m_size));
	model = translationMat * scaleMat * rotateMat;

	m_mesh->Render();
}

bool Target::HitCheck(glm::vec3 start, glm::vec3 ray)
{
	glm::vec3 s = start - m_pos;
	double a = glm::length2(ray);
	double b = 2.0 * glm::dot(ray, s);
	double c = glm::length2(s) - (m_size * m_size);
	double radicand = b*b - 4.0f*a*c;

	if (radicand > 0.00001)
	{
		m_pos = glm::vec3((rand() % 10000 + 1000) - 5000, (rand() % 2000 + 500) / 10, (rand() % 10000 + 1000) - 5000);
		return true;
	}
	else
		return false;

}

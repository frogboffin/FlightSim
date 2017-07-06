#include "SkyBox.h"
#include <windows.h>
#include "GLEW/include/glew.h"

SkyBox::SkyBox()
{
}


SkyBox::~SkyBox()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo_verts);
	glDeleteBuffers(1, &m_vbo_colours);
	glDeleteBuffers(1, &m_vbo_indices);
}

void SkyBox::InitSkyBox()
{
	//Corners
	//SimpleVertex corners[] =
	//{
	//	{ -size, -size, size, 0.0f, 0.0f, 0.0f, 0.75f, 0.33f },// 0
	//	{ size, -size, size, 1.0f, 0.0f, 0.0f, 1.0f, 0.33f }, // 1
	//	{ size, size, size, 1.0f, 1.0f, 0.0f, 1.0f, 0.66f }, // 2
	//	{ -size, size, size, 0.0f, 1.0f, 0.0f, 0.750f, 0.66f }, // 3

	//	{ size, -size, size, 1.0f, 0.0f, 0.0f, 0.0f, 0.33f }, // 4
	//	{ size, -size, -size, 1.0f, 0.0f, 1.0f, 0.25f, 0.33f }, // 5
	//	{ size, size, -size, 1.0f, 1.0f, 1.0f, 0.25f, 0.66f }, // 6
	//	{ size, size, size, 1.0f, 1.0f, 0.0f, 0.0f, 0.66f }, // 7

	//	{ size, -size, -size, 1.0f, 0.0f, 1.0f, 0.25f, 0.33f }, // 8
	//	{ -size, -size, -size, 0.0f, 0.0f, 1.0f, 0.5f, 0.33f }, //9
	//	{ -size, size, -size, 0.0f, 1.0f, 1.0f, 0.5f, 0.66f }, // 10
	//	{ size, size, -size, 1.0f, 1.0f, 1.0f, 0.25f, 0.66f }, // 11

	//	{ -size, -size, -size, 0.0f, 0.0f, 1.0f, 0.5f, 0.33f }, //12
	//	{ -size, -size, size, 0.0f, 0.0f, 0.0f, 0.75f, 0.33f },// 13
	//	{ -size, size, size, 0.0f, 1.0f, 0.0f, 0.75f, 0.66f }, // 14
	//	{ -size, size, -size, 0.0f, 1.0f, 1.0f, 0.5f, 0.66f }, // 15

	//	{ -size, size, size, 0.0f, 1.0f, 0.0f, 0.495f, 1.0f }, // 16
	//	{ size, size, size, 1.0f, 1.0f, 0.0f, 0.259f, 1.0f }, // 17
	//	{ size, size, -size, 1.0f, 1.0f, 1.0f, 0.259f, 0.66f }, // 18
	//	{ -size, size, -size, 0.0f, 1.0f, 1.0f, 0.495f, 0.66f }, // 19

	//	{ -size*1.1f, -size*0.9f, -size*1.1f, 0.0f, 0.0f, 1.0f, 0.26f, 0.1f }, //20
	//	{ size*1.1f, -size*0.9f, -size*1.1f, 1.0f, 0.0f, 1.0f, 0.49f, 0.1f }, // 21
	//	{ size*1.1f, -size*0.9f, size*1.1f, 1.0f, 0.0f, 0.0f, 0.49f, 0.32f }, // 22
	//	{ -size*1.1f, -size*0.9f, size*1.1f, 0.0f, 0.0f, 0.0f, 0.26f, 0.32f }// 23
	//};
	SimpleVertex corners[] =
	{
		{ -size, -size, size, 0.0f, 0.0f, 0.0f, 0.75f, 0.33f },// 0
		{ size, -size, size, 1.0f, 0.0f, 0.0f, 1.0f, 0.33f }, // 1
		{ size, size, size, 1.0f, 1.0f, 0.0f, 1.0f, 0.66f }, // 2
		{ -size, size, size, 0.0f, 1.0f, 0.0f, 0.750f, 0.66f }, // 3

		{ size, -size, size, 1.0f, 0.0f, 0.0f, 0.0f, 0.33f }, // 4
		{ size, -size, -size, 1.0f, 0.0f, 1.0f, 0.25f, 0.33f }, // 5
		{ size, size, -size, 1.0f, 1.0f, 1.0f, 0.25f, 0.66f }, // 6
		{ size, size, size, 1.0f, 1.0f, 0.0f, 0.0f, 0.66f }, // 7

		{ size, -size, -size, 1.0f, 0.0f, 1.0f, 0.25f, 0.33f }, // 8
		{ -size, -size, -size, 0.0f, 0.0f, 1.0f, 0.5f, 0.33f }, //9
		{ -size, size, -size, 0.0f, 1.0f, 1.0f, 0.5f, 0.66f }, // 10
		{ size, size, -size, 1.0f, 1.0f, 1.0f, 0.25f, 0.66f }, // 11

		{ -size, -size, -size, 0.0f, 0.0f, 1.0f, 0.5f, 0.33f }, //12
		{ -size, -size, size, 0.0f, 0.0f, 0.0f, 0.75f, 0.33f },// 13
		{ -size, size, size, 0.0f, 1.0f, 0.0f, 0.75f, 0.66f }, // 14
		{ -size, size, -size, 0.0f, 1.0f, 1.0f, 0.5f, 0.66f }, // 15

		{ -size, size, size, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f }, // 16
		{ size, size, size, 1.0f, 1.0f, 0.0f, 0.25f, 1.0f }, // 17
		{ size, size, -size, 1.0f, 1.0f, 1.0f, 0.25f, 0.66f }, // 18
		{ -size, size, -size, 0.0f, 1.0f, 1.0f, 0.5f, 0.66f }, // 19

		{ -size, -size, -size, 0.0f, 0.0f, 1.0f, 0.25f, 0.1f }, //20
		{ size, -size, -size, 1.0f, 0.0f, 1.0f, 0.5f, 0.1f }, // 21
		{ size, -size, size, 1.0f, 0.0f, 0.0f, 0.5f, 0.33f }, // 22
		{ -size, -size, size, 0.0f, 0.0f, 0.0f, 0.25f, 0.33f }// 23
	};
	unsigned int indices[] =
	{
		3, 1, 0,
		3, 2, 1,

		7, 5, 4,
		7, 6, 5,

		11, 9, 8,
		11, 10, 9,

		15, 14, 12,
		14, 13, 12,

		19, 18, 16,
		18, 17, 16,

		23, 22, 20,
		22, 21, 20
	};

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo_verts);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo_verts);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(SimpleVertex), corners, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)(NULL + 12));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SimpleVertex), (void*)(NULL + 24));
	glEnableVertexAttribArray(2);


	glGenBuffers(1, &m_vbo_indices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vbo_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(int), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void SkyBox::Render()
{
	unsigned int texHandle = dynamic_cast<OGLTexture*>(m_tex)->m_syshandle;

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texHandle);

	glBindVertexArray(m_vao);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

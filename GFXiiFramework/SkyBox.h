#include "OpenGL\OGLTexture.h"
#include "OpenGL\OGLCube.h"
#include <GLM\glm.hpp>

class SkyBox : public OGLCube
{
private:
	OGLTexture *mtextures[6];
	float size = 1000.0f;

public:
	SkyBox();
	~SkyBox();

	void InitSkyBox();
	virtual void Render();
};


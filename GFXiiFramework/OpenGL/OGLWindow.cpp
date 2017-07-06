#include "OGLWindow.h"
#include "Resource.h"
#include "GLEW/include/glew.h"
#include <GLM\glm.hpp>
#include <GLM\gtx\rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Target.h>
#include <iostream>

OGLWindow::OGLWindow()
{

}

OGLWindow::~OGLWindow()
{
	//Clean up the renderable
	delete ship;
	delete skybox;
	delete ground;
	delete m_shader;
	DestroyOGLContext();
}

OGLWindow::OGLWindow(HINSTANCE hInstance, int width, int height)
{
	//InitWindow(hInstance, width, height);
}

HGLRC OGLWindow::CreateOGLContext(HDC hdc)
{
	unsigned int pixelformat;
	HGLRC hglrc;

	static PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Support Window
		PFD_SUPPORT_OPENGL |						// Format Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request RGBA Format
		32,											// Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits mask
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// Z-Buffer depth
		0,											// Stencil Buffer
		0,											// Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(pixelformat=ChoosePixelFormat(hdc,&pfd)))
	{
		return 0;
	}

	if(!SetPixelFormat(hdc,pixelformat,&pfd))
	{
		return 0;
	}

	if (!(hglrc=wglCreateContext(hdc)))
	{
		return 0;
	}

	if(!wglMakeCurrent(hdc,hglrc))
	{
		return 0;
	}

	return hglrc;
}

void OGLWindow::DestroyRenderWindow()
{
	DestroyWindow( m_hwnd );

	m_hwnd = NULL;
	m_hdc = NULL;
}

BOOL OGLWindow::DestroyOGLContext()
{

	glDeleteSamplers( 1, (GLuint*)(&m_texDefaultSampler) );

	if ( m_hglrc )
	{
		wglMakeCurrent( NULL, NULL );
		wglDeleteContext( m_hglrc );
		m_hglrc = NULL;
	}
	
	if ( !ReleaseDC ( m_hwnd, m_hdc ) )
		return FALSE;

	return TRUE;
}

BOOL OGLWindow::InitWindow(HINSTANCE hInstance, int width, int height)
{
	m_hwnd = CreateWindowEx( WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
		L"RenderWindow", L"OGLWindow", WS_OVERLAPPEDWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	if ( ! m_hwnd )
		return FALSE;

	m_hdc = GetDC( m_hwnd );

	if ( !(m_hglrc = CreateOGLContext( m_hdc )) )
		return FALSE;

	glewInit();

	InitOGLState();

	m_width = width;
	m_height = height;

	skybox = new SkyBox();
	skybox->InitSkyBox();
	skyTexture = new OGLTexture();
	skyTexture->CreateTextureFromFile("../asset/texture/sky/star.tga");
	skybox->SetTexture(skyTexture);

	m_camera.InitCamera(1);
	m_camera.SetCameraPosition(new glm::vec3(0.0f, 0.0f, 0.0f));
	m_camera.SetCameraFOV(36.0f);
	m_camera.SetProjectionMatrix(m_width, m_height);

	m_mesh[0] = new OGLMesh(L"../asset/models/Earth1.obj");
	m_texture[0] = new OGLTexture();
	m_texture[0]->CreateTextureFromFile( "../asset/texture/Earth.tga");
	m_mesh[1] = new OGLMesh(L"../asset/models/Earth1.obj");
	m_texture[1] = new OGLTexture();
	m_texture[1]->CreateTextureFromFile("../asset/texture/3.tga");
	m_mesh[2] = new OGLMesh(L"../asset/models/Earth1.obj");
	m_texture[2] = new OGLTexture();
	m_texture[2]->CreateTextureFromFile("../asset/texture/planet1.tga");
	m_mesh[3] = new OGLMesh(L"../asset/models/Earth1.obj");
	m_texture[3] = new OGLTexture();
	m_texture[3]->CreateTextureFromFile("../asset/texture/planet2.tga");
	m_mesh[4] = new OGLMesh(L"../asset/models/Earth1.obj");
	m_texture[4] = new OGLTexture();
	m_texture[4]->CreateTextureFromFile("../asset/texture/planet3.tga");

	srand(time(NULL));

	//Target *earth = new Target(m_mesh, m_texture);
	for (int i = 0; i < 5; i++)
	{
		Target *t = new Target(m_mesh[i], m_texture[i]);
		targets.push_back(t);
	}

	ground = new OGLMesh(L"../asset/models/Terrain1.obj");
	groundTex = new OGLTexture();
	groundTex->CreateTextureFromFile("../asset/texture/3.tga");
	ground->SetTexture(groundTex);
	
	ship = new OGLMesh(L"../asset/models/ship.obj");
	shipTex = new OGLTexture();
	shipTex->CreateTextureFromFile("../asset/texture/ship.tga");
	ship->SetTexture(shipTex);

	return TRUE;
}

void OGLWindow::Render()
{
	Renderable* skyCube = static_cast<Renderable*>(skybox);
	//Renderable* prenderable = static_cast<Renderable*>(m_mesh);
	Renderable* groundRender = static_cast<Renderable*>(ground);
	Renderable* shipRender = static_cast<Renderable*>(ship);

	glm::mat4 proj = *m_camera.GetProjMatrix();
	glm::mat4 view = *m_camera.GetViewMatrix();

	glm::vec3 camPos = (*m_camera.GetCameraPosition());
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glBindSampler(0, m_texDefaultSampler);

	glm::mat4 translationMat;
	glm::mat4 rotateMat;
	glm::mat4 scaleMat;

	skyShader->ActivateShaderProgram();

	glDisable(GL_DEPTH_TEST);
	glm::mat4 skyMat2 = glm::mat4(1.0f);
	rotateMat = glm::rotate(glm::mat4(1.0f), skyboxSpin += 0.0001f, glm::vec3(0.6f, 0.3f, 0));
	translationMat = glm::translate(glm::mat4(1.0f), camPos);
	scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	skyMat2 = translationMat * rotateMat * scaleMat;
	glUniformMatrix4fv(m_uniform_model2, 1, GL_FALSE, &skyMat2[0][0]);
	skyCube->Render();
	glEnable(GL_DEPTH_TEST);


	m_shader->ActivateShaderProgram();

	light_dir = glm::rotate(light_dir, 0.001f, glm::vec3(0.6f, 0.3f, 0));

	glUniform3f(m_uniform_lightdir, light_dir.x, light_dir.y, light_dir.z);

	for each (Target *t in targets)
	{
		glm::mat4 target = *t->GetModelMat();
		glUniformMatrix4fv(m_uniform_model, 1, GL_FALSE, &target[0][0]);
		t->Render();
		if (mouseDown && t->HitCheck(*m_camera.GetCameraPosition(), *m_camera.GetCameraDirection()))
		{
			score += 1;
			std::cout << "Score: " << score << "\n";
		}
	}

	glm::mat4 groundMat = glm::mat4(1.0f);
	rotateMat = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(1, 1, 1));
	translationMat = glm::translate(glm::mat4(1.0f), glm::vec3(0,-1000,0));
	scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	groundMat = translationMat * rotateMat * scaleMat;
	glUniformMatrix4fv(m_uniform_model, 1, GL_FALSE, &groundMat[0][0]);
	groundRender->Render();

	glm::mat4 shipMat = glm::mat4(1.0f);
	rotateMat = glm::rotate(glm::mat4(1.0f), 3.15f - roll, glm::vec3(0.0f, 0.0f, 1.0f));
	rotateMat *= glm::rotate(glm::mat4(1.0f), 3.15f + 0.01f * directionX, glm::vec3(0.0f, 1.0f, 0.0f));
	rotateMat *= glm::rotate(glm::mat4(1.0f), 0 - 0.03f * directionY, glm::vec3(1.0f, 0.0f, 0.0f));
	translationMat = glm::translate(glm::mat4(1.0f),(*m_camera.GetCameraUpVector() * 5.0f + *m_camera.GetCameraDirection() * 15.0f));
	//translationMat *= glm::translate(glm::mat4(1.0f), glm::vec3(-0.3f, -0.3f, -0.3f));
	scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(-2.0f, 2.0f, -2.0f));
	shipMat = translationMat / view * rotateMat;
	glUniformMatrix4fv(m_uniform_model, 1, GL_FALSE, &shipMat[0][0]);
	shipRender->Render();

	m_camera.PedCamera(speed * ped);
	m_camera.StrafeCamera(speed * strafe);
	m_camera.DollyCamera(speed * dolly);
	m_camera.RotateCamera(0, 0, 0.5f * -roll);
	m_camera.RotateCamera(0, 0.01f * directionX, 0); //Rotate Left or Right
	m_camera.RotateCamera(0.01f * directionY, 0, 0); //Rotate Left or Right

	skyShader->ActivateShaderProgram();

	proj = *m_camera.GetProjMatrix();
	view = *m_camera.GetViewMatrix();
	glUniformMatrix4fv(m_uniform_view2, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(m_uniform_projection2, 1, GL_FALSE, &proj[0][0]);

	m_shader->ActivateShaderProgram();

	proj = *m_camera.GetProjMatrix();
	view = *m_camera.GetViewMatrix();

	glUniformMatrix4fv(m_uniform_view, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(m_uniform_projection, 1, GL_FALSE, &proj[0][0]);

	SwapBuffers(m_hdc);
	return;
}

void OGLWindow::Resize( int width, int height )
{
	m_camera.SetCameraAspectRatio((float)width/(float)height);
	
	glViewport( 0, 0, width, height );
	
	m_width = width;
	m_height = height;

	//glMatrixMode( GL_PROJECTION );
	//glLoadIdentity();

	//m_camera.GetProjMatrix();

	//glMatrixMode( GL_MODELVIEW );
	//glLoadIdentity();

	return;
}

void OGLWindow::InitOGLState()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//Initialise OGL shader
	m_shader = new OGLShaderProgram();

	m_shader->CreateShaderProgram();
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/basic.vert", SHADER_VERTEX);
	m_shader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/basic.frag", SHADER_FRAGMENT);

	m_shader->BindAttributeLocation( 0, "position" );
	m_shader->BindAttributeLocation( 1, "inNormal" );
	m_shader->BindAttributeLocation( 2, "inUV" );

	glBindFragDataLocation( m_shader->GetProgramHandle(), 0, "outFrag" );

	m_shader->BuildShaderProgram();

	m_uniform_model = glGetUniformLocation(m_shader->GetProgramHandle(), "model");
	m_uniform_view = glGetUniformLocation(m_shader->GetProgramHandle(), "view");
	m_uniform_projection = glGetUniformLocation(m_shader->GetProgramHandle(), "projection");
	m_uniform_texture = glGetUniformLocation(m_shader->GetProgramHandle(), "texColour");
	m_uniform_lightdir = glGetUniformLocation(m_shader->GetProgramHandle(), "lightdir");

	skyShader = new OGLShaderProgram();

	skyShader->CreateShaderProgram();
	skyShader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/sky.vert", SHADER_VERTEX);
	skyShader->AttachAndCompileShaderFromFile(L"../asset/shader/glsl/sky.frag", SHADER_FRAGMENT);

	skyShader->BindAttributeLocation(0, "position");
	skyShader->BindAttributeLocation(1, "inNormal");
	skyShader->BindAttributeLocation(2, "inUV");

	glBindFragDataLocation(skyShader->GetProgramHandle(), 0, "outFrag");

	skyShader->BuildShaderProgram();

	m_uniform_model2 = glGetUniformLocation(skyShader->GetProgramHandle(), "model");
	m_uniform_view2 = glGetUniformLocation(skyShader->GetProgramHandle(), "view");
	m_uniform_projection2 = glGetUniformLocation(skyShader->GetProgramHandle(), "projection");
	m_uniform_texture2 = glGetUniformLocation(skyShader->GetProgramHandle(), "texColour");

	glActiveTexture(GL_TEXTURE0);
	glUniform1i( m_uniform_texture, 0);

	//Create a texture sampler
	glGenSamplers( 1, (GLuint*)(&m_texDefaultSampler) );
	
	glSamplerParameteri(m_texDefaultSampler , GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glSamplerParameteri(m_texDefaultSampler , GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
	glSamplerParameteri(m_texDefaultSampler , GL_TEXTURE_MIN_FILTER , GL_LINEAR);  
	glSamplerParameteri(m_texDefaultSampler , GL_TEXTURE_MAG_FILTER , GL_LINEAR);
}

BOOL OGLWindow::MouseLBDown ( int x, int y )
{
	if (!mouseDown)
	{
		clickX = x;
		clickY = y;
		mouseDown = true;
	}

	return TRUE;
}

BOOL OGLWindow::MouseLBUp ( int x, int y )
{
	mouseDown = false;
	//std::cout << clickX << " - " << clickY << "\n";
	return TRUE;
}

BOOL OGLWindow::MouseMove ( int x, int y )
{
	float centerX = m_width / 2;
	float centerY = m_height / 2;

	directionX = -(x - centerX) / 100;
	directionY = -(y - centerY) / 100;

	return TRUE;
}

BOOL OGLWindow::GetKeyDown(WPARAM key)
{
	switch (key)
	{
	case 0x57://W key
		dolly = -1;
		break;
	case 0x53://S key
		dolly = 1;
		break;
	case 0x41://A key
		strafe = -1;
		break;
	case 0x44://D key
		strafe = 1;
		break;
	case 0x20://Space
		ped = 1;
		break;
	case 0x10://Shift key
		speed = 20;
		break;
	case 0x45://E key
		roll = 0.1f;
		break;
	case 0x51://Q key
		roll = -0.1f;
		break;	
	case 0x11:
		ped = -1;
		break; 
	}

	return true;
}

BOOL OGLWindow::GetKeyUp(WPARAM key)
{
	switch (key)
	{
	case 0x57://W key
		dolly = 0;
		break;
	case 0x53://S key
		dolly = 0;
		break;
	case 0x41://A key
		strafe = 0;
		break;
	case 0x44://D key
		strafe = 0;
		break;
	case 0x20://Space
		ped = 0;
		break;
	case 0x10://Shift key
		speed = 5;
	case 0x45://E key
		roll = 0;
		break;
	case 0x51://Q key
		roll = 0;
		break;
	case 0x52:
		m_camera.ResetOrientation();
		break;
	case 0x11:
		ped = 0;
		break;
	case 0x09:
		m_camera.SwitchProjectionType();
		break;
	}
	return true;
}

BOOL OGLWindow::MouseWheel(WPARAM scroll)
{
	if (scroll == 120)
	{
		m_camera.ZoomCamera(0.1f);
	}
	else if (scroll == -120)
	{
		m_camera.ZoomCamera(-0.1f);
	}
	//std::cout << m_camera.GetCameraFOV() << "\n ";
	
	return true;
}

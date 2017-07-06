#pragma once

#include "RenderWindow.h"
#include "OGLCube.h"
#include "OGLMesh.h"
#include "OGLShader.h"
#include "OGLTexture.h"
#include <Camera.h>
#include <SkyBox.h>
#include <vector>
#include <Target.h>

class OGLWindow : public RenderWindow
{
	private:
		float	    score = 0;
		float		skyboxSpin = 0.0f;
		float		worldSpinLocal = 0.0f;
		float		worldSpin = 1.0f;
		float		shipRoll = 0.0f;
		glm::vec3	light_dir = glm::vec3(0.0f, 1.0f, 0.0f);

		float			speed = 5.0f;
		float			ped = 0;
		float			strafe = 0;
		float			dolly = 0;
		float			zoom = 0;
		float			roll = 0;
		glm::mat4		proj;

		int directionX, directionY;
		int clickX, clickY;
		float xDelta, yDelta; //initial position of mouse x and y when clicked
		bool mouseDown = false;

		HDC			m_hdc;				//handle to a device context
		HGLRC		m_hglrc;			//handle to a gl rendering context

		float			m_width;
		float			m_height;
		
		//This is not an ideal place to hold geometry data
		Renderable		*m_mesh[5];
		OGLTexture		*m_texture[5];

		std::vector<Target*> targets;

		Camera			m_camera;

		SkyBox			*skybox;
		OGLTexture      *skyTexture;

		Renderable		*ground;
		OGLTexture		*groundTex;

		Renderable		*ship;
		OGLTexture		*shipTex;

		//Declear an OGL shader program
		OGLShaderProgram		*m_shader;
		OGLShaderProgram		*skyShader;
		int						m_uniform_model;
		int						m_uniform_view;
		int						m_uniform_projection;
		int                     m_uniform_texture;
		int						m_uniform_lightdir;

		int						m_uniform_model2;
		int						m_uniform_view2;
		int						m_uniform_projection2;
		int                     m_uniform_texture2;

		int						m_texDefaultSampler;

protected:

		HGLRC CreateOGLContext (HDC hdc);
		BOOL DestroyOGLContext();
		void InitOGLState();

	public:
					OGLWindow();
					OGLWindow(HINSTANCE hInstance, int width, int height);
					~OGLWindow();
		
		BOOL		InitWindow(HINSTANCE hInstance, int width, int height);

		void		Render();
		void		Resize( int width, int height );
		void		DestroyRenderWindow();

		BOOL		MouseLBDown ( int x, int y );
		BOOL		MouseLBUp ( int x, int y );
		BOOL		MouseMove ( int x, int y );
		BOOL		GetKeyUp(WPARAM key);
		BOOL		GetKeyDown(WPARAM key);
		BOOL		MouseWheel(WPARAM scroll);
};

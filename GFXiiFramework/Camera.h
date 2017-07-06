//Created for Graphics I and II
//Author: Minsi Chen

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "vector3.h"
#include <GLM\glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
public:
	enum ECameraType
	{
		CAM_ORTHOGRAPHIC,
		CAM_PERSPECTIVE
	};

private:
	glm::mat4x4						m_viewMatrix;				//View Matrix
	glm::mat4x4						m_projectionMatrix;			//Projection Matrix
	glm::vec3						m_position;					//Position
	glm::vec3						m_upVector;					//up vector
	glm::vec3						m_rightVector;				//right vector
	glm::vec3						m_viewVector;				//view vector
	glm::vec3						m_lookAt;					//look at position
	ECameraType						m_cameratype;				//Projection time
	float							m_fov;						//vertical field of view in degree
	float							orthoZoom = 0.1f;					//orthographic size
	float							m_aspectRatio;				//Aspect ratio of Width/Height
	float							width, height;				//Screen width and height
public:

	Camera();
	virtual							~Camera();

	void							InitCamera(int i);
	void							SetCameraPosition(const glm::vec3* position);
	inline const glm::vec3*			GetCameraPosition() const
	{
		if (m_cameratype == CAM_ORTHOGRAPHIC)
			return &glm::vec3(0, 1000, 0);
		else
			return &m_position;
	}

	void							SetLookAtPoint(const glm::vec3* lookAt);
	inline const glm::vec3*			GetCameraDirection() const
	{
		return &m_viewVector;
	}
	void							SetUpVector(const glm::vec3* up);
	inline const glm::vec3*			GetCameraUpVector() const
	{
		return &m_upVector;
	}
	inline ECameraType				GetCameraType()
	{
		return m_cameratype;
	}

	inline void						SetCameraFOV(float fov)
	{
		m_fov = fov;
		UpdateProjMatrix();
	}

	inline float					GetCameraFOV()
	{
		return m_fov;
	}

	inline void						SetCameraAspectRatio(float ar)
	{
		m_aspectRatio = ar;
	}

	inline float					GetCameraAspectRatio()
	{
		return m_aspectRatio;
	}

	void							UpdateViewMatrix();

	inline glm::mat4*				GetViewMatrix() 
	{	
		return &m_viewMatrix;
	}

	void							UpdateProjMatrix();

	inline glm::mat4*				GetProjMatrix()
	{
		UpdateProjMatrix();
		return &m_projectionMatrix;
	}

	inline void							SwitchProjectionType()
	{
		if (m_cameratype == CAM_ORTHOGRAPHIC)
			m_cameratype = CAM_PERSPECTIVE;
		else
			m_cameratype = CAM_ORTHOGRAPHIC;

		UpdateProjMatrix();
	}

	//TODO: Setup the projection matrix
	void							SetProjectionMatrix(float screenX, float screenY);

	//TODO: Implement the following camera movement
	void							StrafeCamera(float amount);
	void							DollyCamera(float amount);
	void							PedCamera(float amount);
	void							RotateCamera(float yaw, float pitch, float roll);
	void							ZoomCamera(float amount);
	void							ResetOrientation();
};

#endif

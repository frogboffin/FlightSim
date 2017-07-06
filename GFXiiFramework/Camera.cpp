//Created for Graphics I and II
//Author: Minsi Chen

#include "Camera.h"
#include <GLM\gtc\type_ptr.hpp>
#include <iostream>

Camera::Camera()
{
	InitCamera(1);
}

Camera::~Camera()
{

}

void Camera::InitCamera(int i) //camera type 0 == ortho 1 == perspective
{
	if (i == 0)
		m_cameratype = CAM_ORTHOGRAPHIC;
	else
		m_cameratype = CAM_PERSPECTIVE;

	//Initialise the camera that sits at the origin looking down the negative z-axis.
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_upVector = glm::vec3(0.0f, -1.0f, 0.0f);
	m_viewVector = glm::vec3(0.0f, 0.0, -1.0f);
	m_rightVector = glm::cross(m_viewVector, m_upVector);
	m_upVector = glm::cross(m_rightVector, m_viewVector);
	m_fov = 60.0f;
	//m_aspectRatio = 16 / 9;
	UpdateViewMatrix();
	UpdateProjMatrix();
}

void Camera::SetCameraPosition(const glm::vec3* position)
{
	m_position = (*position);
	UpdateViewMatrix();
}

void Camera::SetLookAtPoint(const glm::vec3* lookAt)
{
	m_viewVector = *lookAt;

	glm::normalize(m_viewVector);
	glm::normalize(m_rightVector);
	glm::normalize(m_upVector);

	UpdateViewMatrix();
}

void Camera::SetUpVector(const glm::vec3* up)
{
	m_upVector = (*up);

	m_rightVector = glm::cross(m_viewVector, m_upVector);
	m_viewVector = glm::cross(m_rightVector, m_upVector);

	glm::normalize(m_viewVector);
	glm::normalize(m_rightVector);
	glm::normalize(m_upVector);
	
	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	float mat[16];

	mat[0] = m_rightVector[0];
	mat[4] = m_rightVector[1];
	mat[8] = m_rightVector[2];

	mat[1] = m_upVector[0];
	mat[5] = m_upVector[1];
	mat[9] = m_upVector[2];

	mat[2] = -m_viewVector[0];
	mat[6] = -m_viewVector[1];
	mat[10] = -m_viewVector[2];

	mat[3] = mat[7] = mat[11] = 0.0f;

	mat[12] = -m_position[0];
	mat[13] = -m_position[1];
	mat[14] = -m_position[2];
	mat[15] = 1.0f;

	//m_viewMatrix = glm::make_mat4(mat);
	//std::cout << m_position.x << " " << m_position.y << " " << m_position.z << "\n";
	m_viewMatrix = glm::lookAt(m_position, m_position + m_viewVector, m_upVector);
}

void Camera::UpdateProjMatrix()
{
	if (m_cameratype == CAM_ORTHOGRAPHIC)
	{
		m_projectionMatrix = (glm::ortho(0.0f, -width / orthoZoom, height / orthoZoom, 0.0f, 0.00001f, 30000.0f));
		m_position = glm::vec3(9450, 13440, 5250);
	}
	else
	{
		m_projectionMatrix = glm::perspective(m_fov / 2, m_aspectRatio, 0.1f, 20000.0f);
	}
}

void Camera::SetProjectionMatrix(float screenX, float screenY)
{
	width = screenX;
	height = screenY;

	if (m_cameratype == CAM_ORTHOGRAPHIC)
	{
		m_projectionMatrix = glm::ortho(0.0f, -screenX / orthoZoom, screenY / orthoZoom, 0.0f, 0.0001f, 20000.0f);
	}
	else
	{
		m_projectionMatrix = glm::perspective(GetCameraFOV() / 2, GetCameraAspectRatio(), 0.1f, 20000.0f);
	}

	UpdateProjMatrix();

	//m_projectionMatrix = glm::make_mat4x4(projmat);
	//UpdateViewMatrix();
}

void Camera::StrafeCamera(float amount)
{
	m_position -= amount * m_rightVector;

	//SetCameraPosition(&m_position);
	UpdateViewMatrix();

}

void Camera::PedCamera(float amount)
{
	m_position -= amount * m_upVector;

	//SetCameraPosition(&m_position);
	UpdateViewMatrix();

}

void Camera::DollyCamera(float amount)
{
	m_position += amount * -m_viewVector;

	//SetCameraPosition(&m_position);
	UpdateViewMatrix();

}

void Camera::ZoomCamera(float amount)
{
	if (m_cameratype == CAM_ORTHOGRAPHIC)
	{
		orthoZoom += amount/10;
	}
	else
	{
		m_fov += amount;
	}

	UpdateProjMatrix();
	//UpdateViewMatrix();
}

void Camera::RotateCamera(float yaw, float pitch, float roll)
{
	//std::cout << "Up" << m_upVector.x << "-" << m_upVector.y << "-" << m_upVector.z << "\n";
	//std::cout << "Right" << m_rightVector.x << "-" << m_rightVector.y << "-" << m_rightVector.z << "\n";
	//std::cout << "View" << m_viewVector.x << "-" << m_viewVector.y << "-" << m_viewVector.z << "\n";

	glm::quat _pitch(cos(pitch /2), m_upVector.x * sin(pitch/2), m_upVector.y * sin(pitch/2), m_upVector.z * sin(pitch/2));
	glm::quat _yaw(cos(yaw / 2), m_rightVector.x * sin(yaw / 2), m_rightVector.y * sin(yaw / 2), m_rightVector.z * sin(yaw / 2));
	glm::quat _roll(cos(roll / 2), m_viewVector.x * sin(roll / 2), m_viewVector.y * sin(roll / 2), m_viewVector.z * sin(roll / 2));
	
	//glm::quat rotation =_yaw * _pitch * _roll;
	glm::quat rotation = _pitch * _roll * _yaw;

	glm::vec3 euler = glm::eulerAngles(rotation) * 3.14159f / 180.f;
	//m_viewMatrix /= glm::mat4_cast((rotation));

	//(glm::mat4_cast(glm::normalize(rotation)));

	m_viewMatrix *= glm::mat4(euler[0]);
	//m_viewMatrix *= rotation[0];

	m_upVector = (m_upVector *_roll *_yaw);
	m_rightVector = (m_rightVector * _pitch *_roll);
	m_viewVector = (m_viewVector * _pitch *_yaw);


	UpdateViewMatrix();
}

void Camera::ResetOrientation()
{
	m_upVector = glm::vec3(0.0f, -1.0f, 0.0f);
	m_rightVector = glm::vec3(-1.0f, 0.0, 0.0f);
	m_viewVector = -glm::cross(m_rightVector, m_upVector);
	UpdateViewMatrix();
}


#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

using namespace las;

Camera::Camera() : worldTransform(1), m_isPerspective(true), dirtyView(true), dirtyProjection(true), dirtyProjectionView(true)
{
	setPerspective(60.f, 16.f / 9.f, 1, 1000.f);
}

Camera::~Camera()
{
}

void Camera::setPerspective(float fov, float aspectRatio, float near, float far)
{
	m_fov = fov;
	m_aspectRatio = aspectRatio;
	m_near = near;
	m_far = far;
	m_isPerspective = true;
	dirtyProjection = true;
}

void las::Camera::setFoV(float fov)
{
	m_fov = fov;
	dirtyProjection = true;
}

void las::Camera::setAspectRatio(float aspectRatio)
{
	m_aspectRatio = aspectRatio;
	dirtyProjection = true;
}

void las::Camera::setClippingPlanes(float near, float far)
{
	m_near = near;
	m_far = far;
	dirtyProjection = true;
}

void Camera::setLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up)
{
	//TODO check correctness? (either arguments valid, or check lookAt result)
	viewTransform = glm::lookAt(from, to, up);
	worldTransform = glm::inverse(viewTransform);
	// If setting with lookat, view is now correct
	dirtyView = false;
	dirtyProjectionView = true;
}

void Camera::setPosition(glm::vec3 position)
{
	glm::vec4 tAxis = { position.x, position.y, position.z, 1 };
	worldTransform[3] = tAxis;
	dirtyView = true;
}

glm::vec3 las::Camera::getPosition()
{
	return glm::vec3(worldTransform[3]);
}

glm::mat4 Camera::getWorldTransform()
{
	return worldTransform;
}

glm::mat4 Camera::getView()
{
	return viewTransform;
}

glm::mat4 Camera::getProjection()
{
	updateProjection();
	return projectionTransform;
}

glm::mat4 Camera::getProjectionView()
{
	updateProjectionViewTransform();
	return projectionViewTransform;
}

void Camera::updateView()
{
	if (dirtyView) {
		viewTransform = glm::inverse(worldTransform);
		dirtyView = false;
		dirtyProjectionView = true;
	}
}

void las::Camera::updateProjection()
{
	if (dirtyProjection) {
		if (m_isPerspective) {
			projectionTransform = glm::perspective(m_fov, m_aspectRatio, m_near, m_far);
		}
		else {
			//TODO calculate othographic view 
		}
		dirtyProjection = false;
		dirtyProjectionView = true;
	}
}

void Camera::updateProjectionViewTransform()
{
	updateView();
	updateProjection();
	if (dirtyProjectionView) {
		projectionViewTransform = projectionTransform * viewTransform;
		dirtyProjectionView = false;
	}
}

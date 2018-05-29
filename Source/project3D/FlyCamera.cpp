#include "FlyCamera.h"
#include <algorithm>

#include <glm/gtc/matrix_transform.hpp>

#include "Gizmos.h"

using aie::Input;
using glm::vec3;
using glm::vec4;
using glm::mat4;

las::FlyCamera::FlyCamera(glm::vec3 pos, glm::vec3 target) : m_pos(pos), m_targetPos(target), m_speed(5), m_targetSpeed(5), m_zoomSpeed(5)
{
}

las::FlyCamera::~FlyCamera()
{
}

void las::FlyCamera::update(float deltaTime)
{
	float minDistance = 0.01f;

	vec3 displacement = m_targetPos - m_pos;

	float distance = glm::length(displacement);
	float distanceRatio = distance * 0.1f;

	vec3 forward = glm::normalize(displacement);
	vec3 right = glm::normalize(vec3(-forward.z, 0, forward.x));
	vec3 up = { 0,1,0 };

	Input* input = Input::getInstance();

	// Calculate movement
	float zoomMove = distanceRatio * m_zoomSpeed * deltaTime;
	float targetMove = distanceRatio * m_targetSpeed * deltaTime;
	float move = distanceRatio * m_speed * deltaTime;

	if (input->isKeyDown(zoomIn)) {
		zoomMove = std::min(zoomMove, distance - minDistance);
		m_pos += forward * zoomMove;
	}
	else if (input->isKeyDown(zoomOut)) {
		m_pos -= forward * zoomMove;
	}

	if (input->isKeyDown(moveRight)) {
		m_pos += move * right;
	}
	else if (input->isKeyDown(moveLeft)) {
		m_pos -= move * right;
	}

	if (input->isKeyDown(moveUp)) {
		m_pos += move * up;
	}
	else if (input->isKeyDown(moveDown)) {
		m_pos -= move * up;
	}

	if (input->isKeyDown(panUp)) {
		m_targetPos += targetMove * up;
	}
	else if (input->isKeyDown(panDown)) {
		m_targetPos -= targetMove * up;
	}

	if (input->isKeyDown(panRight)) {
		m_targetPos += targetMove * right;
	}
	else if (input->isKeyDown(panLeft)) {
		m_targetPos -= targetMove * right;
	}

	setLookAt(m_pos, m_targetPos);
}

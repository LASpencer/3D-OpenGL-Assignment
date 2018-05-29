#pragma once
#include "Camera.h"
#include "Input.h"

namespace las {
	class FlyCamera : public Camera {
	public:
		FlyCamera(glm::vec3 pos, glm::vec3 target);

		virtual ~FlyCamera();

		virtual void update(float deltaTime);

		glm::vec3 getTargetPos() { return m_targetPos; }

		const aie::EInputCodes moveLeft = aie::EInputCodes::INPUT_KEY_A;
		const aie::EInputCodes moveRight = aie::EInputCodes::INPUT_KEY_D;
		const aie::EInputCodes zoomIn = aie::EInputCodes::INPUT_KEY_W;
		const aie::EInputCodes zoomOut = aie::EInputCodes::INPUT_KEY_S;
		const aie::EInputCodes moveUp = aie::EInputCodes::INPUT_KEY_LEFT_SHIFT;
		const aie::EInputCodes moveDown = aie::EInputCodes::INPUT_KEY_LEFT_CONTROL;

		const aie::EInputCodes panUp = aie::EInputCodes::INPUT_KEY_UP;
		const aie::EInputCodes panDown = aie::EInputCodes::INPUT_KEY_DOWN;
		const aie::EInputCodes panLeft = aie::EInputCodes::INPUT_KEY_LEFT;
		const aie::EInputCodes panRight = aie::EInputCodes::INPUT_KEY_RIGHT;


	protected:
		glm::vec3 m_pos;
		glm::vec3 m_targetPos;

		float m_speed;
		float m_zoomSpeed;
		float m_targetSpeed;
	};
}
#pragma once
#include <glm/glm.hpp>

namespace las {
	class Camera {
	public:
		Camera();

		virtual ~Camera();

		virtual void update(float deltaTime) {};

		void setPerspective(float fov, float aspectRatio, float near, float far);

		void setFoV(float fov);

		void setAspectRatio(float aspectRatio);

		void setClippingPlanes(float near, float far);

		// TODO allow camera to be set to orthographic (setOrthographic, orthographic functions)

		void setLookAt(glm::vec3 from, glm::vec3 to, glm::vec3 up = { 0,1,0 });

		void setPosition(glm::vec3 position);

		glm::vec3 getPosition();

		glm::mat4 getWorldTransform();

		glm::mat4 getView();

		glm::mat4 getProjection();

		glm::mat4 getProjectionView();


	protected:
		void updateView();
		void updateProjection();
		void updateProjectionViewTransform();

		glm::mat4 worldTransform;
		glm::mat4 viewTransform;
		glm::mat4 projectionTransform;
		glm::mat4 projectionViewTransform;

		float m_fov;
		float m_aspectRatio;
		float m_near;
		float m_far;

		bool m_isPerspective;
		bool dirtyView;
		bool dirtyProjection;
		bool dirtyProjectionView;
	};
}
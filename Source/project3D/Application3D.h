#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

#include "Texture.h"

#include "Shader.h"
#include "Camera.h"
#include "FlyCamera.h"
#include "OBJMesh.h"
#include "Instance.h"
#include "Light.h"
#include "Scene.h"

namespace las {
	class Application3D : public aie::Application {
	public:

		Application3D();
		virtual ~Application3D();

		virtual bool startup();
		virtual void shutdown();

		virtual void update(float deltaTime);
		virtual void draw();

	protected:

		Camera *				m_camera;
		Scene*					m_scene;

		aie::ShaderProgram		m_texturedPhong;

		OBJMesh					m_spearMesh;

		DirectionalLight*		m_directionalLight;
		PointLight*				m_pointLight;
		SpotLight*				m_spotLight;
	};
}
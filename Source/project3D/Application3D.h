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
#include "PostProcessor.h"

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
		aie::ShaderProgram		m_phong;
		aie::ShaderProgram		m_diffuseTexture;
		aie::ShaderProgram		m_glowTexture;

		PostProcessor*			m_postProcessor;

		OBJMesh					m_spearMesh;
		OBJMesh					m_dragonMesh;
		OBJMesh					m_deerMesh;
		OBJMesh					m_foxMesh;
		OBJMesh					m_shibaMesh;
		OBJMesh					m_chairMesh;
		OBJMesh					m_corvetteMesh;
		OBJMesh					m_carrierMesh;
		OBJMesh					m_cruiserMesh;
		OBJMesh					m_destroyerMesh;
		OBJMesh					m_frigateMesh;

		DirectionalLight*		m_directionalLight;
		PointLight*				m_pointLight;
		SpotLight*				m_spotLight;

		float					directionalYaw;
		float					directionalPitch;

		float					spotYaw;
		float					spotPitch;

		Colour					directionalColour;
		Colour					pointColour;
		Colour					spotColour;

	};
}
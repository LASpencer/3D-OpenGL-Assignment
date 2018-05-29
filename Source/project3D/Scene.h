#pragma once
#include <vector>

#include <glm/glm.hpp>

#include "Shader.h"
#include "OBJMesh.h"

namespace las {

	class Camera;
	class Instance;
	class PointLight;
	class DirectionalLight;

	class Scene {
	public:
		// Binding points for light uniform buffer
		static const int dirLightBufferBindPoint = 1;
		static const int pointLightBufferBindPoint = 2;

		static const int maxDirectionalLights = 32;	// If changed, make sure changed in all shaders
		static const int maxPointLights = 32;

		Scene();

		virtual ~Scene();

		std::vector<Instance*>& getInstances() { return m_instances; }
		std::vector<DirectionalLight*>& getDirectionalLights() { return m_directionalLights; }
		std::vector<PointLight*>& getPointLights() { return m_pointLights; }

		virtual void update(float deltaTime) {};

		virtual void draw(Camera* camera);

		bool bindShaderUniforms(aie::ShaderProgram* shader);

		glm::vec3 getAmbient() { return m_ambientLight; }
		void setAmbient(glm::vec3 ambient);

		void setLights(std::vector<DirectionalLight*>& lights);
		void setLights(std::vector<PointLight*>& lights);

		//TODO adding lights with parameters

		bool addDirectionalLight(DirectionalLight* light);

		bool addPointLight(PointLight* light);

		void destroyLight(DirectionalLight* light);

		void destroyLight(PointLight* light);

		void clearLights();

		void setInstances(std::vector<Instance*>& instances);

		void addInstance(Instance* instance);

		Instance* addInstance(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, aie::ShaderProgram* shader, OBJMesh* mesh);

		void destroyInstance(Instance* instance);

		void clearInstances();

	protected:
		glm::vec3 m_ambientLight;

		std::vector<Instance*> m_instances;

		std::vector<DirectionalLight*> m_directionalLights;
		std::vector<PointLight*> m_pointLights;

		unsigned int dirLightUBO;
		unsigned int pointLightUBO;
	};
}
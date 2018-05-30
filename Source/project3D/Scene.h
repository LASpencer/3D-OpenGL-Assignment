#pragma once
#include <vector>

#include <gl_core_4_4.h>
#include <glm/glm.hpp>

#include "Shader.h"
#include "OBJMesh.h"

namespace las {

	class Camera;
	class Instance;
	class PointLight;
	class DirectionalLight;
	class SpotLight;

	class Scene {
	public:
		// Binding points for light uniform buffer
		static const int dirLightBufferBindPoint = 1;
		static const int pointLightBufferBindPoint = 2;
		static const int spotLightBufferBindPoint = 3;

		static const int maxDirectionalLights = 32;	// If changed, make sure changed in all shaders
		static const int maxPointLights = 32;
		static const int maxSpotLights = 32;

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
		void setLights(std::vector<SpotLight*>& lights);

		//TODO adding lights with parameters

		bool addDirectionalLight(DirectionalLight* light);

		bool addPointLight(PointLight* light);

		bool addSpotLight(SpotLight* light);

		void destroyLight(DirectionalLight* light);

		void destroyLight(PointLight* light);

		void destroyLight(SpotLight* light);

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
		std::vector<SpotLight*> m_spotLights;

		unsigned int dirLightUBO;
		unsigned int pointLightUBO;
		unsigned int spotLightUBO;

		template<typename T>
		unsigned int generateLightBuffer(int maxLights);

		template<typename T>
		void bindLightBuffer(const std::vector<T*>& lights, unsigned int buffer, unsigned int uboBindPoint, int maxLights);

		bool bindUBO(aie::ShaderProgram* shader, const char* ubName, unsigned int uboBindPoint);
	};
	template<typename T>
	inline unsigned int Scene::generateLightBuffer(int maxLights)
	{
		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(T) * maxLights + 4, NULL, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
	template<typename T>
	inline void Scene::bindLightBuffer(const std::vector<T*>& lights, unsigned int buffer, unsigned int uboBindPoint, int maxLights)
	{
		// Update buffer data
		// Format is array of structs followed by size
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		int numLights = std::min(lights.size(), maxLights);
		for (int i = 0; i < numLights; ++i) {
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(T) * i, sizeof(T), lights[i]);
		}
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(T) * maxLights, 4, &numLights);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		// Set binding point
		glBindBufferBase(GL_UNIFORM_BUFFER, uboBindPoint, buffer);
	}
}
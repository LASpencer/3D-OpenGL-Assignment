#include "Scene.h"

#include <gl_core_4_4.h>

#include "Utility.h"
#include "Instance.h"
#include "Light.h"

las::Scene::Scene() : m_instances(), m_directionalLights(), m_pointLights()
{
	// TODO write "GenerateLightBuffer<T>" function
	// Create lighting buffers
	glGenBuffers(1, &dirLightUBO);
	glGenBuffers(1, &pointLightUBO);

	glBindBuffer(GL_UNIFORM_BUFFER, dirLightUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(DirectionalLight) * maxDirectionalLights + 4, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, pointLightUBO);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(PointLight) * maxPointLights + 4, NULL, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

las::Scene::~Scene()
{
	glDeleteBuffers(1, &dirLightUBO);
	glDeleteBuffers(1, &pointLightUBO);

	for (DirectionalLight* l : m_directionalLights) {
		delete l;
	}

	for (PointLight* l : m_pointLights) {
		delete l;
	}

	for (Instance* i : m_instances) {
		delete i;
	}
}

void las::Scene::draw(Camera * camera)
{
	//TODO write "BindLightBuffer<T>()" function
	// Copy directional light data to dirLightUBO block
	glBindBuffer(GL_UNIFORM_BUFFER, dirLightUBO);
	// Copy each light into buffer
	int numLights = m_directionalLights.size();
	for (int i = 0; i < numLights && i < maxDirectionalLights; ++i) {
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(DirectionalLight) * i, sizeof(DirectionalLight), m_directionalLights[i]);
	}
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(DirectionalLight) * maxDirectionalLights, 4, &numLights);

	// Copy point light into buffer
	glBindBuffer(GL_UNIFORM_BUFFER, pointLightUBO);
	numLights = m_pointLights.size();
	for (int i = 0; i < numLights && i < maxPointLights; ++i) {
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(PointLight) * i, sizeof(PointLight), m_pointLights[i]);
	}
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(PointLight) * maxPointLights, 4, &numLights);

	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	// Set binding points
	glBindBufferBase(GL_UNIFORM_BUFFER, dirLightBufferBindPoint, dirLightUBO);
	glBindBufferBase(GL_UNIFORM_BUFFER, pointLightBufferBindPoint, pointLightUBO);

	//TODO spotlights

	for (Instance* i : m_instances) {
		i->draw(camera, this);
	}
}

bool las::Scene::bindShaderUniforms(aie::ShaderProgram * shader)
{
	// Bind lights to shader
	unsigned int dirLightIndex = glGetUniformBlockIndex(shader->getHandle(), "DirLights");
	unsigned int pointLightIndex = glGetUniformBlockIndex(shader->getHandle(), "PointLights");

	if (dirLightIndex != GL_INVALID_INDEX) {
		glUniformBlockBinding(shader->getHandle(), dirLightIndex, dirLightBufferBindPoint);
	}

	if (pointLightIndex != GL_INVALID_INDEX) {
		glUniformBlockBinding(shader->getHandle(), pointLightIndex, pointLightBufferBindPoint);
	}

	shader->bindUniform("Ia", m_ambientLight);

	return true;
}

void las::Scene::setAmbient(glm::vec3 ambient)
{
	m_ambientLight = ambient;
}

void las::Scene::setLights(std::vector<DirectionalLight*>& lights)
{
	for (DirectionalLight* l : m_directionalLights) {
		delete l;
	}
	m_directionalLights = lights;
}

void las::Scene::setLights(std::vector<PointLight*>& lights)
{
	for (PointLight* l : m_pointLights) {
		delete l;
	}
	m_pointLights = lights;
}

bool las::Scene::addDirectionalLight(DirectionalLight * light)
{
	// return false if too many lights
	if (m_directionalLights.size() < maxDirectionalLights) {
		m_directionalLights.push_back(light);
		return true;
	}
	else {
		return false;
	}
}

bool las::Scene::addPointLight(PointLight * light)
{
	// return false if too many lights
	if (m_pointLights.size() < maxPointLights) {
		m_pointLights.push_back(light);
		return true;
	}
	else {
		return false;
	}
}

void las::Scene::destroyLight(DirectionalLight * light)
{
	removeEraseFromVector(m_directionalLights, light);
	delete light;
}

void las::Scene::destroyLight(PointLight * light)
{
	removeEraseFromVector(m_pointLights, light);
	delete light;
}

void las::Scene::clearLights()
{
	for (DirectionalLight* l : m_directionalLights) {
		delete l;
	}
	for (PointLight* l : m_pointLights) {
		delete l;
	}
	m_directionalLights.clear();
	m_pointLights.clear();
}

void las::Scene::setInstances(std::vector<Instance*>& instances)
{
	for (Instance* i : m_instances) {
		delete i;
	}
	m_instances = instances;
}

void las::Scene::addInstance(Instance * instance)
{
	m_instances.push_back(instance);
}

las::Instance * las::Scene::addInstance(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, aie::ShaderProgram * shader, OBJMesh * mesh)
{
	Instance* instance = new Instance(pos, rotation, scale, shader, mesh);
	m_instances.push_back(instance);
	return instance;
}

void las::Scene::destroyInstance(Instance * instance)
{
	removeEraseFromVector(m_instances, instance);
	delete instance;
}

void las::Scene::clearInstances()
{
	for (Instance* i : m_instances) {
		delete i;
	}
	m_instances.clear();
}

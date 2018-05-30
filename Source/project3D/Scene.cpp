#include "Scene.h"

#include "Utility.h"
#include "Instance.h"
#include "Light.h"

las::Scene::Scene() : m_instances(), m_directionalLights(), m_pointLights()
{
	// Create lighting buffers
	dirLightUBO = generateLightBuffer<DirectionalLight>(maxDirectionalLights);
	pointLightUBO = generateLightBuffer<PointLight>(maxPointLights);
	spotLightUBO = generateLightBuffer<SpotLight>(maxSpotLights);

}

las::Scene::~Scene()
{
	glDeleteBuffers(1, &dirLightUBO);
	glDeleteBuffers(1, &pointLightUBO);
	glDeleteBuffers(1, &spotLightUBO);

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
	// Update light info
	bindLightBuffer(m_directionalLights, dirLightUBO, dirLightBufferBindPoint, maxDirectionalLights);
	bindLightBuffer(m_pointLights, pointLightUBO, pointLightBufferBindPoint, maxPointLights);
	bindLightBuffer(m_spotLights, spotLightUBO, spotLightBufferBindPoint, maxSpotLights);


	for (Instance* i : m_instances) {
		i->draw(camera, this);
	}
}

bool las::Scene::bindShaderUniforms(aie::ShaderProgram * shader)
{
	// Bind lights to shader
	bindUBO(shader, "DirLights", dirLightBufferBindPoint);
	bindUBO(shader, "PointLights", pointLightBufferBindPoint);
	bindUBO(shader, "SpotLights", spotLightBufferBindPoint);

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

inline bool las::Scene::bindUBO(aie::ShaderProgram * shader, const char * ubName, unsigned int uboBindPoint)
{
	unsigned int blockIndex = glGetUniformBlockIndex(shader->getHandle(), ubName);
	if (blockIndex != GL_INVALID_INDEX) {
		glUniformBlockBinding(shader->getHandle(), blockIndex, uboBindPoint);
		return true;
	}
	else {
		return false;
	}
}

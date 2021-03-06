#include "Instance.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Scene.h"
#include "Camera.h"

las::Instance::Instance()
{
}

las::Instance::Instance(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, aie::ShaderProgram* shader, OBJMesh* mesh) :
	m_pos(pos), m_rotation(rotation), m_scale(scale), m_shader(shader), m_mesh(mesh), m_transformDirty(true)
{
}

const glm::mat4 & las::Instance::getTransform()
{
	calculateTransform();
	return m_transform;
}

void las::Instance::setPosition(glm::vec3 & pos)
{
	m_pos = pos;
	m_transformDirty = true;
}

void las::Instance::setRotation(glm::vec3 & euler)
{
	m_rotation = euler;
	m_transformDirty = true;
}

void las::Instance::setScale(glm::vec3 & scale)
{
	m_scale = scale;
	m_transformDirty = true;
}

void las::Instance::setShader(aie::ShaderProgram * shader)
{
	m_shader = shader;
}

void las::Instance::setMesh(OBJMesh * mesh)
{
	m_mesh = mesh;
}

void las::Instance::draw(las::Camera* camera, Scene * scene)
{
	calculateTransform();

	m_shader->bind();
	m_shader->bindUniform("ProjectionViewModel", camera->getProjectionView() * m_transform);
	m_shader->bindUniform("ModelMatrix", m_transform);
	m_shader->bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_transform)));
	m_shader->bindUniform("CameraPosition", camera->getPosition());
	scene->bindShaderUniforms(m_shader);
	m_mesh->draw();
}

void las::Instance::calculateTransform()
{
	if (m_transformDirty) {
		glm::mat4 translation, rotation, scale;

		translation = glm::translate(glm::mat4(1), m_pos);
		rotation = mat4_cast(glm::quat(m_rotation));
		scale = glm::scale(glm::mat4(1), m_scale);

		m_transform = translation * rotation * scale;

		m_transformDirty = false;
	}
}

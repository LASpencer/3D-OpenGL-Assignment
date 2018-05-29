#include "Instance.h"
#include "Scene.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

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

void las::Instance::draw(glm::mat4 pv, Scene * scene)
{
	calculateTransform();

	m_shader->bind();
	m_shader->bindUniform("ProjectionViewModel", pv * m_transform);
	m_shader->bindUniform("ModelMatrix", m_transform);
	m_shader->bindUniform("NormalMatrix", glm::inverseTranspose(glm::mat3(m_transform)));
	scene->bindShaderUniforms(m_shader);
	m_mesh->draw();
}

void las::Instance::calculateTransform()
{
	if (m_transformDirty) {
		glm::mat4 translation, rotation, scale;

		glm::translate(translation, m_pos);
		rotation = mat4_cast(glm::quat(m_rotation));
		glm::scale(scale, m_scale);

		m_transform = translation * rotation * scale;

		m_transformDirty = false;
	}
}

#pragma once
#include <glm/mat4x4.hpp>

#include "Shader.h"
#include "OBJMesh.h"

namespace las {
	class Scene;
	class Camera;
	// Instance of a mesh to draw, including transform and shader to apply
	class Instance {
	public:
		Instance();

		Instance(glm::vec3 pos, glm::vec3 rotation, glm::vec3 scale, aie::ShaderProgram* shader, OBJMesh* mesh);

		//TODO accessors, change transform with pos/rot/scale
		const glm::mat4& getTransform();

		glm::vec3 getPosition() { return m_pos; }
		void setPosition(glm::vec3& pos);

		glm::vec3 getRotation() { return m_rotation; }
		void setRotation(glm::vec3& euler);

		glm::vec3 getScale() { return m_scale; }
		void setScale(glm::vec3& scale);

		aie::ShaderProgram* getShader() { return m_shader; }
		void setShader(aie::ShaderProgram* shader);

		OBJMesh* getMesh() { return m_mesh; }
		void setMesh(OBJMesh* mesh);

		void draw(Camera* camera, Scene* scene);

	protected:
		glm::mat4 m_transform;
		
		glm::vec3 m_pos;
		glm::vec3 m_rotation;
		glm::vec3 m_scale;

		bool m_transformDirty;

		aie::ShaderProgram* m_shader;
		OBJMesh* m_mesh;

		void calculateTransform();
	};

}
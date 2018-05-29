#pragma once
#include <glm/glm.hpp>


namespace las {
	struct DirectionalLight {

		DirectionalLight() {};

		DirectionalLight(glm::vec3 dir, glm::vec3 Id, glm::vec3 Is) : direction(dir), diffuse(Id), specular(Is)
		{};

		glm::vec3 direction;	glm::float32_t pad1;
		glm::vec3 diffuse;		glm::float32_t pad2;
		glm::vec3 specular;		glm::float32_t pad3;
	};

	struct PointLight {
		PointLight() {};
		PointLight(glm::vec4 pos, glm::vec3 Id, glm::vec3 Is, float _linearAttenuation, float _quadraticAttenuation) :
			position(pos), diffuse(Id), specular(Is), linearAttenuation(_linearAttenuation), quadraticAttenuation(_quadraticAttenuation)
		{};

		glm::vec4 position;
		glm::vec3 diffuse;		glm::float32_t pad1;
		glm::vec3 specular;		glm::float32_t pad2;
		float linearAttenuation;
		float quadraticAttenuation;
		float pad3;
		float pad4;
	};

	struct SpotLight {
		
		glm::vec4 position;
		glm::vec3 direction;	glm::float32_t pad1;
		glm::vec3 diffuse;		glm::float32_t pad2;
		glm::vec3 specular;		glm::float32_t pad3;
		float phi;
		float theta;
		float pad4;
		float pad5;

	};
}
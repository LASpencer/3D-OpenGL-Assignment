#pragma once
#include <glm/glm.hpp>


namespace las {
	struct Colour {
		glm::vec3 colour;
		float intensity;
	};

	struct DirectionalLight {

		DirectionalLight() {};

		DirectionalLight(glm::vec3 dir, glm::vec3 Id, glm::vec3 Is) : direction(dir), diffuse(Id), specular(Is)
		{};

		glm::vec3 direction;	glm::float32_t pad1;
		glm::vec3 diffuse;		glm::float32_t pad2;
		glm::vec3 specular;		glm::float32_t pad3;

		void setColour(Colour colour) {
			diffuse = specular = colour.colour * colour.intensity;
		}
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

		void setColour(Colour colour) {
			diffuse = specular = colour.colour * colour.intensity;
		}
	};

	struct SpotLight {
		SpotLight() {};
		SpotLight(glm::vec4 pos, glm::vec3 dir, glm::vec3 Id, glm::vec3 Is, float _phi, float _theta, float linAtten, float quadAtten) :
			position(pos), direction(dir), diffuse(Id), specular(Is), phi(_phi), theta(_theta), linearAttenuation(linAtten), quadraticAttenuation(quadAtten)
		{};
			
		glm::vec4 position;
		glm::vec3 direction;	glm::float32_t pad1;
		glm::vec3 diffuse;		glm::float32_t pad2;
		glm::vec3 specular;		glm::float32_t pad3;
		float phi;
		float theta;
		float linearAttenuation;
		float quadraticAttenuation;

		void setColour(Colour colour) {
			diffuse = specular = colour.colour * colour.intensity;
		}
	};
}
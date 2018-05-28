#pragma once
#include "Texture.h"

namespace las {
	class Mesh {
	public:
		// a vertex structure for obj files
		struct Vertex {
			glm::vec4 position; // added to attrib location 0
			glm::vec4 normal;	// added to attrib location 1
			glm::vec2 texcoord;	// added to attrib location 2

								// normal-mapping data
			glm::vec4 tangent;	// added to attrib location 3
		};

		// a basic material
		class Material {
		public:

			Material() : ambient(1), diffuse(1), specular(0), emissive(0), specularPower(1), opacity(1) {}
			~Material() {}

			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			glm::vec3 emissive;

			float specularPower;
			float opacity;

			aie::Texture diffuseTexture;			// bound slot 0
			aie::Texture alphaTexture;				// bound slot 1
			aie::Texture ambientTexture;			// bound slot 2
			aie::Texture specularTexture;			// bound slot 3
			aie::Texture specularHighlightTexture;	// bound slot 4
			aie::Texture normalTexture;				// bound slot 5
			aie::Texture displacementTexture;		// bound slot 6
		};

		//TODO bring draw, getMaterial, getMaterialCount into here
	};
}
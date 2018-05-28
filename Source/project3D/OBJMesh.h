#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <vector>
#include "Texture.h"
#include "Mesh.h"

namespace las {

// a simple triangle mesh wrapper
class OBJMesh : public Mesh  {
public:

	OBJMesh() {}
	~OBJMesh();

	// will fail if a mesh has already been loaded in to this instance
	bool load(const char* filename, bool loadTextures = true, bool flipTextureV = false);

	// allow option to draw as patches for tessellation
	void draw(bool usePatches = false);

	// access to the filename that was loaded
	const std::string& getFilename() const { return m_filename; }

	// material access
	size_t getMaterialCount() const { return m_materials.size();  }
	Material& getMaterial(size_t index) { return m_materials[index];  }

private:

	void calculateTangents(std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);

	struct MeshChunk {
		unsigned int	vao, vbo, ibo;
		unsigned int	indexCount;
		int				materialID;
	};

	std::string				m_filename;
	std::vector<MeshChunk>	m_meshChunks;
	std::vector<Material>	m_materials;
};

}
#version 410
layout( location = 0) in vec4 Position;
layout( location = 1 ) in vec4 Normal;
layout( location = 2 ) in vec2 TexCoord;
layout( location = 3 ) in vec4 Tangent;

out vec4 vPosition;
out vec3 vNormal;
out vec3 vTangent;
out vec3 vBiTangent;
out vec2 vTexCoord;

uniform mat4 ProjectionViewModel;
uniform mat4 ModelMatrix;

void main() {
	vPosition = ModelMatrix * Position;
	vNormal = (ModelMatrix * Normal).xyz;
	vTangent = (ModelMatrix * Tangent).xyz;
	vBiTangent = cross(vNormal, vTangent) * Tangent.w;
	vTexCoord = TexCoord;

	gl_Position = ProjectionViewModel * Position;
}
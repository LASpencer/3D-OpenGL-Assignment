#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;
uniform float offset;
uniform float amplitude;
uniform float numWaves;
uniform vec2 direction;

out vec4 FragColour;

void main(){
	// calculate texel size
	vec2 texSize = textureSize(colourTarget, 0);
	vec2 texelSize = 1.0f / texSize;
	
	// Adjust texture coordinate to avoid missampling edges
	vec2 scale = (texSize - texelSize) / texSize;
	vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

	vec2 dirNormal = normalize(direction);
	vec2 orthoDirection = vec2(-dirNormal.y, dirNormal.x);
	float waveY = dot(orthoDirection,texCoord);
	texCoord += dirNormal * sin(waveY * 2 * 3.14159 * numWaves + offset) * amplitude;
	FragColour = texture(colourTarget,texCoord);
}
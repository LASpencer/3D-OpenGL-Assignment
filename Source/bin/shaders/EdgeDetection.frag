#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;

out vec4 FragColour;

vec4 FilterKernel(vec2 texCoord, mat3 kernel, float scale)
{
	vec2 texel = 1.0f / textureSize(colourTarget,0);
	vec4 colour = texture(colourTarget, texCoord + texel * vec2(-1,1)) * kernel[0][0];
	colour += texture(colourTarget, texCoord + texel * vec2(-1,0)) * kernel[0][1];
	colour += texture(colourTarget, texCoord + texel * vec2(-1,-1)) * kernel[0][2];
	colour += texture(colourTarget, texCoord + texel * vec2(0,1)) * kernel[1][0];
	colour += texture(colourTarget, texCoord) * kernel[1][1];
	colour += texture(colourTarget, texCoord + texel * vec2(0,-1)) * kernel[1][2];
	colour += texture(colourTarget, texCoord + texel * vec2(1,1)) * kernel[2][0];
	colour += texture(colourTarget, texCoord + texel * vec2(1,0)) * kernel[2][1];
	colour += texture(colourTarget, texCoord + texel * vec2(1,-1)) * kernel[2][2];
	
	return colour * scale;
}

void main(){
	// calculate texel size
	vec2 texSize = textureSize(colourTarget, 0);
	vec2 texelSize = 1.0f / texSize;
	
	// Adjust texture coordinate to avoid missampling edges
	vec2 scale = (texSize - texelSize) / texSize;
	vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;
	
	mat3 kernel = mat3( 0, -1, 0,
						-1, 5, -1,
						0, -1, 0);
	return FilterKernel(texCoord, kernel, 1);
}
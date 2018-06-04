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
	
	// Detect edges
	mat3 xKernel = mat3(-1, 0, 1,
						-2, 0, 2,
						-1, 0, 1);
	mat3 yKernel = mat3(-1, -2, -1,
						0, 0, 0,
						1, 2, 1);
	vec4 x = FilterKernel(texCoord, xKernel, 1);
	vec4 y = FilterKernel(texCoord, yKernel, 1);
	float value = sqrt(dot(x,x) + dot(y,y));
	
	FragColour = vec4(value, value, value, 1);
}
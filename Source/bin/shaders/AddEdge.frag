#version 410

in vec2 vTexCoord;

uniform sampler2D colourTarget;
uniform sampler2D edgeTarget;
uniform vec4 edgeColour;

out vec4 FragColour;

// TODO choose edge colour?

void main(){
	// calculate texel size
	vec2 texSize = textureSize(colourTarget, 0);
	vec2 texelSize = 1.0f / texSize;
	
	// Adjust texture coordinate to avoid missampling edges
	vec2 scale = (texSize - texelSize) / texSize;
	vec2 texCoord = vTexCoord / scale + texelSize * 0.5f;

	// edge to black
	//vec4 edge = vec4(vec3(1,1,1) - texture(edgeTarget, texCoord).rgb, 1);
	vec4 edge = vec4(edgeColour.rgb,texture(edgeTarget,texCoord).g * edgeColour.a);
	
	//FragColour = texture(colourTarget, texCoord) * edge;
	FragColour = vec4(mix(texture(colourTarget, texCoord).rgb, edge.rgb,edge.a),1);
}
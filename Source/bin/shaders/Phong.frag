#version 410

#define MAX_DIR_LIGHTS 32
#define MAX_POINT_LIGHTS 32
#define MAX_SPOT_LIGHTS 32

struct DirectionalLight{
	vec4 direction;
	vec4 diffuse;
	vec4 specular;
};

struct PointLight{
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	float linAtten;
	float quadAtten;
	float pad1;
	float pad2;
};

struct SpotLight{
	vec4 position;
	vec4 direction;
	vec4 diffuse;
	vec4 specular;
	float phi;
	float theta;
	float linAtten;
	float quadAtten;
};

struct Intensities{
	vec3 diffuse;
	vec3 specular;
};

in vec4 vPosition;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;
in vec2 vTexCoord;

uniform vec3 CameraPosition;

uniform vec3 Ka;	// ambient material
uniform vec3 Kd; 	// diffuse material
uniform vec3 Ks;	// specular material
uniform float specularPower;

uniform vec3 Ia; // ambient colour

layout (std140) uniform DirLights{
	DirectionalLight arrDirLights[MAX_DIR_LIGHTS];
	int numDirLights;
};

layout (std140) uniform PointLights{
	PointLight arrPointLights[MAX_POINT_LIGHTS];
	int numPointLights;
};

layout (std140) uniform SpotLights{
	SpotLight arrSpotLights[MAX_SPOT_LIGHTS];
	int numSpotLights;
};

out vec4 FragColour;

Intensities calculateDirLight(DirectionalLight light, vec3 N, vec3 V);
Intensities calculatePointLight(PointLight light, vec3 N, vec3 V);
Intensities calculateSpotLight(SpotLight light, vec3 N, vec3 V);

void main(){
	// Ensure normal and light directions are normalised
	vec3 N = normalize(vNormal);
	
	// Calculate view and reflection
	vec3 V = normalize(CameraPosition - vPosition.xyz);
	
	// Calculate intensity for all lights
	Intensities totalIntensities;
	totalIntensities.diffuse = vec3(0);
	totalIntensities.specular = vec3(0);
	
	for(int i = 0; i < numDirLights; i++){
		Intensities currIntensity = calculateDirLight(arrDirLights[i], N, V);
		totalIntensities.diffuse += currIntensity.diffuse;
		totalIntensities.specular += currIntensity.specular;
	}

	for(int i = 0; i < numPointLights; i++){
		Intensities currIntensity = calculatePointLight(arrPointLights[i], N, V);
		totalIntensities.diffuse += currIntensity.diffuse;
		totalIntensities.specular += currIntensity.specular;
	}
	
	for(int i = 0; i < numSpotLights; i++){
		Intensities currIntensity = calculateSpotLight(arrSpotLights[i], N, V);
		totalIntensities.diffuse += currIntensity.diffuse;
		totalIntensities.specular += currIntensity.specular;
	}
	
	// calculate colour properties
	vec3 ambient = Ia * Ka;
	vec3 diffuse = totalIntensities.diffuse * Kd;
	vec3 specular = totalIntensities.specular * Ks;

	FragColour = vec4(ambient + diffuse + specular, 1);
}

Intensities calculateDirLight(DirectionalLight light, vec3 N, vec3 V){
	vec3 L = normalize(light.direction.xyz);
	vec3 R = reflect(L, N);

	// calculate lambert term
	float lambertTerm = clamp(dot(N, -L), 0.0f, 1.0f);
	
	// calculate specular terms
	float specularTerm = pow( max( 0, dot(R, V)), specularPower);

	Intensities intensities;

	intensities.diffuse = light.diffuse.xyz * lambertTerm;
	intensities.specular = light.specular.xyz * specularTerm;

	return intensities;
}

Intensities calculatePointLight(PointLight light, vec3 N, vec3 V){
	vec3 displacement = vPosition.xyz - light.position.xyz;
	vec3 direction = normalize(displacement);
	float distance = length(displacement);

	vec3 R = reflect(direction, N);

	// TODO figure out some branchless max range thing
	float attenuation = 1.0 / (1.0 + light.linAtten * distance + light.quadAtten * distance * distance);

	// calculate lambert term
	float lambertTerm = clamp(dot(N, -direction), 0.0f, 1.0f);
	
	// calculate specular terms
	float specularTerm = pow( max( 0, dot(R, V)), specularPower);

	Intensities intensities;
	intensities.diffuse = light.diffuse.xyz * attenuation * lambertTerm;
	intensities.specular = light.specular.xyz * attenuation * specularTerm;

	
	return intensities;
}

Intensities calculateSpotLight(SpotLight light, vec3 N, vec3 V){
	vec3 displacement = vPosition.xyz - light.position.xyz;
	vec3 direction = normalize(displacement);
	vec3 lightDirection = normalize(light.direction.xyz);
	float distance = length(displacement);

	vec3 R = reflect(direction, N);
	
	// calculate lambert term
	float lambertTerm = clamp(dot(N, -direction), 0.0f, 1.0f);
	
	// calculate specular terms
	float specularTerm = pow( max( 0, dot(R, V)), specularPower);
	
	// Calculate attenuation
	float distanceAttenuation = 1.0 / (1.0 + light.linAtten * distance + light.quadAtten * distance * distance);
	
	float cosPhi = cos(light.phi);
	float cosTheta = cos(light.phi + light.theta);
	float cosAngle = dot(direction, lightDirection);
	float angleAttenuation = clamp((cosAngle - cosTheta) / (cosPhi - cosTheta), 0.0f, 1.0f);
	
	float attenuation = distanceAttenuation * angleAttenuation;
	
	Intensities intensities;
	intensities.diffuse = light.diffuse.xyz * attenuation * lambertTerm;
	intensities.specular = light.specular.xyz * attenuation * specularTerm;
	
	// TODO test this works correctly
	return intensities;
}
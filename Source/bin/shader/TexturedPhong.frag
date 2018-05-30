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
	float pad[2];
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

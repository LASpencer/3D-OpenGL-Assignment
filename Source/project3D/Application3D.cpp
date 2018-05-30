#include "Application3D.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include "imgui.h"

#include "Light.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;
using namespace las;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	m_scene = new Scene();

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_camera = new FlyCamera(vec3(10), vec3(0));
	m_camera->setPerspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);

	m_texturedPhong.loadShader(aie::eShaderStage::VERTEX, "./shaders/texturedPhong.vert");
	m_texturedPhong.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/texturedPhong.frag");
	
	if (m_texturedPhong.link() == false) {
		printf("Textured Phong shader error: %s\n", m_texturedPhong.getLastError());
		return false;
	}

	// Load obj files
	if (m_spearMesh.load("./models/soulspear.obj", true, true) == false) {
		printf("Could not load Spear mesh\n");
	}

	m_scene->addInstance(new Instance(vec3(0), vec3(0), vec3(1), &m_texturedPhong, &m_spearMesh));

	m_scene->addInstance(new Instance(vec3(2,0,2), vec3(1), vec3(1), &m_texturedPhong, &m_spearMesh));

	m_scene->addInstance(new Instance(vec3(-2, 0, 2), vec3(0), vec3(2,0.5,3), &m_texturedPhong, &m_spearMesh));

	m_scene->addInstance(new Instance(vec3(-2, 0, -2), vec3(1), vec3(2, 0.5, 3), &m_texturedPhong, &m_spearMesh));

	m_scene->addInstance(new Instance(vec3(3, 0, -3), vec3(0), vec3(1), &m_texturedPhong, &m_spearMesh));

	// Set up lighting
	m_scene->setAmbient(vec3(0.05));
	m_directionalLight = new DirectionalLight(vec3(-1, -1, 0), vec3(0, 0, 1), vec3(0, 0, 1));
	m_spotLight = new SpotLight(vec4(2, 0, -2, 1), vec3(-1, 1, 1), vec3(3, 0, 0), vec3(3, 0, 0), 0.3, 0.1, 0.03, 0.003);
	m_pointLight = new PointLight(vec4(1, 2, 1, 1), vec3(0, 1, 0), vec3(0, 1, 0), 0.03, 0.003);
	m_scene->addDirectionalLight(m_directionalLight);
	m_scene->addSpotLight(m_spotLight);
	m_scene->addPointLight(m_pointLight);

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
	delete m_camera;
	delete m_scene;
}

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// update camera
	m_camera->update(deltaTime);

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	m_scene->update(deltaTime);

	// ImGui display
	ImGui::Begin("Details");
	ImGui::Text("FPS: %i", getFPS());
	ImGui::End();

	ImGui::Begin("Lights");
	//TODO stuff to control lights
	//TODO also have light intensity?
	ImGui::Text("Directional Light");
	//HACK
	ImGui::ColorEdit3("Diffuse##directional", &m_directionalLight->diffuse.r);
	ImGui::ColorEdit3("Specular##directoinal", &m_directionalLight->specular.r);
	//TODO direction
	//TODO maybe button/checkbox to force same colour?
	ImGui::Text("Point Light");
	ImGui::ColorEdit3("Diffuse##point", &m_pointLight->diffuse.r);
	ImGui::ColorEdit3("Specular##point", &m_pointLight->specular.r);
	ImGui::DragFloat3("Position##point", &m_pointLight->position.x);
	ImGui::InputFloat("Linear Attenuation##point", &m_pointLight->linearAttenuation);
	ImGui::InputFloat("Quadratic Attenuation##point", &m_pointLight->quadraticAttenuation);
	ImGui::Text("Spot Light");
	ImGui::ColorEdit3("Diffuse##spot", &m_spotLight->diffuse.r);
	ImGui::ColorEdit3("Specular##spot", &m_spotLight->specular.r);
	ImGui::DragFloat3("Position##spot", &m_spotLight->position.x);
	//TODO directoin
	ImGui::InputFloat("Linear Attenuation##spot", &m_spotLight->linearAttenuation);
	ImGui::InputFloat("Quadratic Attenuation##spot", &m_spotLight->quadraticAttenuation);
	//TODO
	ImGui::End();

	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_camera->setAspectRatio(getWindowWidth() / (float)getWindowHeight());

	m_scene->draw(m_camera);

	// draw 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());
}
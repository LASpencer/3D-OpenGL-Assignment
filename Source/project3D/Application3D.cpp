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

	m_postProcessor = new PostProcessor();

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

	if (!m_postProcessor->initialise(getWindowWidth(), getWindowHeight())) {
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
	m_spotLight = new SpotLight(vec4(2, 0, -2, 1), vec3(-1, 1, 1), vec3(3, 0, 0), vec3(3, 0, 0), 0.43, 0.04, 0.03, 0.003);
	m_pointLight = new PointLight(vec4(1, 2, 1, 1), vec3(0, 1, 0), vec3(0, 1, 0), 0.03, 0.003);
	m_scene->addDirectionalLight(m_directionalLight);
	m_scene->addSpotLight(m_spotLight);
	m_scene->addPointLight(m_pointLight);

	directionalPitch = 0.22f;
	directionalYaw = 0.5f;
	spotPitch = 0.66f;
	spotYaw = 5.57f;

	directionalColour = { vec3(0,0,1), 1 };
	spotColour = { vec3{1,0,0}, 3 };
	pointColour = { vec3(0,1,0), 1 };

	return true;
}

void Application3D::shutdown() {

	Gizmos::destroy();
	delete m_camera;
	delete m_postProcessor;
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
	// store colour and intensity for each light, write to light struct afterwards
	ImGui::Text("Directional Light");
	//HACK
	ImGui::ColorEdit3("Colour##directional", &directionalColour.colour.r);
	ImGui::DragFloat("Intensity##directional", &directionalColour.intensity,0.01f,0.0f,10.f);
	//TODO direction
	ImGui::DragFloat("Yaw##directional", &directionalYaw, 0.01f, 0.0f, 6.28f, "%.3f rad");
	ImGui::DragFloat("Pitch##directional", &directionalPitch, 0.01f, -3.14f, 3.14f, "%.3f rad");
	//TODO maybe button/checkbox to force same colour?
	ImGui::Text("Point Light");
	ImGui::ColorEdit3("Colour##point", &pointColour.colour.r);
	ImGui::DragFloat("Intensity##point", &pointColour.intensity, 0.01f, 0.0f, 10.f);
	ImGui::DragFloat3("Position##point", &m_pointLight->position.x, 0.1f);
	ImGui::InputFloat("Linear Attenuation##point", &m_pointLight->linearAttenuation);
	ImGui::InputFloat("Quadratic Attenuation##point", &m_pointLight->quadraticAttenuation);
	ImGui::Text("Spot Light");
	ImGui::ColorEdit3("Colour##spot", &spotColour.colour.r);
	ImGui::DragFloat("Intensity##spot", &spotColour.intensity, 0.01f, 0.0f, 10.f);
	ImGui::DragFloat3("Position##spot", &m_spotLight->position.x, 0.1f);
	//TODO direction
	ImGui::DragFloat("Yaw##spot", &spotYaw, 0.01f, 0.0f, 6.28f, "%.3f rad");
	ImGui::DragFloat("Pitch##spot", &spotPitch, 0.01f, -3.14f, 3.14f, "%.3f rad");

	ImGui::InputFloat("Linear Attenuation##spot", &m_spotLight->linearAttenuation);
	ImGui::InputFloat("Quadratic Attenuation##spot", &m_spotLight->quadraticAttenuation);
	ImGui::DragFloat("Phi##spot", &m_spotLight->phi, 0.01f, 0.01f, 3.14f, "%.3f rad");
	ImGui::DragFloat("Theta##spot", &m_spotLight->theta, 0.01f, 0.01f, 3.14f, "%.3f rad");
	//TODO
	ImGui::End();

	m_directionalLight->direction = vec3(sin(directionalYaw) * cos(directionalPitch), sin(directionalPitch), cos(directionalYaw) * cos(directionalPitch));
	m_spotLight->direction = vec3(sin(spotYaw) * cos(spotPitch), sin(spotPitch), cos(spotYaw) * cos(spotPitch));

	m_directionalLight->setColour(directionalColour);
	m_pointLight->setColour(pointColour);
	m_spotLight->setColour(spotColour);

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
	m_postProcessor->setAspectRatio(getWindowWidth(), getWindowHeight());

	m_postProcessor->bind(this);

	m_scene->draw(m_camera);

	// draw 3D gizmos
	Gizmos::draw(m_camera->getProjectionView());

	// draw 2D gizmos using an orthogonal projection matrix (or screen dimensions)
	Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());

	m_postProcessor->unbind();

	m_postProcessor->draw(this);
}
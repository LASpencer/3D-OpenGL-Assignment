#include "PostProcessor.h"

las::PostProcessor::PostProcessor()
{

}

las::PostProcessor::~PostProcessor()
{
}

bool las::PostProcessor::initialise(unsigned int width, unsigned int height)
{
	if (m_rawRender.initialise(1, width, height) == false) {
		printf("Raw Render Target Error\n");
		return false;
	}
	if (m_edge.initialise(1, width, height) == false) {
		printf("Edge Render Target Error\n");
		return false;
	}

	// TODO initialise additional renderTargets

	m_edgeShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/post.vert");
	m_edgeShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/EdgeDetection.frag");

	if (m_edgeShader.link() == false) {
		printf("Edge Detection Shader Error: %s\n", m_edgeShader.getLastError());
		return false;
	}
}

void las::PostProcessor::bind(aie::Application* app)
{
	m_rawRender.bind();
	app->clearScreen();
}

void las::PostProcessor::unbind()
{
	m_rawRender.unbind();
}

void las::PostProcessor::draw(aie::Application* app)
{
	// TODO bind next rendertarget and shader
	// TODO render that stage
	// TODO eventually draw m_screen
	app->clearScreen();
	m_edgeShader.bind();
	m_edge.bind();

	m_rawRender.getTarget(0).bind(0);
	m_edgeShader.bindUniform("colourTarget", 0);
	m_screen.draw();

	//HACK
	m_edge.unbind();
	//app->clearScreen();
	//m_screen.draw();

}

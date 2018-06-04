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
	if (m_waveRender.initialise(1, width, height) == false) {
		printf("Wave Render Target Error\n");
		return false;
	}

	// TODO initialise additional renderTargets

	m_edgeShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/post.vert");
	m_edgeShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/EdgeDetection.frag");

	m_waveShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/post.vert");
	m_waveShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/Wave.frag");

	m_addEdgeShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/post.vert");
	m_addEdgeShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/AddEdge.frag");

	m_addEdgeShader.setVerbose(true);

	if (m_edgeShader.link() == false) {
		printf("Edge Detection Shader Error: %s\n", m_edgeShader.getLastError());
		return false;
	}

	if (m_waveShader.link() == false) {
		printf("Wave Detection Shader Error: %s\n", m_waveShader.getLastError());
		return false;
	}

	if (m_addEdgeShader.link() == false) {
		printf("Add Edge Shader Error: %s\n", m_addEdgeShader.getLastError());
		return false;
	}

	return true;
}

void las::PostProcessor::setAspectRatio(unsigned int width, unsigned int height)
{
	m_rawRender.setAspectRatio(width, height);
	m_edge.setAspectRatio(width, height);
	m_waveRender.setAspectRatio(width, height);
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
	m_edgeShader.bind();
	m_edge.bind();
	app->clearScreen();

	m_rawRender.getTarget(0).bind(0);
	m_edgeShader.bindUniform("colourTarget", 0);
	m_screen.draw();

	//HACK
	m_edge.unbind();

	//TODO instead write to wave buffer, then combine (edge, with wavy haze behind)
	m_waveShader.bind();
	m_waveRender.bind();
	app->clearScreen();

	m_waveShader.bindUniform("colourTarget", 0);
	m_waveShader.bindUniform("amplitude", 0.005f);
	m_waveShader.bindUniform("numWaves", 5.0f);
	m_waveShader.bindUniform("direction", glm::vec2(1, 0));
	m_waveShader.bindUniform("offset", app->getTime() * 1.0f);

	m_screen.draw();

	m_waveRender.unbind();

	//app->clearScreen();

	m_addEdgeShader.bind();

	m_addEdgeShader.bindUniform("colourTarget", 0);
	m_addEdgeShader.bindUniform("edgeTarget", 1);
	m_addEdgeShader.bindUniform("edgeColour", glm::vec4(1, 1, 0, 0.5f));

	m_waveRender.getTarget(0).bind(0);
	m_edge.getTarget(0).bind(1);

	m_screen.draw();
}

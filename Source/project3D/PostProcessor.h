#pragma once
#include "RenderTarget.h"
#include "ScreenQuad.h"
#include "Shader.h"
#include "Application.h"

namespace las {
	class PostProcessor {
	public:
		PostProcessor();

		virtual ~PostProcessor();

		bool initialise(unsigned int width, unsigned int height);

		virtual void setAspectRatio(unsigned int width, unsigned int height);

		void bind(aie::Application* app);

		void unbind();

		virtual void draw(aie::Application* app);

		//HACK
		glm::vec4		outlineColour = { 0,1,1,0.5f };
		float			waveAmplitude = 0.005f;
		float			numWaves = 5.f;
		glm::vec2		waveDirection = { 1,0 };
		float			waveSpeed = 1.0f;

	protected:
		aie::RenderTarget m_rawRender;
		aie::RenderTarget m_edge;
		aie::RenderTarget m_blurRender;
		aie::RenderTarget m_waveRender;

		aie::ShaderProgram m_edgeShader;
		aie::ShaderProgram m_waveShader;
		aie::ShaderProgram m_blurShader;
		aie::ShaderProgram m_addEdgeShader;

		ScreenQuad m_screen;
	};
}
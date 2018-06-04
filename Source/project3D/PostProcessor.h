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
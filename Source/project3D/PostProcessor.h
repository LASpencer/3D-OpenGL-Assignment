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

		// TODO setAspectRatio (try without first!)

		void bind(aie::Application* app);

		void unbind();

		void draw(aie::Application* app);

	protected:
		aie::RenderTarget m_rawRender;
		aie::RenderTarget m_edge;

		aie::ShaderProgram m_edgeShader;

		ScreenQuad m_screen;
	};
}
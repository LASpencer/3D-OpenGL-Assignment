#pragma once
#include <glm/glm.hpp>

namespace las {
	class ScreenQuad {
	public:

		ScreenQuad();
		~ScreenQuad();

		void draw();

	protected:
		unsigned int vao, vbo;
	};
}
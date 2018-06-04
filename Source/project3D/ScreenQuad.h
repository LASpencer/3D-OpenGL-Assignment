#pragma once

namespace las {
	class ScreenQuad {
	public:
		ScreenQuad();

		~ScreenQuad();

		void draw();

	private:
		unsigned int vao, vbo;
	};
}
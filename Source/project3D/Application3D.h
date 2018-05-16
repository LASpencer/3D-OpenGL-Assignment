#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>

#include "Camera.h"

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	las::Camera* m_camera;
};
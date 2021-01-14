#pragma once

#include <Core/Engine.h>
#include <Core/GPU/Mesh.h>

#include "Utils.h"

class Fuel
{
	public:
		Fuel();
		~Fuel();

		static Mesh* CreateFuel(glm::vec3 color);

		glm::vec3 color;
		glm::mat4 matrix;
		Mesh* mesh;
		Shader* shader;

		float scaleX;
};
#pragma once

#include <Core/Engine.h>
#include <Core/GPU/Mesh.h>

class Player
{
	public:
		Player();
		~Player();

		static void Init();

		glm::mat4 matrix;

		static Mesh* mesh;
		static Shader* shader;

		float radius;

		float transPlayerX;
		float transPlayerY;
		float transPlayerZ;

};
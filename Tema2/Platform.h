#pragma once

#include <Core/Engine.h>
#include <Core/GPU/Mesh.h>

class Platform
{
public:
	Platform();
	~Platform();

	static Mesh* CreatePlatform(glm::vec3 color);

	glm::vec3 color;
	glm::mat4 matrix;
	Mesh* mesh;

	int idxCol;

	float transX, transZ;
	float scaleZ;
	float minX, maxX;
	float height;
	float minZ, maxZ;
};
#include <iostream>
#include <Core/Engine.h>

#include "Platform.h"
#include "Utils.h"

Platform::Platform()
{
	height = 0.7f;
	idxCol = -1;
}

Platform::~Platform()
{
}

Mesh* Platform::CreatePlatform(glm::vec3 color)
{	

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(-1, -1,  1), color),
		VertexFormat(glm::vec3(1, -1,  1), color),
		VertexFormat(glm::vec3(-1,  1,  1), color),
		VertexFormat(glm::vec3(1,  1,  1), color),
		VertexFormat(glm::vec3(-1, -1, -1), color),
		VertexFormat(glm::vec3(1, -1, -1), color),
		VertexFormat(glm::vec3(-1,  1, -1), color),
		VertexFormat(glm::vec3(1,  1, -1), color),
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 2,		1, 3, 2,
		2, 3, 7,		2, 7, 6,
		1, 7, 3,		1, 5, 7,
		6, 7, 4,		7, 5, 4,
		0, 4, 1,		1, 4, 5,
		2, 6, 4,		0, 2, 4,
	};

	Mesh* platform = Utils::CreateMesh("cube", vertices, indices);

	return platform;
}



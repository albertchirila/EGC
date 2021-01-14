/* NUME : CHIRILA ALBERT
   GRUPA: 334CB
 */

#include "Fuel.h"
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Fuel::Fuel()
{
	scaleX = 1;
}

Fuel::~Fuel()
{
}


Mesh* Fuel::CreateFuel(glm::vec3 color)
{
	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0,  0), color),
		VertexFormat(glm::vec3(0.7f, 0,  0), color),
		VertexFormat(glm::vec3(0, 0.1f,  0), color),

		VertexFormat(glm::vec3(0.7f, 0,  0), color),
		VertexFormat(glm::vec3(0.7f, 0.1f,  0), color),
		VertexFormat(glm::vec3(0, 0.1f,  0), color)
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 2,
		3, 4, 5
	};

	Mesh* mesh = Utils::CreateMesh("fuelBar", vertices, indices);
	return mesh;
}
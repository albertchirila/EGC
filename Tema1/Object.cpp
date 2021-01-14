/* NUME : CHIRILA ALBERT
   GRUPA: 334CB
 */

#include "Object.h"
#include <iostream>

#include <Core/Engine.h>

using namespace std;


// functie creare shuriken
Mesh* Object::CreateShuriken(std::string name, float length, glm::vec3 color)
{

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0, 0,  0), color),
		VertexFormat(glm::vec3(length, 0,  0), color),
		VertexFormat(glm::vec3(length, length,  0), color),
	
		VertexFormat(glm::vec3(0, 0,  0), color),
		VertexFormat(glm::vec3(0, length,  0), color),
		VertexFormat(glm::vec3(-length, length,  0), color),

		VertexFormat(glm::vec3(0, 0,  0), color),
		VertexFormat(glm::vec3(-length, 0,  0), color),
		VertexFormat(glm::vec3(-length, -length,  0), color),

		VertexFormat(glm::vec3(0, 0,  0), color),
		VertexFormat(glm::vec3(0, -length,  0), color),
		VertexFormat(glm::vec3(length, -length,  0), color),
	};

	std::vector<unsigned short> indices =
	{
		0, 1, 2,
		3, 4, 5,
		6, 7, 8,
		9, 10, 11
	};


	Mesh* shuriken = new Mesh(name);
	shuriken->InitFromData(vertices, indices);
	return shuriken;
}

// functii creare balon: elipsa  + polilinie
Mesh* Object::CreateBalloon(std::string name, float numTriangles, float radiusX, float radiusY, glm::vec3 color)
{
	Mesh* balloon = new Mesh(name);

	float arg;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices.emplace_back(glm::vec3(0, 0, 0), color);

	for (int i = 0; i < numTriangles; i++)
	{
		arg = 2.0f * (float)M_PI * i / numTriangles;

		vertices.emplace_back(glm::vec3(cos(arg) * radiusX, sin(arg) * radiusY, 0), color);
		indices.push_back(i);
	}


	indices.push_back(numTriangles);
	indices.push_back(1);

	balloon->InitFromData(vertices, indices);
	balloon->SetDrawMode(GL_TRIANGLE_FAN);
	return balloon;
}

Mesh* Object::CreatePolyline(std::string name, float radiusY, glm::vec3 color)
{
	Mesh* polyline = new Mesh(name);

	float arg;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices.emplace_back(glm::vec3(0, -radiusY, 0), color);
	vertices.emplace_back(glm::vec3(-10, -(radiusY + 10), 0), color);
	vertices.emplace_back(glm::vec3(10, -(radiusY + 20), 0), color);
	vertices.emplace_back(glm::vec3(-10, -(radiusY + 30), 0), color);

	indices = { 0,1,2,3 };

	polyline->InitFromData(vertices, indices);
	polyline->SetDrawMode(GL_LINE_STRIP);
	return polyline;
}

//functie creare arc
Mesh* Object::CreateBow(std::string name, float numTriangles, float radiusX, float radiusY, glm::vec3 color)
{
	Mesh * bow = new Mesh(name);

	float arg;
	int idx = 3;

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices.emplace_back(glm::vec3(0, radiusY, 0), color);
	vertices.emplace_back(glm::vec3(0, 0, 0), color);
	vertices.emplace_back(glm::vec3(0, -radiusY, 0), color);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	for (int i = 15; i < numTriangles; i++)
	{
		arg = 2.0f * (float)M_PI * i / numTriangles;

		vertices.emplace_back(glm::vec3(cos(arg) * radiusX, sin(arg) * radiusY, 0), color);
		indices.push_back(idx++);
	}

	for (int i = 0; i <= numTriangles / 4; i++)
	{
		arg = 2.0f * (float)M_PI * i / numTriangles;

		vertices.emplace_back(glm::vec3(cos(arg) * radiusX, sin(arg) * radiusY, 0), color);
		indices.push_back(idx++);
	}
	indices.push_back(idx);

	bow->InitFromData(vertices, indices);
	bow->SetDrawMode(GL_LINE_STRIP);
	return bow;

}


//functii creare sageata: dreptunghi + triunghi(varful sagetii)
Mesh* Object::CreateArrow(std::string name, float len_arrow, glm::vec3 color)
{
	Mesh* arrow = new Mesh(name);

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	// dreptunghi -> 2 triunghiuri
	vertices.emplace_back(glm::vec3(0, -(len_arrow - 19), 0), color);
	vertices.emplace_back(glm::vec3(3 * len_arrow, -(len_arrow -19), 0), color);
	vertices.emplace_back(glm::vec3(3 * len_arrow, len_arrow - 19, 0), color);

	vertices.emplace_back(glm::vec3(0, len_arrow - 19, 0), color);
	vertices.emplace_back(glm::vec3(3 * len_arrow, len_arrow - 19, 0), color);
	vertices.emplace_back(glm::vec3(0, -(len_arrow - 19), 0), color);

	//varful sagetii -> triunghi
	vertices.emplace_back(glm::vec3(3 * len_arrow, -(len_arrow - 12), 0), color);
	vertices.emplace_back(glm::vec3(4 * len_arrow, 0, 0), color);
	vertices.emplace_back(glm::vec3(3 * len_arrow, len_arrow - 12, 0), color);

	indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8};

	arrow->InitFromData(vertices, indices);
	arrow->SetDrawMode(GL_TRIANGLES);
	return arrow;

}

// functie creare power-bar: dreptunghi
Mesh* Object::CreatePowerBar(std::string name, float length, glm::vec3 color)
{

	Mesh* powerBar = new Mesh("powerBar");

	std::vector<VertexFormat> vertices;
	std::vector<unsigned short> indices;

	vertices.emplace_back(VertexFormat(glm::vec3(0, 0, 0), color));
	vertices.emplace_back(VertexFormat(glm::vec3(length, 0, 0), color));
	vertices.emplace_back(VertexFormat(glm::vec3(length, 0.5f * length, 0), color));
	vertices.emplace_back(VertexFormat(glm::vec3(0, 0.5f * length, 0), color));

	indices = { 0, 1, 2, 3 };

	powerBar->InitFromData(vertices, indices);
	powerBar->SetDrawMode(GL_TRIANGLE_FAN);
	return powerBar;
}
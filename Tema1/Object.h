/* NUME : CHIRILA ALBERT
   GRUPA: 334CB
 */

#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object
{
	Mesh* CreateShuriken(std::string name, float length, glm::vec3 color);
	Mesh *CreateBalloon(std::string name, float numTriangle, float radiusX, float radiusY, glm::vec3 color);
	Mesh* CreatePolyline(std::string name, float radiusY, glm::vec3 color);
	Mesh* CreateBow(std::string name, float numTriangles, float radiusX, float radiusY, glm::vec3 color);
	Mesh* CreateArrow(std::string name, float len_arrow, glm::vec3 color);
	Mesh* CreatePowerBar(std::string name, float length, glm::vec3 color);
}


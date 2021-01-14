#pragma once

#include <Core/Engine.h>
#include <Core/GPU/Mesh.h>

class Utils
{
	public:
		Utils() = default;
		~Utils() = default;

		static Mesh* CreateMesh(char* name, std::vector<VertexFormat>& vertices, std::vector<GLushort>& indices);
};
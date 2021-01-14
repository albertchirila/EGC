/* NUME : CHIRILA ALBERT
   GRUPA: 334CB
 */

#include "Player.h"
#include <iostream>

#include <Core/Engine.h>

using namespace std;

Mesh* Player::mesh = nullptr;
Shader* Player::shader = nullptr;

Player::Player()
{
	transPlayerX = transPlayerY = transPlayerZ = 0;
	radius = 0.4f;
}

Player::~Player()
{
}

void Player::Init()
{
		mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");

		shader = new Shader("sphereShader");
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexSphereShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragSphereShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
}
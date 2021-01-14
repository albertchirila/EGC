#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include "cmath"
#include <time.h>

#include <Core/Engine.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}

void Tema2::Init()
{
	srand(time(NULL));

	// initializare coloane cu platforme
	numColumns = 3;
	for (int i = 0; i < numColumns; i++)
		columns.emplace_back();

	// initializare campuri coloane
	for (int i = 0; i < numColumns; i++)
	{
		columns[i].numPlatforms = 5;
		for (int j = 0; j < columns[i].numPlatforms; j++)
			columns[i].platforms.push_back(new Platform());
	}

	// initializare platforme pentru fiecare coloana
	random = 1 + rand() % 5;
	InitPlatform(0, 0, GetColor(random), -6, -40, 20);
	for (int j = 1; j < columns[0].numPlatforms; j++)
	{
		random = 1 + rand() % 5;
		InitPlatform(0, j, GetColor(random), -6, columns[0].platforms[j - 1]->transZ - 45, 20);
	}

	InitPlatform(1, 0, glm::vec3(0, 0, 1), 0, -15, 15);
	for (int j = 1; j < columns[1].numPlatforms; j++)
	{
		random = 1 + rand() % 5;
		InitPlatform(1, j, GetColor(random), 0, columns[1].platforms[j - 1]->transZ - 35, 15);
	}

	random = 1 + rand() % 5;
	InitPlatform(2, 0, GetColor(random), 6, -50, 25);
	for (int j = 1; j < columns[2].numPlatforms; j++)
	{
		random = 1 + rand() % 5;
		InitPlatform(2, j, GetColor(random), 6, columns[2].platforms[j - 1]->transZ - 55, 25);
	}

	copyPlatform = new Platform();

	// initializari jucator + variabile jucator
	player = new Player();
	player->Init();

	copyTransZ = 0;
	speed = copySpeed = 8;
	maxSpeed = false;
	fall = false;
	jump = up = down = limitLeft = limitRight = false;


	// initializare combustibil - bara + combustibil
	fuelBar = new Fuel();
	fuel = new Fuel();
	InitFuel(fuelBar, glm::vec3(1, 1, 1));
	InitFuel(fuel, glm::vec3(1, 0, 0));
	hitOnce = false;

	// initializare camera
	camera = new Tema::Camera();
	camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 2, 0), glm::vec3(0, 2, 0));
	changeCamera = false;

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
}

void Tema2::FrameStart()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{	
	// jucatorul se misca continuu
	MovePlayer(deltaTimeSeconds);

	// se creaza jucatorul	
	player->matrix = glm::mat4(1);
	player->matrix = glm::translate(player->matrix, glm::vec3(0, 1, 0));
	player->matrix = glm::translate(player->matrix, glm::vec3(player->transPlayerX, player->transPlayerY, player->transPlayerZ));
	player->matrix = glm::scale(player->matrix, glm::vec3(0.8f));
	// se afiseaza jucatorul doar in third-person camera
	if (!changeCamera)
		RenderSimpleMesh(player->mesh, player->shader, player->matrix);

	// generare harta
	CreateMap();

	//creare combustibil
	CreateFuel();

	Platform *p = CheckCollision();

	if (p)
	{
		if (copyPlatform != p)
			hitOnce = true;
		else
			hitOnce = false;
		copyPlatform = p;

		// se verifica o singura data pe ce platforma a aterizat jucatorul
		if (hitOnce)
			CheckPlatform(p);

		// se schimba culoarea placii in mov
		ChangeColor(p, glm::vec3(1, 0, 1));
	}
	else
	{
		// daca se paraseste o platforma, culoarea ii va reveni
		if (jump)
			ChangeColor(copyPlatform, copyPlatform->color);
		// jucatorul nu se mai afla pe nici o placa si va cadea
		if (!jump)
			PlayerFall(deltaTimeSeconds);
	}

}

void Tema2::FrameEnd()
{
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

// functie ce initializeaza combustibilul
void Tema2::InitFuel(Fuel* f, glm::vec3 color)
{
	f->color = color;
	f->mesh = Fuel::CreateFuel(color);
	f->shader = new Shader("fuelShader");
	f->shader->AddShader("Source/Laboratoare/Tema2/Shaders/VertexFuel.glsl", GL_VERTEX_SHADER);
	f->shader->AddShader("Source/Laboratoare/Tema2/Shaders/FragmentFuel.glsl", GL_FRAGMENT_SHADER);
	f->shader->CreateAndLink();
}

// functie ce initializeaza platforma
void Tema2::InitPlatform(int col, int platform, glm::vec3 color, float transX, float transZ, float scaleZ)
{
	if (col == 0)
	{
		columns[0].platforms[platform]->minX = -8;
		columns[0].platforms[platform]->maxX = -4;
	}
	if (col == 1)
	{
		columns[1].platforms[platform]->minX = -2;
		columns[1].platforms[platform]->maxX = 2;
	}
	if (col == 2)
	{
		columns[2].platforms[platform]->minX = 4;
		columns[2].platforms[platform]->maxX = 8;
	}
	columns[col].platforms[platform]->idxCol = col;
	columns[col].platforms[platform]->color = color;
	columns[col].platforms[platform]->mesh = Platform::CreatePlatform(columns[col].platforms[platform]->color);
	columns[col].platforms[platform]->transX = 0;
	columns[col].platforms[platform]->transX = transX;
	columns[col].platforms[platform]->transZ = 0;
	columns[col].platforms[platform]->transZ = transZ;
	columns[col].platforms[platform]->scaleZ = scaleZ;
	columns[col].platforms[platform]->minZ = columns[col].platforms[platform]->transZ + scaleZ;
	columns[col].platforms[platform]->maxZ = columns[col].platforms[platform]->transZ - scaleZ;
}

// functie ce returneaza o culoare obtinuta aleator
glm::vec3 Tema2::GetColor(int random)
{
	glm::vec3 color;

	if (random == 1)
		color = glm::vec3(0, 0, 1);
	else if (random == 2)
		color = glm::vec3(1, 0, 0);
	else if (random == 3)
		color = glm::vec3(1, 1, 0);
	else if (random == 4)
		color = glm::vec3(1, 0.5f, 0);
	else if (random == 5)
		color = glm::vec3(0, 1, 0);

	return color;
}

// functie ce deseneaza bara de combustibil
void Tema2::RenderFuel(Mesh* mesh, Shader* shader, glm::mat4 & matrix, glm::vec3 & color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);
	
	int modelLocation = glGetUniformLocation(shader->GetProgramID(), "Model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(matrix));

	GLint locObject = glGetUniformLocation(shader->program, "v_color");
	glUniform3fv(locObject, 1, glm::value_ptr(color));

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// functie ce creaza platformele si jucatorul
void Tema2::RenderSimpleMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	shader->Use();
	glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}

void Tema2::RenderPlatform(int col, int platform)
{
	Platform* p = columns[col].platforms[platform];
	/* daca platforma a iesit din scena, se va desena dupa ultima
	   platforma de pe coloana corespunzatoare */
	if ((transpose(player->matrix)[2][3]) < p->maxZ - 2)
	{
		int random = 1 + rand() % 5;
		int nr = columns[col].numPlatforms;
		if (col == 0)
		{
			if (platform == 0)
				InitPlatform(0, platform, GetColor(random), -6, columns[0].platforms[nr - 1]->transZ - 45, 20);
			else
				InitPlatform(0, platform, GetColor(random), -6, columns[0].platforms[platform - 1]->transZ - 45, 20);
		}
		if (col == 1)
		{
			if (platform == 0)
				InitPlatform(1, platform, GetColor(random), 0, columns[1].platforms[nr - 1]->transZ - 35, 15);
			else
				InitPlatform(1, platform, GetColor(random), 0, columns[1].platforms[platform - 1]->transZ - 35, 15);
		}
		if (col == 2)
		{
			if (platform == 0)
				InitPlatform(2, platform, GetColor(random), 6, columns[2].platforms[nr - 1]->transZ - 55, 25);
			else
				InitPlatform(2, platform, GetColor(random), 6, columns[2].platforms[platform - 1]->transZ - 55, 25);
		}
	}
	p->matrix = glm::mat4(1);
	p->matrix = glm::translate(p->matrix, glm::vec3(p->transX, 0.5f, p->transZ));
	p->matrix = glm::scale(p->matrix, glm::vec3(2, 0.2f, p->scaleZ));
	/* platforma este desenata doar daca poate fi vizibila in scena */
	if ((transpose(player->matrix)[2][3]) >= p->maxZ - 2) //numere negative
		RenderSimpleMesh(p->mesh, shaders["VertexColor"], p->matrix);
}

// functie ce creaza harta
void Tema2::CreateMap()
{
	for (int i = 0; i < numColumns; i++)
	{
		for (int j = 0; j < columns[i].numPlatforms; j++)
			RenderPlatform(i, j);
	}
}

// functie ce creaza bara de combustibil
void Tema2::CreateFuel()
{
	// dreptunghiul din fata -> rosu
	fuel->matrix = glm::mat3(1);
	fuel->matrix = glm::translate(fuel->matrix, glm::vec3(-0.95f, 0.8f, 0));
	fuel->matrix = glm::scale(fuel->matrix, glm::vec3(fuel->scaleX, 1, 0));
	RenderFuel(fuel->mesh, fuel->shader, fuel->matrix, fuel->color);

	//dreptunghiul din spate -> alb
	fuelBar->matrix = glm::mat3(1);
	fuelBar->matrix = glm::translate(fuelBar->matrix, glm::vec3(-0.95f, 0.8f, 0));
	RenderFuel(fuelBar->mesh, fuelBar->shader, fuelBar->matrix, fuelBar->color);

}
// functie ce realizeaza miscarea jucatorului
void Tema2::MovePlayer(float deltaTimeSeconds)
{
	if (!fall)
	{
		player->transPlayerZ -= deltaTimeSeconds * speed;
		fuel->scaleX -= 0.0005f;
		if (fuel->scaleX < 0)
			EndGame("YOU RAN OUT OF FUEL!");
	}

	// daca jucatorul se deplaseaza cu viteza data de coliziunea cu placa portocalie
	if (maxSpeed)
	{
		if (player->transPlayerZ - copyTransZ < -100)
		{
			speed = copySpeed;
			maxSpeed = false;
		}
	}

	if (jump)
		Jump(deltaTimeSeconds);

	// se verifica limitele din stanga si din dreapta
	if (player->transPlayerX < -8)
		limitLeft = true;
	else
		limitLeft = false;

	if (player->transPlayerX > 8)
		limitRight = true;
	else
		limitRight = false;
}

//functie ce executa o saritura a jucatorului + miscarea camerei
void Tema2::Jump(float deltaTimeSeconds)
{
	if (up)
	{
		player->transPlayerY += 6 * deltaTimeSeconds;
		if (player->transPlayerY > 2.5)
		{
			up = false;
			down = true;
		}
		if (changeCamera && camera->position.y < 4.5f)
			camera->TranslateUpword(6 * deltaTimeSeconds);
	}
	if (down)
	{
		player->transPlayerY -= 5 * deltaTimeSeconds;
		if (player->transPlayerY < 0)
		{
			down = jump = false;
			player->transPlayerY = 0;
		}
		if (changeCamera && camera->position.y > 2.f)
			camera->TranslateUpword(-5 * deltaTimeSeconds);
	}
}

// functie ce realizeaza caderea jucatorului + miscare camera + terminarea jocului
void Tema2::PlayerFall(float deltaTimeSeconds)
{
	fall = true;
	player->transPlayerY -= deltaTimeSeconds * 5;
	camera->TranslateUpword(-deltaTimeSeconds * 5);
	if (player->transPlayerY < -8)
		EndGame("YOU FELL OFF!");
}

// functie ce verifica tipul de placa pe care jucatorul a aterizat
void Tema2::CheckPlatform(Platform* p)
{
	// platforma rosie -> se termina jocul
	if (p->color == glm::vec3(1, 0, 0))
		EndGame("YOU HIT A RED PLATFORM!");

	// platforma portocalie -> viteza foarte mare
	if (p->color == glm::vec3(1, 0.5f, 0))
	{
		copyTransZ = p->transZ;
		speed = 30;
		maxSpeed = true;
	}
	// platforma galbena -> jucatorul pierde combustibil
	if (p->color == glm::vec3(1, 1, 0))
	{
		if (fuel->scaleX - 0.03f < 0)
			EndGame("YOU RAN OUT OF FUEL!");
		else
			fuel->scaleX -= 0.03f;
	}
	// platforma verde -> jucatorul castiga combustibil
	if (p->color == glm::vec3(0, 1, 0))
	{
		if (fuel->scaleX + 0.1f >= 1)
			fuel->scaleX = 1;
		else
			fuel->scaleX += 0.1f;
	}
}

// functie ce schimba culoarea platformei dupa coliziune
void Tema2::ChangeColor(Platform *p, glm::vec3 color)
{
	p->mesh = Platform::CreatePlatform(color);
	p->matrix = glm::mat4(1);

	if (p->idxCol == 0)
		p->matrix = glm::translate(p->matrix, glm::vec3(-6, 0.5f, p->transZ));
	else if (p->idxCol == 1)
		p->matrix = glm::translate(p->matrix, glm::vec3(0, 0.5f, p->transZ));
	else if (p->idxCol == 2)
		p->matrix = glm::translate(p->matrix, glm::vec3(6, 0.5f, p->transZ));

	p->matrix = glm::scale(p->matrix, glm::vec3(2, 0.2f, p->scaleZ));
	RenderSimpleMesh(p->mesh, shaders["VertexColor"], p->matrix);
}

// functie ce detecteaza coliziunea cu platforma
Platform* Tema2::CheckCollision()
{
	glm::mat4 m = transpose(player->matrix);
	float tx = (transpose(player->matrix))[0][3];
	float ty = (transpose(player->matrix))[1][3];
	float tz = (transpose(player->matrix))[2][3];


	for (int i = 0; i < numColumns; i++)
	{
		for (int j = 0; j < columns[i].numPlatforms; j++)
		{
			Platform* p = columns[i].platforms[j];
			if (tx >= p->minX && tx <= p->maxX)
				if (tz <= p->minZ && tz >= p->maxZ)
					if (ty - player->radius <= p->height)
						return p;
		}
	}

	return nullptr;
}

void Tema2::EndGame(std::string message)
{
	cout << message << endl << "GAME OVER!" << endl;
	exit(0);
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	if (!fall)
		camera->TranslateForward(deltaTime * speed);
	
	if (window->KeyHold(GLFW_KEY_W) && !maxSpeed) {
		copySpeed = speed;
		if (speed <= 16)
			speed += 0.1;
	}

	if (window->KeyHold(GLFW_KEY_S) && !maxSpeed) {
		copySpeed = speed;
		if (speed >= 8)
			speed -= 0.1;
	}

	if (window->KeyHold(GLFW_KEY_A)) {
		// daca jucatorul nu a atins limita de stanga si nu a cazut de pe platforma
		if (!limitLeft && !fall)
		{
			player->transPlayerX -= deltaTime * speed;
			camera->TranslateRight(-deltaTime * speed);
		}
	}

	if (window->KeyHold(GLFW_KEY_D)) {
		// daca jucatorul nu a atins limita de dreapta si nu a cazut de pe platforma
		if (!limitRight && !fall)
		{
			player->transPlayerX += deltaTime * speed;
			camera->TranslateRight(deltaTime * speed);
		}
	}
}

void Tema2::OnKeyPress(int key, int mods)
{
	
	if (key == GLFW_KEY_SPACE && !fall)
	{
		jump = up = true;
		down = false;
	}

	if (key == GLFW_KEY_C && !jump)
	{
		changeCamera = !changeCamera;
		if (changeCamera)
			camera->MoveForward(3.5f);
		else
			camera->MoveForward(-3.5f);
	}
}
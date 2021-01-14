/* NUME : CHIRILA ALBERT
   GRUPA: 334CB
 */

#include "Tema1.h"
#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Tema1_2D.h"
#include "Object.h"
#include "cmath"

using namespace std;

Tema1::Tema1()
{
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	//Initializari joc

	/* -> numarul de baloane
	   -> numarul de shuriken */
	numBalloons = 7;
	numShuriken = 3;

	// initializarea vectorilor specifici baloanelor
	for (int i = 0; i < numBalloons; i++)
	{
		struct Balloon balloon;
		balloons.push_back(balloon);
		struct Polyline polyline;
		polylines.push_back(polyline);
		struct InitialBalloon initial;

		switch (i)
		{
		case 0:
			balloonColor.push_back("redBalloon");
			polylineColor.push_back("redPolyline");
			initial.posX = 570;
			initial.posY = -120;
			initialBalloon.push_back(initial);

		case 1:
			balloonColor.push_back("yellowBalloon");
			polylineColor.push_back("yellowPolyline");
			initial.posX = 640;
			initial.posY = -40;
			initialBalloon.push_back(initial);

		case 2:
			balloonColor.push_back("redBalloon");
			polylineColor.push_back("redPolyline");
			initial.posX = 730;
			initial.posY = -90;
			initialBalloon.push_back(initial);

		case 3:
			balloonColor.push_back("yellowBalloon");
			polylineColor.push_back("yellowPolyline");
			initial.posX = 860;
			initial.posY = -200;
			initialBalloon.push_back(initial);

		case 4:
			balloonColor.push_back("yellowBalloon");
			polylineColor.push_back("yellowPolyline");
			initial.posX = 930;
			initial.posY = -170;
			initialBalloon.push_back(initial);

		case 5:
			balloonColor.push_back("redBalloon");
			polylineColor.push_back("redPolyline");
			initial.posX = 1030;
			initial.posY = -130;
			initialBalloon.push_back(initial);

		case 6:
			balloonColor.push_back("yellowBalloon");
			polylineColor.push_back("yellowPolyline");
			initial.posX = 1140;
			initial.posY = -160;
			initialBalloon.push_back(initial);

		default:
			break;
		}
	}

	// initializarea vectorilor specifici shuriken-urilor
	for (int i = 0; i < numShuriken; i++)
	{	
		struct Shuriken shuriken;
		shurikens.push_back(shuriken);
		struct InitialShuriken initial;

		switch (i)
		{
		case 0:
			initial.posX = 1700;
			initial.posY = 660;
			initialShuriken.push_back(initial);

		case 1:
			initial.posX = 1800;
			initial.posY = 430;
			initialShuriken.push_back(initial);

		case 2:
			initial.posX = 1900;
			initial.posY = 120;
			initialShuriken.push_back(initial);

		default:
			break;
		}
	}

	copyIdx = -1;
	contor = 0;

	/* -> numarul de vieti 
	   -> scorul */
	lives = 3;
	score = 0;

	// Initializari variabile:
	// - translatie
	translatePlayerX = translatePlayerY = 0;
	translateArrowX = translateArrowY = 0;

	// - rotatie
	angle = 0;

	//scalare
	scaleBarX = scaleBarY = 1;

	// - miscarea mouse-ului
	mouseCrtX = mouseCrtY = 0;

	// - viteza minima a sagetii
	speedArrow = 400;

	// Initializari obiecte:
	// - shuriken
	float shurikenSide = 25;
	Mesh* shuriken = Object::CreateShuriken("shuriken", shurikenSide, glm::vec3(0.184314f, 0.309804f, 0.309804f));
	AddMeshToList(shuriken); 

	// - balon
	float numTriangles = 20;
	float radiusX = 20.0f;
	float radiusY = 30.0f;
	Mesh* redBalloon = Object::CreateBalloon("redBalloon", numTriangles, radiusX, radiusY, glm::vec3(1, 0, 0));
	AddMeshToList(redBalloon);
	Mesh* yellowBalloon = Object::CreateBalloon("yellowBalloon", numTriangles, radiusX, radiusY, glm::vec3(1, 1, 0));
	AddMeshToList(yellowBalloon);

	// - polilinie
	Mesh* redPolyline = Object::CreatePolyline("redPolyline", radiusY, glm::vec3(1, 0, 0));
	AddMeshToList(redPolyline);
	Mesh* yellowPolyline = Object::CreatePolyline("yellowPolyline", radiusY, glm::vec3(1, 1, 0));
	AddMeshToList(yellowPolyline);

	// - arc
	radiusX = 30.0f;
	radiusY = 45.0f;
	Mesh* bow = Object::CreateBow("bow", numTriangles, radiusX, radiusY, glm::vec3(1, 0, 0));
	AddMeshToList(bow);

	// - sageata
	float len_arrow = 20;
	Mesh *arrow = Object::CreateArrow("arrow", len_arrow, glm::vec3(1, 0, 0));
	AddMeshToList(arrow);

	// - powerBar
	float len_bar = 10;
	Mesh *powerBar = Object::CreatePowerBar("powerBar", len_bar, glm::vec3(1, 0, 0));
	AddMeshToList(powerBar);

}

void Tema1::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{	

	// daca sageata nu e aruncata, se calculeaza unghiul de rotatie
	if (!throwArrow)
	{
		float dx = mouseCrtX - 100;
		float dy = mouseCrtY - (transpose(bow))[1][2];
		angle = atan2(-dy, dx);
	}

	// Obiectele care formeaza jucatorul
	// arcul
	bow = glm::mat3(1);
	bow *= Transform2D::Translate(100, 360);
	bow *= Transform2D::Translate(translatePlayerX, translatePlayerY);
	bow *= Transform2D::Rotate(angle);
	RenderMesh2D(meshes["bow"], shaders["VertexColor"], bow);

	//sageata -> linie + triughi
	arrow = glm::mat3(1);
	arrow *= Transform2D::Translate(100, 360);
	MoveArrow(deltaTimeSeconds);
	arrow *= Transform2D::Translate(translateArrowX, translateArrowY);
	arrow *= Transform2D::Rotate(angle);
	RenderMesh2D(meshes["arrow"], shaders["VertexColor"], arrow);

	// varful sagetii - utilizat la coliziunea sageata-tinte
	point = glm::mat3(1);
	point *= Transform2D::Translate(180, 360);
	point *= Transform2D::Translate(translateArrowX, translateArrowY);
	point *= Transform2D::Rotate(angle);

	//Obiectele care reprezinta tintele
	// shuriken
	if (!createShurikens)
	{
		for (int i = 0; i < numShuriken; i++)
		{
			shurikens[i].shuriken *= Transform2D::Translate(initialShuriken[i].posX, initialShuriken[i].posY);
			RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], shurikens[i].shuriken);
		}
		createShurikens = true;
	}
	for (int i = 0; i < numShuriken; i++)
	{
		MoveShuriken(deltaTimeSeconds, i);
		RenderMesh2D(meshes["shuriken"], shaders["VertexColor"], shurikens[i].shuriken);
	}

	// balon -> elipsa + polilinie 
	if (!createBalloons)
	{
		for (int i = 0; i < numBalloons; i++)
		{
			balloons[i].balloon *= Transform2D::Translate(initialBalloon[i].posX, initialBalloon[i].posY);
			RenderMesh2D(meshes[balloonColor[i]], shaders["VertexColor"], balloons[i].balloon);
			polylines[i].polyline *= Transform2D::Translate(initialBalloon[i].posX, initialBalloon[i].posY);
			RenderMesh2D(meshes[polylineColor[i]], shaders["VertexColor"], polylines[i].polyline);
		}
		createBalloons = true;
	}
	for (int i = 0; i < numBalloons; i++)
	{
		MoveBalloon(deltaTimeSeconds, i);
		RenderMesh2D(meshes[balloonColor[i]], shaders["VertexColor"], balloons[i].balloon);
		RenderMesh2D(meshes[polylineColor[i]], shaders["VertexColor"], polylines[i].polyline);
	}

	// Celelalte obiecte
	// puterea de lansare a sagetii - dreptunghi
	powerBar = glm::mat3(1);
	powerBar *= Transform2D::Translate(100, 300);
	powerBar *= Transform2D::Translate(translatePlayerX, translatePlayerY);
	powerBar *= Transform2D::Scale(scaleBarX, scaleBarY);
	RenderMesh2D(meshes["powerBar"], shaders["VertexColor"], powerBar);

	// se verifica coliziunea sageata-balon
	int idx = CheckBalloonCollision();

	// se afiseaza scorul la coliziunea cu un balon
	if ((contor == 0 && idx != -1) || (copyIdx != idx && idx != -1))
	{
		if (balloonColor[idx] == "redBalloon")
		{
			score += 166;
			cout << "YOU HIT A RED BALLOON! SCORE: " << score << endl;
		}
		else
		{
			if (score >= 55)
				score -= 55;
			else
				score = 0;
			cout << "YOU HIT A YELLOW BALLOON! SCORE: " << score << endl;
		}
		contor = 1;
	}
	copyIdx = idx;

	// se verifica coliziunea sageata-shuriken
	CheckShurikenCollision();

	// se verifica coliziunea jucator(arc)-shuriken
	CheckPlayerCollision();
	if (hitPlayer)
		wasHit = true;
	// daca jucatorul a fost lovit si mai are vieti ramase
	if (!hitPlayer && wasHit)
	{
		lives -= 1;
		wasHit = false;
		if (lives != 0)
			cout << "YOU WERE HIT! YOU HAVE ONLY " << lives << " LIVES LEFT!" << endl;
	}

	// daca jucatorul a pierdut vietile, jocul de incheie
	if (lives == 0)
	{	
		cout << endl << "YOU HAVE NO MORE LIVES!" << endl;
		cout << "GAME OVER!" << endl;
		cout << "FINAL SCORE: " << endl;
		cout << score << endl;
		exit(0);
	}
}

void Tema1::FrameEnd()
{
	
}

// functie ce realizeaza deplasarea shuriken-ului pe axa OX
void Tema1::MoveShuriken(float deltaTimeSeconds, int idx)
{
	// daca obiectul este in miscare, se continua translatia acestuia pana la o limita setata
	if (shurikens[idx].isMoving)
	{
		shurikens[idx].translateX -= deltaTimeSeconds * 200;
		shurikens[idx].angularStep += 40 * deltaTimeSeconds * RADIANS(10);

		shurikens[idx].shuriken = glm::mat3(1);
		shurikens[idx].shuriken *= Transform2D::Translate(shurikens[idx].translateX, shurikens[idx].translateY);
		shurikens[idx].shuriken *= Transform2D::Translate(initialShuriken[idx].posX, initialShuriken[idx].posY);
		shurikens[idx].shuriken *= Transform2D::Rotate(shurikens[idx].angularStep);
		// a iesit din scena
		if (transpose(shurikens[idx].shuriken)[0][2] < -80)
			shurikens[idx].isMoving = false;
	}
	else
	{
		// a fost lovit
		if (hitShuriken)
			DestroyShuriken(deltaTimeSeconds, idx);
		// obiectul revine pe pozitia initiala
		else
		{
			shurikens[idx].translateX = 0;
			shurikens[idx].shuriken = glm::mat3(1);
			shurikens[idx].shuriken *= Transform2D::Translate(initialShuriken[idx].posX, initialShuriken[idx].posY);
			shurikens[idx].isMoving = true;
		}
	}
}

// functie ce realizeaza deplasarea balonului pe axa OY
void Tema1::MoveBalloon(float deltaTimeSeconds, int idx)
{
	// daca obiectul este in miscare, se continua translatia acestuia pana la o limita setata
	if (balloons[idx].isMoving)
	{
		balloons[idx].translateY = deltaTimeSeconds * 100;
		polylines[idx].translateY = deltaTimeSeconds * 100;
		balloons[idx].balloon *= Transform2D::Translate(balloons[idx].translateX, balloons[idx].translateY);
		polylines[idx].polyline *= Transform2D::Translate(polylines[idx].translateX, polylines[idx].translateY);
		// a iesit din scena
		if (transpose(balloons[idx].balloon)[1][2] > 870)
			balloons[idx].isMoving = false;
	}
	else
	{
		// a fost lovit
		if (hitBalloon)
			DestroyBalloon(deltaTimeSeconds, idx);
		// obiectul revine pe pozitia initiala
		else
		{
			balloons[idx].translateY = polylines[idx].translateX = 0;
			balloons[idx].balloon = polylines[idx].polyline = glm::mat3(1);
			balloons[idx].balloon *= Transform2D::Translate(initialBalloon[idx].posX, initialBalloon[idx].posY);
			polylines[idx].polyline *= Transform2D::Translate(initialBalloon[idx].posX, initialBalloon[idx].posY);
			balloons[idx].isMoving = true;
		}
	}
}

// functie ce realizeaza deplasarea sagetii
void Tema1::MoveArrow(float deltaTimeSeconds)
{
	// daca sageata a fost lansata
	if (throwArrow)
	{
		// se actualizeaza pozitia sagetii
		translateArrowX += deltaTimeSeconds * speedArrow;
		translateArrowY += tan(angle) * deltaTimeSeconds * speedArrow;
		if (translateArrowX > 1280 || translateArrowY < -400 || translateArrowY > 400)
		{
			// resetare viteza
			speedArrow = 400;
			throwArrow = false;
		}
	}
	else
	{
		// se readuce sageata in pozitia in care se afla arcul
		translateArrowX = 0;
		translateArrowY = translatePlayerY;
	}
}

void Tema1::DestroyShuriken(float deltaTimeSeconds, int idx)
{
	// se scaleaza pana dispare
	if (shurikens[idx].scaleX < 0)
	{
		score += 100;
		cout << "YOU HIT A SHURIKEN! SCORE: " << score << endl;
		shurikens[idx].translateX = 0;
		shurikens[idx].scaleX = shurikens[idx].scaleY = 1;
		hitShuriken = false;
	}
	shurikens[idx].scaleX -= 1 * deltaTimeSeconds * 0.8;
	shurikens[idx].scaleY -= 1 * deltaTimeSeconds * 0.8;
	shurikens[idx].shuriken *= Transform2D::Scale(shurikens[idx].scaleX, shurikens[idx].scaleY);
	
}

// functia care distruge balonul
void Tema1::DestroyBalloon(float deltaTimeSeconds, int idx)
{
	// se actualizeaza scorul in functie de culoare
	if (balloons[idx].scaleX < 0)
	{
		balloons[idx].translateY = 0;
		balloons[idx].scaleX = balloons[idx].scaleY= 1;
		hitBalloon = false;
	}
	balloons[idx].scaleX -= 1 * deltaTimeSeconds * 0.8;
	balloons[idx].scaleY -= 1 * deltaTimeSeconds * 0.8;
	balloons[idx].balloon *= Transform2D::Scale(balloons[idx].scaleX, balloons[idx].scaleY);
}

// functie care verifica coliziunea shuriken-jucator(arc)
void Tema1::CheckPlayerCollision()
{
	glm::mat3 b = bow;
	b= transpose(bow);
	hitPlayer = false;

	for (int i = 0; i < numShuriken; i++)
	{
		glm::mat3 m = shurikens[i].shuriken;
		m = glm::transpose(m);

		// variabile shuriken-jucator
		float psx = b[0][2] - m[0][2];
		float psy = b[1][2] - m[1][2];

		float distance_ps = sqrt(psx * psx + psy * psy);

		float radiusShuriken = 25.0f;
		float radiusPlayer = 45.0f;

		if (distance_ps < radiusPlayer + radiusShuriken)
			hitPlayer = true;
	}
}

// functia care verifica coliziunea sageata-shuriken
void Tema1::CheckShurikenCollision()
{
	point = glm::transpose(point);

	for (int i = 0; i < numShuriken; i++)
	{
		glm::mat3 m = shurikens[i].shuriken;
		m = glm::transpose(m);

		// variabile sageata-shuriken
		float psx = point[0][2] - m[0][2];
		float psy = point[1][2] - m[1][2];

		float distance_ps = sqrt(psx * psx + psy * psy);

		float radiusShuriken = 25.0f;

		if (distance_ps < radiusShuriken && throwArrow)
		{
			hitShuriken = true;
			shurikens[i].isMoving = false;
		}
	}
	
	point = glm::transpose(point);
}

// functia care verifica coliziunea sageata-balon
int Tema1::CheckBalloonCollision()
{
	point = glm::transpose(point);

	for (int i = 0; i < numBalloons; i++)
	{
		glm::mat3 m = balloons[i].balloon;
		m = glm::transpose(m);

		// variabile sageata-balon
		float pbx = point[0][2] - m[0][2];
		float pby = point[1][2] - m[1][2];

		float distance_pb = sqrt(pbx * pbx + pby * pby);

		float radiusBalloon = 30.0f;

		if (distance_pb < radiusBalloon)
		{
			hitBalloon = true;
			balloons[i].isMoving = false;
			point = glm::transpose(point);
			return i;
		}
	}

	point = glm::transpose(point);
	return -1;
}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
	if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{	
		// deplasare arc sus
		if (window->KeyHold(GLFW_KEY_W))
		{
			// daca nu iese din scena
			if (translatePlayerY < 300)
			{
				translatePlayerY += deltaTime * 500;
				// sageata nu e lansata
				if (!throwArrow)
					translateArrowY = translatePlayerY;
			}
			
		}
		// deplasare arc jos
		if (window->KeyHold(GLFW_KEY_S))
		{
			// daca nu iese din scena
			if (translatePlayerY > -300)
			{
				translatePlayerY -= deltaTime * 500;
				// sageata nu e lansata
				if (!throwArrow)
					translateArrowY = translatePlayerY;
			}
		}
		// cresterea vitezei de tragere
		if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT))
		{
			holdMouse = true;
			// se incarca segeata doar dupa ce revine in pozitie initiala
			if (!throwArrow)
			{
				if (scaleBarX < 13)
				{
					scaleBarX += deltaTime * 6;
					speedArrow += 4;
				}
			}
		}

		// daca s-a apasat butonul mouse-ului, se lanseaza sageata
		if (!window->MouseHold(GLFW_MOUSE_BUTTON_LEFT) && holdMouse)
		{
			scaleBarX = 1;
			holdMouse = false;
			throwArrow = true;
		}

	}
}

void Tema1::OnKeyPress(int key, int mods)
{
}

void Tema1::OnKeyRelease(int key, int mods)
{
}

void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{	
	mouseCrtX = mouseX;
	mouseCrtY = mouseY;
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}

void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema1::OnWindowResize(int width, int height)
{
}

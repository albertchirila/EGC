#pragma once
#include <Component/SimpleScene.h>
#include <Core/GPU/Mesh.h>

#include "Fuel.h"
#include "Camera.h"
#include "Player.h"
#include "Platform.h"
#include "Fuel.h"

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		// functii pentru initializare
		void InitFuel(Fuel* f, glm::vec3 color);
		void InitPlatform(int col, int platform, glm::vec3 color, float transX, float transZ, float scaleZ);
		glm::vec3 GetColor(int random);

		// functii de desenare
		void RenderPlatform(int col, int platform);
		void RenderFuel(Mesh* mesh, Shader* shader, glm::mat4 & matrix, glm::vec3 & color);
		void RenderSimpleMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix);

		//functii de amplasare in scena a obiectelor
		void CreateMap();
		void CreateFuel();

		//functii de miscare a jucatorului
		void MovePlayer(float deltaTimeSeconds);
		void Jump(float deltaTimeSeconds);
		void PlayerFall(float deltaTimeSeconds);

		//functii platforme
		void CheckPlatform(Platform* p);
		void ChangeColor(Platform *p, glm::vec3 color);

		// functie coliziune
		Platform* CheckCollision();
	
		// functie de final de joc -> afisarea mesaj in consola
		void EndGame(std::string message);

		// functii apsare taste
		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;

	protected:

		//numarul de coloane cu platforme
		int numColumns, random;

		// structura unei coloane cu platforme
		struct Column {
			int numPlatforms;
			std::vector<Platform*> platforms;
		};
		std::vector<Column> columns;

		Platform* copyPlatform;

		// jucatorul
		Player* player;

		// variabile pentru miscarea jucatorului
		float speed, copySpeed, copyTransZ;
		bool maxSpeed;
		bool jump, fall;
		bool up, down;
		bool limitLeft, limitRight;
		
		// combustibilul
		Fuel* fuelBar;
		Fuel* fuel;

		bool hitOnce;

		// camera
		Tema::Camera *camera;
		bool changeCamera;

		glm::mat4 projectionMatrix;
};

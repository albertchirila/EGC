/* NUME : CHIRILA ALBERT
   GRUPA: 334CB
 */

#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Tema1 : public SimpleScene
{
	public:
		Tema1();
		~Tema1();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

		// functii create
		void MoveShuriken(float deltaTimeSeconds, int idx);
		void MoveBalloon(float deltaTimeSeconds, int idx);
		void MoveArrow(float deltaTimeSeconds);
		int CheckBalloonCollision();
		void CheckShurikenCollision();
		void CheckPlayerCollision();
		void DestroyBalloon(float deltaTimeSeconds, int idx);
		void DestroyShuriken(float deltaTimeSeconds, int idx);

	protected:
		glm::mat3 bow;
		glm::mat3 arrow;
		glm::mat3 line;
		glm::mat3 top;
		glm::mat3 powerBar;
		glm::mat3 point;

		// numarul de baloane si numarul de shurikens
		int numBalloons, numShuriken;

		//SHURIKEN
		struct Shuriken {
			float translateX = 0;
			float translateY = 0;
			float angularStep = 0;
			float scaleX = 1;
			float scaleY = 1;
			bool isMoving = true;
			glm::mat3 shuriken = glm::mat3(1);
		};

		// pozitia initiala a shuriken-ului
		struct InitialShuriken {
			float posX, posY;
		};

		std::vector<Shuriken> shurikens;
		std::vector<InitialShuriken> initialShuriken;

		// BALON
		struct Balloon {
			float translateX = 0;
			float translateY = 0;
			float scaleX = 1;
			float scaleY = 1;
			bool isMoving = true;
			glm::mat3 balloon = glm::mat3(1);
		};

		// pozitia initiala a balonului
		struct InitialBalloon {
			float posX, posY;
		};

		std::vector<InitialBalloon> initialBalloon;
		std::vector<Balloon> balloons;
		std::vector<std::string> balloonColor;

		// POLILINIE
		struct Polyline {
			float translateX = 0;
			float translateY = 0;
			glm::mat3 polyline = glm::mat3(1);
		};

		std::vector<Polyline> polylines;
		std::vector<std::string> polylineColor;


		//variabile joc
		int lives;
		int score;

		//variabile verificare
		int copyIdx, contor;
		bool createBalloons = false;
		bool createShurikens = false;
		bool hitBalloon = false;
		bool hitShuriken = false;
		bool hitPlayer = false;
		bool wasHit = false;
		bool holdMouse = false;
		bool throwArrow = false;

		//viteza sagetii
		float speedArrow;

		//variabile pentru miscarea mouse-ului
		float mouseCrtX;
		float mouseCrtY;

		//variabile translatie
		float translatePlayerX, translatePlayerY;
		float translateArrowX, translateArrowY;

		//variabile scalare
		float scaleBarX;
		float scaleBarY;

		//variabile rotatie
		float angle;
};

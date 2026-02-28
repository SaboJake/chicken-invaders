#pragma once
#include "lab_m1/tema1/game/enemies/enemy.h"
#include <functional>

class Chicken : public Enemy {
	public:
		float timeEgg;

		Chicken(std::vector<Mesh*> meshes, int type, float x, float y, float z,
			float speed, int health, std::function<void(Chicken&, float)> movePattern)
			: Enemy(meshes, type, x, y, z, speed, health) {
			hitboxR = glm::vec2(2.1 * 64, 2.5 * 64);
			this->movePattern = movePattern;
			ResetEggTimer();
		}
		Chicken(std::vector<Mesh*> meshes, std::vector<glm::mat4> offsets, int type,
			float x, float y, float z, float speed, int health,
			std::function<void(Chicken&, float)> movePattern)
			: Enemy(meshes, offsets, type, x, y, z, speed, health) {
			hitboxR = glm::vec2(2.1 * 64, 2.5 * 64);
			this->movePattern = movePattern;
			ResetEggTimer();

		}
		Chicken(int type, float x, float y, float z, float speed, int health,
			std::function<void(Chicken&, float)> movePattern)
			: Enemy({}, {}, type, x, y, z, speed, health) {
			this->movePattern = movePattern;
			ResetEggTimer();

		}
		Enemy* Clone() const override {
			return new Chicken(*this);
		}
		void Move(float deltaTime) override;
		bool hitCheck(float x, float y) override;
		glm::vec2 getCenter() override;

		int moveDir = 1;

		bool SpawnEgg(float deltaTime);
private:
	void ResetEggTimer();
	std::function<void(Chicken&, float)> movePattern;
};
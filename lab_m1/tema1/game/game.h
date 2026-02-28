#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema1/editor/placed.h"
#include "lab_m1/tema1/game/projectiles/bullet.h"
#include "lab_m1/tema1/game/enemies/enemy.h"
#include "components/text_renderer.h"
#include "lab_m1/tema1/my_utils/repeated_action.h"

namespace m1
{
    class Game : public gfxc::SimpleScene
    {
    public:
        struct ViewportSpace
        {
            ViewportSpace() : x(0), y(0), width(1), height(1) {}
            ViewportSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {
            }
            int x;
            int y;
            int width;
            int height;
        };

        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {
            }
            float x;
            float y;
            float width;
            float height;
        };

    public:
        Game();
        ~Game();

        void Init() override;
        std::vector<Placed*> placedObjects;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

        // Sets the logic space and view space
        // logicSpace: { x, y, width, height }
        // viewSpace: { x, y, width, height }
        glm::mat3 VisualizationTransf2D(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);
        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace);

        void SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

    protected:
        float L;
        ViewportSpace viewSpace;
        LogicSpace logicSpace;
        glm::mat3 modelMatrix, visMatrix;
		glm::mat4 modelMatrix4D, visMatrix4D;

		glm::vec2 playerDirection;
		glm::vec2 playerPosition;
        float playerSpeed;
        float engineSpeedup;

		std::vector<glm::vec3> shootingPositions;
		std::vector<Bullet*> projectiles;
		std::vector<Bullet*> enemyProjectiles;

		std::vector<Enemy*> enemies;

		std::vector<std::vector<Enemy*>> enemyWaves;

		std::vector<Mesh*> defaultChickenMeshes();
        std::vector<glm::mat4> defaultChickenOffsets();
        std::vector<std::string> waveNames;
		void initWaves();
		void spawnWave(int waveIndex);
		int currentWaveIndex;

        float waveDelay;
        float waveTimer;
        bool waitingForNextWave;

        gfxc::TextRenderer* textRenderer;

		void AddBullets();
        int powerLevel;
        int maxPowerLevel;
        int equipedWeapon;

        float powerUpSpawnChance;
		std::vector<Bullet*> powerUps;
		void SpawnPowerUp(float x, float y);

        std::vector<std::vector<std::vector<Bullet*>>> initWeapons();
        std::vector<std::vector<std::vector<Bullet*>>> weapons;
		std::vector<float> weaponCooldowns;
        float timeSinceLastShot;

        int lives;
        int scoreThreshold;
		void ExtraLifeCheck();
        bool playerDead;
        bool invincible;
        float invicibilityTime;
        int flickerCount;
		std::vector<Placed*> placedObjectsCopy;
        RepeatedAction *invincibilityFlicker;
        bool invincibilityFlickerStep;
		bool invincibilityFlickerActive;

        int score;

		float screenShakeDuration;
		float screenShakeIntensity;
		bool screenShakeActive;
		void TriggerScreenShake(float duration, float intensity);

        void RestoreBumpers();
    };
}   // namespace m1

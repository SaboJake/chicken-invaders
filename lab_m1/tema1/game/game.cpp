#include "lab_m1/tema1/game/game.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/my_utils/transform2D.h"
#include "lab_m1/tema1/my_utils/transform3D.h"
#include "lab_m1/tema1/my_utils/object2D.h"
#include "lab_m1/tema1/my_utils/util.h"
#include "lab_m1/tema1/my_utils/repeated_action.h"
#include "lab_m1/tema1/game/projectiles/neutron_gun.h"
#include "lab_m1/tema1/game/projectiles/egg.h"
#include "lab_m1/tema1/game/projectiles/power_level.h"
#include "lab_m1/tema1/game/enemies/chicken.h"
#include "lab_m1/tema1/game/movement_patterns.h"
#include "lab_m1/tema1/editor/objects/bumper.h"
#include "components/text_renderer.h"
#include "core/managers/resource_path.h"

using namespace std;
using namespace m1;

#define MOUSE_BUTTON_LEFT 1
#define MOUSE_BUTTON_RIGHT 2

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

glm::vec2 getMouseLogicPos(int mouseX, int mouseY,
    Game::ViewportSpace viewSpace, Game::LogicSpace logicSpace)
{
    float scale = min((float)viewSpace.width / logicSpace.width,
        (float)viewSpace.height / logicSpace.height);

    float marginX = (viewSpace.width - logicSpace.width * scale) / 2;
    float marginY = (viewSpace.height - logicSpace.height * scale) / 2;

    glm::vec2 logicPos;
    logicPos.x = logicSpace.x + (mouseX - viewSpace.x - marginX) / scale;
    logicPos.y = logicSpace.y + (mouseY - viewSpace.y - marginY) / scale;

    return logicPos;
}

Game::Game()
{
}


Game::~Game()
{
}


vector<Mesh*> Game::defaultChickenMeshes() {
    return {
        { meshes["foot"], meshes["foot"], meshes["leg"], meshes["leg"], meshes["wing1"], meshes["wing2"], meshes["body"], meshes["eye1"], meshes["eye2"], meshes["head"] }
    };
}

vector<glm::mat4> Game::defaultChickenOffsets() {
    return {
        {
            transform3D::Translate(-0.7 * L, 2 * L, 0),
            transform3D::Translate(0.15 * L, 2 * L, 0),
            transform3D::Translate(0.3 * L, 1.7 * L, 0),
            transform3D::Translate(-0.3 * L, 1.7 * L, 0),
            transform3D::Translate(-0.4 * L, 1.3 * L, 0),
            transform3D::Translate(0.4 * L, 1.3 * L, 0),
            transform3D::Translate(-0.6 * L, 0.5 * L, 0),
            glm::mat4(1), glm::mat4(1), glm::mat4(1)
        }
    };
}

void Game::initWaves() {
	vector<Enemy*> wave0;
    for (int i = 0; i < 1; i++) {
        wave0.push_back(new Chicken(defaultChickenMeshes(), defaultChickenOffsets(),
            1,
            300.0f, 300.0f, 0.0f,
            50.0f,
            2,
            MovementPatterns::Stationary));
    }
	//enemyWaves.push_back(wave0);

    vector<Enemy*> wave1;
    for (int i = 0; i < 5; i++) {
        wave1.push_back(new Chicken(defaultChickenMeshes(), defaultChickenOffsets(),
            1, // Chicken type
            300.0f, 200.0f, 0.0f,
            100.0f,
            10,
            MovementPatterns::SineWave));
		wave1.back()->x += i * 200.0f; // Space out chickens
    }
	vector<Enemy*> wave2;
    for (int i = 0; i < 3; i++) {
        wave2.push_back(new Chicken(defaultChickenMeshes(), defaultChickenOffsets(),
            1, // Chicken type
            300.0f, 100.0f, 0.0f,
            100.0f,
            15,
			MovementPatterns::LinearDownward));
		wave2.back()->x += i * 200.0f; // Space out chickens
    }
	vector<Enemy*> wave3;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 3; j++) {
            wave3.push_back(new Chicken(defaultChickenMeshes(), defaultChickenOffsets(),
                1, // Chicken type
                300.0f + j * 250.0f, 100.0f + i * 200.0f, 0.0f,
                100.0f,
                10,
                MovementPatterns::LeftRightOscillation1));
		}
    }
	vector<Enemy*> wave4;
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 5; j++) {
            wave4.push_back(new Chicken(defaultChickenMeshes(), defaultChickenOffsets(),
                1, // Chicken type
                200.0f + j * 250.0f, 100.0f + i * 200.0f, 0.0f,
                10.0f,
                1,
				MovementPatterns::SineWaveDownward));
        }
    }
    enemyWaves.push_back({});
    enemyWaves.push_back(wave4);
	enemyWaves.push_back(wave1);
	enemyWaves.push_back(wave2);
    enemyWaves.push_back(wave3);
    enemyWaves.push_back(wave0);

    waveNames = {
		"Wave 1: The invasion begins",
        "Wave 2: Small Dance",
        "Wave 3: Dive Bombers",
        "Wave 4: 2x3",
        "Wave 5: The One"
    };

}

void Game::spawnWave(int waveIndex) {
    if (waveIndex < 0 || waveIndex >= enemyWaves.size()) {
        cout << "Invalid wave index: " << waveIndex << endl;
        return;
    }
    for (Enemy* enemy : enemyWaves[waveIndex]) {
        enemies.push_back(enemy->Clone());
    }
}


void Game::RestoreBumpers() {
    for (Placed* obj : placedObjects) {
        if (obj->type == 4) {
            Bumper* bumper = dynamic_cast<Bumper*>(obj);
            if (bumper) {
                bumper->topDestroyed = false;
                bumper->offsets[1] = transform3D::Translate(-L, -2.0f * L, 0.0f) * transform3D::Scale(3.0f, 2.0f, 1.0);
            }
        }
    }
}

void Game::TriggerScreenShake(float duration, float intensity) {
    screenShakeDuration = duration;
    screenShakeIntensity = intensity;
	screenShakeActive = true;
}

void Game::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    logicSpace.x = 0;       // logic x
    logicSpace.y = 0;       // logic y
    logicSpace.width = 1920;   // logic width
    logicSpace.height = 1080;  // logic height

    glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
    L = 64;

    Mesh* square1 = object2D::CreateSquare("square1", corner, L, glm::vec3(1, 0, 0));
    Mesh* roundedTriangle = object2D::CreateRoundedTriangle("roundedTriangle",
        glm::vec3(-0.23 * L + 0.5 * L, 0.95 * L, 0), glm::vec3(0 + 0.5 * L, L, 0), glm::vec3(-0.3 * L + 0.5 * L, 2 * L, 0), 0.01 * L, glm::vec3(0, 1, 0), true);
    Mesh* rt2 = object2D::CreateRoundedTriangle("rt2",
        glm::vec3(0.23 * L + 0.5 * L, 0.95 * L, 0), glm::vec3(0 + 0.5 * L, L, 0), glm::vec3(0.3 * L + 0.5 * L, 2 * L, 0), 0.01 * L, glm::vec3(0, 1, 0), true);
    Mesh* rt3 = object2D::CreateRoundedTriangle("rt3",
        glm::vec3(0 + 0.5 * L, 0, 0), glm::vec3(-0.25 * L + 0.5 * L, L, 0), glm::vec3(0.25 * L + 0.5 * L, L, 0), 0.05 * L, glm::vec3(0, 1, 0), true);

    std::vector<Mesh*> neuronGunParts = { roundedTriangle, rt2, rt3 };
    Mesh* neuronGunSprite = MergeMeshes("neutronGunSprite", neuronGunParts);

    Mesh* headBase = object2D::CreateCircle("headBase", glm::vec3(0, 0, 0), 0.5 * L, glm::vec3(0.7, 0.7, 0.7), true);
    Mesh* eye1 = object2D::CreateCircle("eye1", glm::vec3(-0.15 * L, -0.2 * L, 0), 0.1 * L, glm::vec3(0, 0, 0), true);
    Mesh* eye2 = object2D::CreateCircle("eye2", glm::vec3(0.15 * L, -0.2 * L, 0), 0.1 * L, glm::vec3(0, 0, 0), true);
    Mesh* beak = object2D::CreateRoundedTriangle("beak",
        glm::vec3(0, -0.1 * L, 0),
        glm::vec3(-0.2 * L, 0.1 * L, 0),
        glm::vec3(0.5 * L, 0.3 * L, 0),
        0.05 * L, glm::vec3(1, 0.5, 0), true);
    Mesh* comb1 = object2D::CreateRoundedTriangle("comb1",
        glm::vec3(-0.3 * L, -0.4 * L, 0),
        glm::vec3(-0.1 * L, -0.9 * L, 0),
        glm::vec3(0.1 * L, -0.5 * L, 0),
        0.05 * L, glm::vec3(1, 0, 0), true);
    Mesh* comb2 = object2D::CreateRoundedTriangle("comb2",
        glm::vec3(0.0f, -0.5 * L, 0),
        glm::vec3(0.3 * L, -1.2 * L, 0),
        glm::vec3(0.3 * L, -0.4 * L, 0),
        0.05 * L, glm::vec3(1, 0, 0), true);

    std::vector<Mesh*> eyeParts = { eye1, eye2 };
    Mesh* eyes = MergeMeshes("eyes", eyeParts);

    std::vector<Mesh*> headParts = { beak, comb1, comb2, headBase };
    Mesh* head = MergeMeshes("head", headParts);

    Mesh* body = object2D::CreateRoundedRectangle("body",
        glm::vec3(0, 0, 0),
        1.2 * L,
        1.2 * L,
        0.2f * L,
        glm::vec3(1, 1, 0), true);

    Mesh* wing1 = object2D::CreateRoundedTriangle("wing1",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(-1.5f * L, -L, 0.0f),
        glm::vec3(-L, -2 * L, 0.0f),
        0.1f * L,
        glm::vec3(0.7, 0.7, 0.7), true);

    Mesh* wing2 = object2D::CreateRoundedTriangle("wing2",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.5f * L, -L, 0.0f),
        glm::vec3(L, -2 * L, 0.0f),
        0.1f * L,
        glm::vec3(0.7, 0.7, 0.7), true);

    Mesh* leg = object2D::CreateRoundedRectangle("leg",
        glm::vec3(-0.1f * L, 0.0f, 0.0f),
        0.25f * L,
        0.4f * L,
        0.05f * L,
        glm::vec3(1, 0.5, 0), true);

    Mesh* foot = object2D::CreateRoundedTriangle("foot",
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.3f * L, 0.4f * L, 0.0f),
        glm::vec3(0.6f * L, 0, 0.0f),
        0.05f * L,
        glm::vec3(1, 0.5, 0), true);


	Mesh* egg = object2D::CreateCircle("egg", glm::vec3(0, 0, 0), 0.3 * L, glm::vec3(1, 1, 1), true);

	Mesh* powerLevelMesh = object2D::CreateCircle("powerLevelMesh", glm::vec3(0, 0, 0), 0.5 * L, glm::vec3(1, 0, 1), true);

    AddMeshToList(square1);
	AddMeshToList(roundedTriangle);
	AddMeshToList(rt2);
	AddMeshToList(rt3);
    AddMeshToList(neuronGunSprite);
	AddMeshToList(headBase);
	AddMeshToList(eye1);
	AddMeshToList(eye2);
	AddMeshToList(beak);
	AddMeshToList(comb1);
	AddMeshToList(comb2);
    AddMeshToList(head);
	AddMeshToList(eyes);

	AddMeshToList(body);

	AddMeshToList(wing1);
	AddMeshToList(wing2);
	AddMeshToList(leg);
	AddMeshToList(foot);

	AddMeshToList(egg);

	AddMeshToList(powerLevelMesh);

	playerDirection = glm::vec2(0, 0);
	playerPosition = glm::vec2(0, 0);
	playerSpeed = 200.0f;
	engineSpeedup = 100.0f;

    for (Placed* obj : placedObjects) {
        if (obj->type == 2) {
			shootingPositions.push_back(glm::vec3(obj->x, obj->y - 3 * L, obj->z));
		}
        else if (obj->type == 3) {
			playerSpeed += engineSpeedup;
        }
    };

    waveDelay = 5.0f;
	waveTimer = 0.0f;
	waitingForNextWave = false;

	powerUpSpawnChance = 0.2f;

	powerLevel = 1;
    maxPowerLevel = 3;
	equipedWeapon = 1; // Neutron Gun
	weapons = initWeapons();
	timeSinceLastShot = 0.0f;

	currentWaveIndex = 0;
	initWaves();
	spawnWave(0);

	textRenderer = new gfxc::TextRenderer(window->props.selfDir, logicSpace.width, logicSpace.height);
	textRenderer->Load("assets/fonts/Hack-Bold.ttf", 100);

	playerDead = false;
    lives = 3;
	invincible = false;
	score = 0;
    scoreThreshold = 10000;

	invicibilityTime = 5.0f;
    flickerCount = 50;

    invincibilityFlicker = new RepeatedAction(flickerCount, invicibilityTime / flickerCount, [this]() {
        if (this->invincibilityFlickerStep) {
            this->placedObjects = this->placedObjectsCopy;
        }
        else {
            this->placedObjects = {};
        }
        this->invincibilityFlickerStep = !this->invincibilityFlickerStep;
        });


    invincibilityFlickerActive = false;
    for (Placed* obj : placedObjects) {
        placedObjectsCopy.push_back(obj);
    }

	screenShakeDuration = 0.0f;
	screenShakeIntensity = 0.0f;
	screenShakeActive = false;

    glEnable(GL_DEPTH_TEST);
}

void Game::ExtraLifeCheck() {
    while (score >= scoreThreshold) {
        lives++;
        scoreThreshold += 10000;
    }
}

// Uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Game::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewportSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    smin = (sx < sy) ? sx : sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y + viewSpace.height - smin * logicSpace.y - (viewSpace.height - smin * logicSpace.height) / 2;

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, -smin, ty, // Invert the Y-axis scaling
        0.0f, 0.0f, 1.0f));
}


void Game::SetViewportArea(const ViewportSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}


void Game::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Game::Update(float deltaTimeSeconds)
{
	timeSinceLastShot += deltaTimeSeconds;

    if (screenShakeActive) {
		screenShakeDuration -= deltaTimeSeconds;
        if (screenShakeDuration > 0) {
            float offsetX = 2 * screenShakeIntensity * ((rand() % 100) / 100.0f - 0.5f);
            float offsetY = 2 * screenShakeIntensity * ((rand() % 100) / 100.0f - 0.5f);
			GetSceneCamera()->SetPosition(glm::vec3(offsetX, offsetY, 50));
        }
        else {
			screenShakeActive = false;
			GetSceneCamera()->SetPosition(glm::vec3(0, 0, 50));
        }
    }

    glm::ivec2 resolution = window->GetResolution();


    // The viewport is now the right half of the window

    viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
    SetViewportArea(viewSpace, glm::vec3(0), true);

    // Compute uniform 2D visualization matrix
    visMatrix = glm::mat3(1);
    visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);
    visMatrix4D = glm::mat4(
        visMatrix[0][0], visMatrix[0][1], 0.0f, 0.0f,
        visMatrix[1][0], visMatrix[1][1], 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        visMatrix[2][0], visMatrix[2][1], 0.0f, 1.0f
    );

    if (invincibilityFlickerActive) {
		invincibilityFlicker->Update(deltaTimeSeconds);
    }
    
    if (invincibilityFlickerActive && invincibilityFlicker->IsFinished()) {
		invincible = false;
		invincibilityFlickerActive = false;
    }

    DrawScene(visMatrix, deltaTimeSeconds);
}


void Game::FrameEnd()
{
}

void Game::DrawScene(glm::mat3 visMatrix, float deltaTimeSeconds)
{
    if (glm::length(playerDirection) > 0.0f) {
        playerDirection = glm::normalize(playerDirection);
		playerPosition += playerDirection * playerSpeed * deltaTimeSeconds;
	}
	glm::mat4 moveOffset = transform3D::Translate(playerPosition.x, playerPosition.y, 0.0f);

    // Draw the space ship
    for (Placed* object : placedObjects) {
        modelMatrix4D = visMatrix4D * transform3D::Translate(object->x, object->y, object->z);
		modelMatrix4D = modelMatrix4D * moveOffset;
        for (int k = 0; k < object->meshes.size(); k++) {
            RenderMesh(object->meshes[k], shaders["VertexColor"], modelMatrix4D * object->offsets[k]);
        }
    }

	// Draw projectiles
	auto it1 = projectiles.begin();
    while (it1 != projectiles.end()) {
        Bullet* bullet = *it1;
        bullet->Move(deltaTimeSeconds);
        bullet->Despawn(logicSpace.width, logicSpace.height);

        if (!bullet->isActive) {
            delete bullet;
            it1 = projectiles.erase(it1);
            continue;
        }

        modelMatrix4D = visMatrix4D * transform3D::Translate(bullet->x, bullet->y, bullet->z);
		modelMatrix4D = modelMatrix4D * bullet->offset;
        RenderMesh(bullet->mesh, shaders["VertexColor"], modelMatrix4D);
        it1++;
    }

	auto it2 = enemyProjectiles.begin();

    while (it2 != enemyProjectiles.end()) {
        Bullet* bullet = *it2;
        bullet->Move(deltaTimeSeconds);
        bullet->Despawn(logicSpace.width, logicSpace.height);
        if (!bullet->isActive) {
            delete bullet;
            it2 = enemyProjectiles.erase(it2);
            continue;
        }
        modelMatrix4D = visMatrix4D * transform3D::Translate(bullet->x, bullet->y, bullet->z);
		modelMatrix4D = modelMatrix4D * bullet->offset;
        RenderMesh(bullet->mesh, shaders["VertexColor"], modelMatrix4D);
        it2++;
	}

	auto itPowerUps = powerUps.begin();

    while (itPowerUps != powerUps.end()) {
        Bullet* powerUp = *itPowerUps;
        powerUp->Move(deltaTimeSeconds);
		powerUp->Despawn(logicSpace.width, logicSpace.height);
        if (!powerUp->isActive) {
            delete powerUp;
            itPowerUps = powerUps.erase(itPowerUps);
            continue;
        }
		modelMatrix4D = visMatrix4D * transform3D::Translate(powerUp->x, powerUp->y, powerUp->z);
        modelMatrix4D = modelMatrix4D * powerUp->offset;
        RenderMesh(powerUp->mesh, shaders["VertexColor"], modelMatrix4D);
        itPowerUps++;
	}

    for (Enemy* enemy : enemies) {
        enemy->Move(deltaTimeSeconds);

		enemy->OffscreenCheck(logicSpace.width, logicSpace.height);

        if (enemy->type == 1) { // Chicken
            Chicken* chicken = dynamic_cast<Chicken*>(enemy);
            if (chicken && chicken->isActive) {
				// Check if the chicken reaches the bottom of the screen
                if (chicken->y > logicSpace.height + 100) {
                    playerDead = true;
                }

                if (chicken->SpawnEgg(deltaTimeSeconds)) {
					// Spawn egg projectile
                    enemyProjectiles.push_back(new Egg(meshes["egg"], chicken->x, chicken->y + 2 * L, chicken->z + 0.1f));
                }
            }
		}

        modelMatrix4D = visMatrix4D * transform3D::Translate(enemy->x, enemy->y, enemy->z);
        for (int k = 0; k < enemy->meshes.size(); k++) {
            RenderMesh(enemy->meshes[k], shaders["VertexColor"], modelMatrix4D * enemy->offsets[k]);
        }
    }

    for (Bullet* bullet : projectiles) {
        if (!bullet->isActive) continue;

        for (Enemy* enemy : enemies) {
            if (!enemy->isActive) continue;

            if (enemy->hitCheck(bullet->getCenter().x, bullet->getCenter().y)) {

				// Add particle effect (maybe)

                bullet->isActive = false;

				int damageDealt = bullet->damage;
				enemy->health -= bullet->damage;                

                if (enemy->health <= 0) {
                    enemy->isActive = false;
					damageDealt += enemy->health;
                    score += 50;
					ExtraLifeCheck();

					SpawnPowerUp(enemy->x, enemy->y);

                }
				score += damageDealt * 10;
				ExtraLifeCheck();

                break;
            }
        }
    }

    for (Bullet* bullet : enemyProjectiles) {
        if (!bullet->isActive) continue;
        for (Placed* obj : placedObjects) {
            if (obj->clickCheck(bullet->getCenter().x - playerPosition.x,
                bullet->getCenter().y - playerPosition.y, L)) {
				cout << "Player hit!" << endl;
                bullet->isActive = false;

                // Destroy top part of bumper
                if (!invincible && obj->type == 4) {
                    Bumper* bumper = dynamic_cast<Bumper*>(obj);
                    if (bumper && !bumper->topDestroyed) {
                        bumper->topDestroyed = true;
                        // Disable top part mesh
                        //bumper->meshes.pop_back();
                        //bumper->offsets.pop_back();
                        bumper->offsets[1] = glm::mat4(0);
                        break;
                    }
                }

                if (invincible) {
                    break;
                }
                if (lives <= 0) {
                    playerDead = true;
                }
                else {
                    lives--;
                    powerLevel--;
                    if (powerLevel < 1) {
                        powerLevel = 1;
                    }
					invincibilityFlickerActive = true;
                    invincibilityFlickerStep = false;
					invincible = true;
					std::cout << "Player is now invincible for " << invicibilityTime << " seconds." << std::endl;
                    invincibilityFlicker->Reset(flickerCount, invicibilityTime / flickerCount);
					RestoreBumpers();
                    
					TriggerScreenShake(0.5f, 5.0f);
                    break;
				}

                break;
            }
        }
    }

    for (Bullet* powerUp : powerUps) {
        if (!powerUp->isActive) continue;
        for (Placed* obj : placedObjects) {
            if (obj->clickCheck(powerUp->getCenter().x - playerPosition.x,
                powerUp->getCenter().y - playerPosition.y, L)) {
                powerUp->isActive = false;
                cout << "Power-up collected!" << endl;
                score += 1000;
				ExtraLifeCheck();

                if (powerUp->type == 1000) {
                    powerLevel++;
                    if (powerLevel > maxPowerLevel) {
                        powerLevel = maxPowerLevel;
                    }
                }
            }
        }
    }


    auto itEnemies = enemies.begin();
    while (itEnemies != enemies.end()) {
        if (!(*itEnemies)->isActive) {
            delete* itEnemies;
            itEnemies = enemies.erase(itEnemies);
        }
        else {
            itEnemies++;
        }
    }

    if (enemies.empty() && currentWaveIndex < enemyWaves.size()) {
        if (!waitingForNextWave) {
            waitingForNextWave = true;
            waveTimer = 0.0f;

        }
        else {
            waveTimer += deltaTimeSeconds;
            if (waveTimer >= waveDelay) {
                currentWaveIndex++;
                spawnWave(currentWaveIndex);
                waitingForNextWave = false;
            }
        }
    }

    if (!playerDead && waitingForNextWave && currentWaveIndex < waveNames.size()) {
        textRenderer->RenderText(waveNames[currentWaveIndex], 200.0f, logicSpace.height / 2, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    if (playerDead) {
		placedObjects.clear();
		shootingPositions.clear();
        enemies.clear();
		projectiles.clear();
		enemyProjectiles.clear();
		powerUps.clear();

		textRenderer->RenderText("Game Over!", logicSpace.width / 2 - 500.0f, logicSpace.height / 2, 2.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	textRenderer->RenderText("Lives: " + to_string(lives), logicSpace.width - 500.0f, logicSpace.height - 100.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    textRenderer->RenderText("Score: " + to_string(score), 10.0f, 10.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Game::OnInputUpdate(float deltaTime, int mods)
{
	playerDirection.x = window->KeyHold(GLFW_KEY_D) - window->KeyHold(GLFW_KEY_A);
    playerDirection.y = window->KeyHold(GLFW_KEY_S) - window->KeyHold(GLFW_KEY_W);
}

void Game::SpawnPowerUp(float x, float y) {
    float randomChance = static_cast<float>(rand()) / RAND_MAX;
    if (randomChance <= powerUpSpawnChance) {
        PowerLevel* powerUp = new PowerLevel(meshes["powerLevelMesh"], x, y, 0.1f, 100.0f, 1, 0);
        powerUps.push_back(powerUp);
    }
}

vector<vector<vector<Bullet*>>> Game::initWeapons() {
    vector<vector<vector<Bullet*>>> weapons;
    weapons.push_back(vector<vector<Bullet*>>()); // Placeholder for index 0
    vector<vector<Bullet*>> neutronGunLevels = {
        {
            new NeutronGun(
                meshes["neutronGunSprite"],
                0, 0, 0,
                -1000.0f,
                1,
                3
            )
        },
        {
            new NeutronGun(
                meshes["neutronGunSprite"],
                -0.5 * L, 0, 0,
                -1000.0f,
                1,
                3
            ),
            new NeutronGun(
                meshes["neutronGunSprite"],
                0.5 * L, 0, 0,
                -1000.0f,
                1,
                3
            )
        },
        {
            new NeutronGun(
                meshes["neutronGunSprite"],
                transform3D::Scale(1.5f, 1.5f, 1.0f),
                -0.25 * L, -0.375 * L, 0,
                -1050.0f,
                1,
                7
			),
}
    };
	weapons.push_back(neutronGunLevels);
    weaponCooldowns = {
		1000.0f, // Placeholder for index 0
        0.3f, // Neutron Gun
	};
    
	return weapons;
}

void Game::AddBullets()
{
    switch (equipedWeapon) {
    case 1: // Neutron Gun
        switch (powerLevel) {
        case 1:
        {
            NeutronGun* bullet = new NeutronGun(
                meshes["neutronGunSprite"],
                playerPosition.x,
                playerPosition.y,
                0.0f,
                -1000.0f,
                1,
                3
            );
            projectiles.push_back(bullet);
            break;
        }
        case 2:
        {
            NeutronGun* bullet1 = new NeutronGun(
                meshes["neutronGunSprite"],
                playerPosition.x - 0.5 * L,
                playerPosition.y,
                0.0f,
                -1000.0f,
                1,
                3
            );
            NeutronGun* bullet2 = new NeutronGun(
                meshes["neutronGunSprite"],
                playerPosition.x + 0.5 * L,
                playerPosition.y,
                0.0f,
                -1000.0f,
                1,
                3
            );
            projectiles.push_back(bullet1);
            projectiles.push_back(bullet2);
            break;
		}
        default:
            break;
        }
    default:
        break;
    }
}

void Game::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_SPACE) {
        cout << "Spacebar pressed!" << endl;
		// Spawn bullet
        if (timeSinceLastShot >= weaponCooldowns[equipedWeapon]) {
			timeSinceLastShot = 0.0f;
            for (glm::vec3 pos : shootingPositions) {
                for (Bullet* bullet : weapons[equipedWeapon][powerLevel - 1]) {
                    Bullet* newBullet = bullet->Clone();
                    newBullet->x += pos.x + playerPosition.x;
                    newBullet->y += pos.y + playerPosition.y;
                    projectiles.push_back(newBullet);
                }
            }
        }
	}
}


void Game::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Game::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Game::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	glm::vec2 logicPos = getMouseLogicPos(mouseX, mouseY, viewSpace, logicSpace);
    // Add mouse button press event
    if (button == MOUSE_BUTTON_LEFT) {
        cout << "Left mouse button pressed at (" << logicPos.x << ", " << logicPos.y << ")" << endl;
	}
}


void Game::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Game::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

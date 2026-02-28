#include "lab_m1/tema1/scene.h"
#include <iostream>

namespace m1 {
    Scene::Scene() : currentScene(nullptr) {
		std::cout << "Scene constructor called." << std::endl;
    }

    Scene::~Scene() {
        if (currentScene) {
            delete currentScene;
            currentScene = nullptr;
        }
    }

    void Scene::SetCurrentScene(SimpleScene* scene) {
        if (currentScene) {
            currentScene->SetActive(false);
            delete currentScene;
        }
        currentScene = scene;
    }

    void Scene::SwitchScene(SimpleScene* newScene) {
        std::cout << "Switching scene..." << std::endl;
        if (currentScene) {
            currentScene->SetActive(false);
            delete currentScene;
        }
        currentScene = newScene;
        if (currentScene) {
            currentScene->Init();
        }
    }

    void Scene::Init() {
		std::cout << "Scene Init called." << std::endl;
        if (currentScene) {
            currentScene->Init();
        }
    }

    void Scene::OnInputUpdate(float deltaTime, int mods) {
        if (currentScene) {
            currentScene->OnInputUpdate(deltaTime, mods);
        }
	}

    void Scene::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {
        if (currentScene) {
            currentScene->OnMouseScroll(mouseX, mouseY, offsetX, offsetY);
        }
	}

    void Scene::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
        if (currentScene) {
            currentScene->OnMouseMove(mouseX, mouseY, deltaX, deltaY);
        }
    }

    void Scene::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
		std::cout << "Scene received OnMouseBtnPress event." << std::endl;
        if (currentScene) {
            currentScene->OnMouseBtnPress(mouseX, mouseY, button, mods);
        }
    }

    void Scene::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
        if (currentScene) {
            currentScene->OnMouseBtnRelease(mouseX, mouseY, button, mods);
        }
    }

    void Scene::OnKeyPress(int key, int mods) {
        if (currentScene) {
            currentScene->OnKeyPress(key, mods);
        }
    }

    void Scene::OnKeyRelease(int key, int mods) {
        if (currentScene) {
            currentScene->OnKeyRelease(key, mods);
        }
    }

    void Scene::Update(float deltaTimeSeconds) {
        if (currentScene) {
			currentScene->Update(deltaTimeSeconds);
        }
    }
}
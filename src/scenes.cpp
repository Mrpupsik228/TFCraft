#include "scenes.hpp"

void SceneManager::changeScene(std::unique_ptr<Scene> newScene) {
    if (currentScene) {
        currentScene->unload();
    }
    currentScene = std::move(newScene);
    currentScene->load();
}
        
void SceneManager::updateScene(float dt) {
    if (currentScene) {
        currentScene->update(dt);
    }
}

void Scene::unload() {};

Scene::~Scene() {
    Scene::unload();
}
#include "engine/engine.h"
#include "scenes.hpp"

class GameScene : public Scene {
public:
    void load() override {
        std::cout << "Loading Game Scene" << std::endl;
    }
    void update(float dt) override {
        std::cout << "Updating Game Scene" << std::endl;
    }
    void unload() override {
        std::cout << "Unloading Game Scene" << std::endl;
    }

    ~GameScene() {
        GameScene::unload();
    }
};

int main() {
    BS::Window::create(1280, 720, "TFCraft");
    
    SceneManager sceneManager;

    sceneManager.changeScene(std::make_unique<GameScene>());

    while(BS::Window::isRunning()) {
        BS::Window::pollEvents();
        sceneManager.updateScene(0.016f);
        BS::Window::swapBuffers();
    }

    BS::Window::close();
}
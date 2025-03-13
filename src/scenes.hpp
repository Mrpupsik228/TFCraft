#include <iostream>
#include <memory>

class Scene {
public:
    virtual void load() = 0;
    virtual void update(float dt) = 0;
    virtual void unload() = 0;

    virtual ~Scene();
};
      
class SceneManager {
private:
    std::unique_ptr<Scene> currentScene;
public:
    void changeScene(std::unique_ptr<Scene> newScene);
    
    void updateScene(float dt);
};

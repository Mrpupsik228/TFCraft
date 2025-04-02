#include "engine/engine.h"
#include "chunk.hpp"

int main() {
    BS::Window::create(800, 600, "Hello World");

    BS::Window::disableVSync();

    BS::Timer time;

    BS::ShaderProgram shaderProgram = BS::ShaderProgram("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl", nullptr);

    Chunk *chunk = new Chunk();

    chunk->setBlock(0, 0, 0, 1);

    BS::Mesh mesh = chunk->generate();

    glm::mat4 projection = BS::MatrixHelper::perspective(90.0, BS::Window::getAspect(), 0.1, 500);
    glm::mat4 view = BS::MatrixHelper::view(glm::vec3(0, 0, 5), glm::vec3(0, 0, 0));

    glm::vec3 cameraVelocyti = glm::vec3(0, 0, 0);
    glm::vec3 cameraPos = glm::vec3(0, 0, 0);

    while(BS::Window::isRunning()) {
        BS::Window::pollEvents();
        time.update();

        if(BS::Window::isKeyPressed(BS::KeyCode::W)) {
            cameraVelocyti.x = 1;
        }
        if(BS::Window::isKeyPressed(BS::KeyCode::S)) {
            cameraVelocyti.x = -1;
        }
        if(BS::Window::isKeyPressed(BS::KeyCode::A)) {
            cameraVelocyti.z = -1;
        }
        if(BS::Window::isKeyPressed(BS::KeyCode::D)) {
            cameraVelocyti.z = 1;
        }

        cameraPos += cameraVelocyti * time.getDelta();

        

        shaderProgram.use();
        shaderProgram.setMatrix4("projectionView", projection * view);
        

        //BS::Logger::info("FPS: %i", static_cast<int>(1.0f / time.getDelta()));
        mesh.render();

        BS::Window::swapBuffers();
    }

    mesh.destroy();
    BS::Window::close();
    return 0;
}
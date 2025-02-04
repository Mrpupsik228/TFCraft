#pragma once
#include <glm/gtc/matrix_transform.hpp>

struct Transform {
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 velocity;

    Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 velocity): position(position), rotation(rotation), velocity(velocity) {};
    Transform(glm::vec3 position, glm::vec3 velocity): position(position), rotation(glm::vec3(0)), velocity(velocity) {};
    Transform(glm::vec3 position): position(position), rotation(glm::vec3(0)), velocity(glm::vec3(0)) {};
    Transform(): position(glm::vec3(0)), rotation(glm::vec3(0)), velocity(glm::vec3(0)) {};
};

glm::vec3 collide(Transform a, Transform b);


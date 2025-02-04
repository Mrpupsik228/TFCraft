#pragma once

#include "engine/timer.hpp"
#include <glm/glm.hpp>

class Camera {
private:
	glm::vec3 front, up, right;
	glm::mat4 projection, view, projectionView;
public:
	glm::vec3 position, rotation;
	float fov, near, far;

	Camera(float fov, float near, float far);

	void update();
	void fly(const Timer& timer, float speed, float sensitivity);

	const glm::vec3& getFront() const;
	const glm::vec3& getUp() const;
	const glm::vec3& getRight() const;

	const glm::mat4& getProjection() const;
	const glm::mat4& getView() const;
	const glm::mat4 getProjectionView() const;
};
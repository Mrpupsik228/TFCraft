#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include "engine/engine.hpp"

Camera::Camera(float fov, float near, float far) {
	this->fov = fov;
	this->near = near;
	this->far = far;

	this->position = glm::vec3();
	this->rotation = glm::vec3();

	this->front = glm::vec3();
	this->up = glm::vec3();
	this->right = glm::vec3();

	this->projection = glm::mat4(1.0f);
	this->view = glm::mat4(1.0f);
	this->projectionView = glm::mat4(1.0f);
}

void Camera::update() {
	glm::mat4 rotator = glm::mat4(1.0f);
	rotator = glm::rotate(rotator, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	rotator = glm::rotate(rotator, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	rotator = glm::rotate(rotator, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));

	this->front = glm::vec3(rotator * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
	this->up = glm::vec3(rotator * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	this->right = glm::vec3(rotator * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

	this->projection = glm::perspective(glm::radians(this->fov), Window::getAspect(), this->near, this->far);
	this->view = glm::lookAt(this->position, this->position + this->front, this->up);
	this->projectionView = this->projection * this->view;
}
void Camera::fly(const Timer& timer, float speed, float sensitivity) {
	this->rotation.x -= Input::getCursorDeltaY() * sensitivity;
	this->rotation.y -= Input::getCursorDeltaX() * sensitivity;

	glm::vec3 wishVelocity = glm::vec3();

	if (Input::isKeyPressed(GLFW_KEY_W)) {
		wishVelocity += this->front;
	}
	if (Input::isKeyPressed(GLFW_KEY_S)) {
		wishVelocity -= this->front;
	}
	if (Input::isKeyPressed(GLFW_KEY_D)) {
		wishVelocity += this->right;
	}
	if (Input::isKeyPressed(GLFW_KEY_A)) {
		wishVelocity -= this->right;
	}
	if (Input::isKeyPressed(GLFW_KEY_E)) {
		wishVelocity += this->up;
	}
	if (Input::isKeyPressed(GLFW_KEY_Q)) {
		wishVelocity -= this->up;
	}
	if (Input::isKeyPressed(GLFW_KEY_SPACE)) {
		wishVelocity.y += 1.0f;
	}
	if (Input::isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
		wishVelocity.y -= 1.0f;
	}

	float magnitude = glm::length(wishVelocity);
	if (magnitude > 0.001f) {
		wishVelocity /= magnitude;
	}

	wishVelocity *= speed;
	this->position += wishVelocity * timer.getDeltaTime();
}

const glm::vec3& Camera::getFront() const {
	return this->front;
}
const glm::vec3& Camera::getUp() const {
	return this->up;
}
const glm::vec3& Camera::getRight() const {
	return this->right;
}

const glm::mat4& Camera::getProjection() const {
	return this->projection;
}
const glm::mat4& Camera::getView() const {
	return this->view;
}
const glm::mat4 Camera::getProjectionView() const {
	return this->projectionView;
}
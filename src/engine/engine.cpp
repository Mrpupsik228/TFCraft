#include "engine.hpp"

#include <assert.h>
#include <iostream>
#include <math.h>

#include <glad/glad.h>
#include <stb_image.h>

float Window::width = 0.0f, Window::height = 0.0f;
float Window::aspect = 1.0f;

GLFWwindow *Window::handle = nullptr;
bool Window::justResized = false;

void Window::create(int width, int height, const char* title) {
	assert(glfwInit());

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

	Window::handle = glfwCreateWindow(width, height, title, nullptr, nullptr);
	assert(Window::handle != nullptr);

	glfwSetFramebufferSizeCallback(Window::handle, Window::frameBufferSizeCallback);
	glfwSetKeyCallback(Window::handle, Input::keyCallback);
	glfwSetMouseButtonCallback(Window::handle, Input::mouseCallback);

	glfwMakeContextCurrent(Window::handle);
	assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

	glfwSwapInterval(0);
	stbi_set_flip_vertically_on_load(true);

	Window::width = width;
	Window::height = height;
	Window::aspect = Window::width / Window::height;
}
void Window::destroy() {
	glfwDestroyWindow(Window::handle);
	glfwTerminate();

	Window::handle = nullptr;
	Window::justResized = false;
}

void Window::pollEvents() {
	Window::justResized = false;

	glfwPollEvents();
	Input::frameCounter++;

	double mouseX, mouseY;
	glfwGetCursorPos(Window::handle, &mouseX, &mouseY);

	Input::cursorX = (float)mouseX;
	Input::cursorY = (float)mouseY;

	if (!Input::cursorJustReleased) {
		Input::cursorDeltaX = Input::cursorX - Input::previousCursorX;
		Input::cursorDeltaY = Input::cursorY - Input::previousCursorY;
	} else {
		Input::cursorJustReleased = false;

		Input::cursorDeltaX = 0.0f;
		Input::cursorDeltaY = 0.0f;
	}

	Input::previousCursorX = Input::cursorX;
	Input::previousCursorY = Input::cursorY;
}
void Window::swapBuffers() {
	glfwSwapBuffers(Window::handle);
}
void Window::resetViewport() {
	assert(Window::handle != nullptr);
	glViewport(0, 0, (GLsizei)Window::getWidth(), (GLsizei)Window::getHeight());
}

float Window::getWidth() {
	return Window::width;
}
float Window::getHeight() {
	return Window::height;
}
float Window::getAspect() {
	return Window::aspect;
}

bool Window::isJustResized() {
	return Window::justResized;
}
bool Window::isRunning() {
	return !glfwWindowShouldClose(Window::handle);
}

GLFWwindow* Window::getHandle() {
	return Window::handle;
}

void Window::frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
	Window::width = width;
	Window::height = height;

	Window::resetViewport();

	if (fabsf(Window::height) > 0.001f) {
		Window::aspect = Window::width / Window::height;
	} else {
		Window::aspect = 1.0f;
		std::cout << "Window::frameBufferSizeCallback: Could not calculate aspect, because height is 0 - that causes division by zero.\n";
	}

	Window::justResized = true;
}

uint64_t Input::keyFrames[GLFW_KEY_LAST + 1] = {};
uint64_t Input::mouseFrames[GLFW_MOUSE_BUTTON_LAST + 1] = {};

float Input::cursorX = 0.0f, Input::cursorY = 0.0f;
float Input::cursorDeltaX = 0.0f, Input::cursorDeltaY = 0.0f;
float Input::previousCursorX = 0.0f, Input::previousCursorY = 0.0f;

size_t Input::frameCounter = 1;
bool Input::cursorJustReleased = false;

void Input::grabCursor()  {
	glfwSetInputMode(Window::getHandle(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void Input::releaseCursor() {
	glfwSetInputMode(Window::getHandle(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	Input::cursorJustReleased = true;
}
void Input::toggleCursor() {
	glfwSetInputMode(Window::getHandle(), GLFW_CURSOR, glfwGetInputMode(Window::getHandle(), GLFW_CURSOR) == GLFW_CURSOR_NORMAL ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

bool Input::isKeyPressed(size_t key) {
	return key <= GLFW_KEY_LAST && Input::keyFrames[key] > 0LU;
}
bool Input::isKeyJustPressed(size_t key) {
	return key <= GLFW_KEY_LAST && Input::keyFrames[key] == (Input::frameCounter - 1);
}

bool Input::isMousePressed(size_t button) {
	return button <= GLFW_MOUSE_BUTTON_LAST && Input::mouseFrames[button] > 0;
}
bool Input::isMouseJustPressed(size_t button) {
	return button <= GLFW_MOUSE_BUTTON_LAST && Input::mouseFrames[button] == (Input::frameCounter - 1);
}

float Input::getCursorX() {
	return Input::cursorX;
}
float Input::getCursorY() {
	return Input::cursorY;
}

float Input::getCursorDeltaX() {
	return Input::cursorDeltaX;
}
float Input::getCursorDeltaY() {
	return Input::cursorDeltaY;
}

bool Input::isCursorReleased() {
	return glfwGetInputMode(Window::getHandle(), GLFW_CURSOR) == GLFW_CURSOR_NORMAL;
}

void Input::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		Input::keyFrames[key] = Input::frameCounter;
	} else if (action == GLFW_RELEASE) {
		Input::keyFrames[key] = 0;
	}
}
void Input::mouseCallback(GLFWwindow *window, int button, int action, int mods) {
	if (action == GLFW_PRESS) {
		Input::mouseFrames[button] = Input::frameCounter;
	} else if (action == GLFW_RELEASE) {
		Input::mouseFrames[button] = 0;
	}
}
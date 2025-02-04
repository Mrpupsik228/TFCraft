#pragma once

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

class Window {
private:
	static float width, height;
	static float aspect;
public:
	static void create(int width, int height, const char* title);
	static void destroy();

	static void pollEvents();
	static void swapBuffers();

	static void resetViewport();

	static float getWidth();
	static float getHeight();
	static float getAspect();

	static bool isJustResized();
	static bool isRunning();

	static GLFWwindow* getHandle();
private:
	static GLFWwindow* handle;
	static bool justResized;

	static void frameBufferSizeCallback(GLFWwindow *window, int width, int height);
};

class Input {
private:
	friend Window;

	static uint64_t keyFrames[GLFW_KEY_LAST + 1];
	static uint64_t mouseFrames[GLFW_MOUSE_BUTTON_LAST + 1];

	static float cursorX, cursorY;
	static float cursorDeltaX, cursorDeltaY;
	static float previousCursorX, previousCursorY;

	static size_t frameCounter;
	static bool cursorJustReleased;

	static void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
	static void mouseCallback(GLFWwindow *window, int button, int action, int mods);
public:
	static void grabCursor();
	static void releaseCursor();
	static void toggleCursor();

	static bool isKeyPressed(size_t key);
	static bool isKeyJustPressed(size_t key);

	static bool isMousePressed(size_t button);
	static bool isMouseJustPressed(size_t button);

	static float getCursorX();
	static float getCursorY();

	static float getCursorDeltaX();
	static float getCursorDeltaY();

	static bool isCursorReleased();
};
#pragma once

#include <glad/glad.h>

struct Texture {
	static GLuint loadFromFile(const char* location, GLuint filter, GLuint wrap);

	static void use(GLuint texture, GLuint slot);
	static void drop();
	static void destroy(GLuint texture);
};
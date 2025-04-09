#include "texture.hpp"

#include <iostream>
#include <stb_image.h>

GLuint Texture::loadFromFile(const char* location, GLuint filter, GLuint wrap) {
	int width, height, channels;
	unsigned char* data = stbi_load(location, &width, &height, &channels, STBI_rgb_alpha);
	
	GLint format = GL_RGBA;
	switch (channels) {
		case STBI_rgb: {
			format = GL_RGB;
			break;
		}
		case STBI_grey_alpha: {
			std::cout << "Texture::loadFromFile: Format \"Grayscale-Alpha\" is not supported.\n";
			break;
		}
		case STBI_grey: {
			format = GL_R;			
			break;
		}
		default: {
			break;
		}
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST + filter - GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);
	return texture;
}

void Texture::use(GLuint texture, GLuint slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, texture);
}
void Texture::drop() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
void Texture::destroy(GLuint texture) {
	glDeleteTextures(1, &texture);
}
#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader {
private:
	GLuint id;
public:
	Shader(const char* vertexLocation, const char* fragmentLocation);
	~Shader();

	void use() const;
	void destroy();

	static void drop();

	void setInt(const char* name, int value) const;
	void setFloat(const char* name, float value) const;
	void setVector2(const char* name, glm::vec2 value) const;
	void setVector3(const char* name, glm::vec3 value) const;
	void setVector4(const char* name, glm::vec4 value) const;
	void setMatrix4(const char* name, glm::mat4 value) const;
};
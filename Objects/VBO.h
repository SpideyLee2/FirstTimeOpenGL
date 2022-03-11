#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV;
};

// VBO (Vertex Buffer Object) is an array of references.
class VBO {
public:
	// Reference ID of the VBO.
	GLuint ID;

	// Constructor that generates a VBO and links it to indices.
	VBO(std::vector<Vertex>& vertices);

	void Bind();
	void Unbind();
	void Delete();
};
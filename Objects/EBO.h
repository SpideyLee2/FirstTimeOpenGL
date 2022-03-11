#pragma once

#include <glad/glad.h>
#include <vector>

// EBO (Entity Buffer Object) stores the indices.
class EBO {
public:
	// Reference ID of the EBO.
	GLuint ID;

	// Constructor that generates a EBO and links it to indices.
	EBO(std::vector<GLuint> indices);

	void Bind();
	void Unbind();
	void Delete();
};
#pragma once

#include<glad/glad.h>

// VBO (Vertex Buffer Object) is an array of references.
class VBO {
public:
	// Reference ID of the VBO.
	GLuint ID;

	// Constructor that generates a VBO and links it to indices.
	VBO(GLfloat* vertices, GLsizeiptr size);

	void Bind();
	void Unbind();
	void Delete();
};
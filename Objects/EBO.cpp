#include "EBO.h"

// Constructor that generates an EBO and links it to indices.
EBO::EBO(GLuint* indices, GLsizeiptr size) {
	// Generates the buffer object containing 1 objects.
	glGenBuffers(1, &ID);
	// Binds the EBO to GL_ELEMENT_ARRAY_BUFFER, making the EBO the binded object.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	// Stores vertices in the VBO.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Must be unbinded after VAO since EBO is stored in the VAO.
void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
	glDeleteBuffers(1, &ID);
}
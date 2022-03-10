#include "VBO.h"

// Constructor that generates a VBO and links it to vertices.
VBO::VBO(GLfloat* vertices, GLsizeiptr size) {
	// Generates the buffer object containing 1 objects.
	glGenBuffers(1, &ID);
	// Binds ID to GL_ARRAY_BUFFER, making ID the binded object.
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	// Stores vertices in the VBO.
	// GL_STREAM_...	- vertices will be modified once and used a few times.
	// GL_STATIC_...	- vertices will be modified once and used many times.
	// GL_DYNAMIC_...	- vertices will be modified many times and used many times.
	// GL_..._DRAW		- vertices will be used to draw an image on the screen.
	// GL_..._READ 		- vertices will read the buffer object into it.
	// GL_..._COPY 		- vertices will read the buffer object into it and be used to draw an image 
	//					  on the screen.
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
	glDeleteBuffers(1, &ID);
}
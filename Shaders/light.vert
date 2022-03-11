#version 330 core

// Light position
layout (location = 0) in vec3 aPos;

// Imports the model matrix from the main function
uniform mat4 model;
// Imports the camera matrix from the main function
uniform mat4 camMatrix;

void main() {
	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * model * vec4(aPos, 1.0f);
}
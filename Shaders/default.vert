#version 330 core

// Positions/coordinates
layout (location = 0) in vec3 aPos;
// Normals
layout (location = 1) in vec3 aNormal;
// Colors
layout (location = 2) in vec3 aColor;
// Texture Coordinates
layout (location = 3) in vec2 aTex;

// Outputs the current position for the fragment shader
out vec3 currPos;
// Outputs the normal of the triangle for the fragment shader
out vec3 normal;
// Outputs the color for the fragment shader
out vec3 color;
// Outputs the texture coordinates for the fragment shader
out vec2 texCoord;

// Imports the scale of the vertices from the main function
uniform float scale;

// Imports the camera matrix from the main function
uniform mat4 camMatrix;

// Imports the model matrix from the main function
uniform mat4 model;

void main() {
	// Calculates the current position
	currPos = vec3(model * vec4(aPos, 1.0f));
	// Assigns the normal from the vertex data to "normal"
	normal = aNormal;
	// Assigns the colors from the vertex data to "color"
	color = aColor;
	// Assigns the texture coordinates from the vertex data to "texCoord"
	texCoord = aTex;

	// Outputs the positions/coordinates of all vertices
	gl_Position = camMatrix * vec4(aPos, 1.0);
}
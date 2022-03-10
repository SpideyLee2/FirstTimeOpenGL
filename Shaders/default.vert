#version 330 core
// Positions/coordinates
layout (location = 0) in vec3 aPos;
// Colors
layout (location = 1) in vec3 aColor;
// Textures
layout (location = 2) in vec2 aTex;

// Outputs the color for the fragment shader
out vec3 color;

// Outputs the mapping of a texture
out vec2 texCoord;

// Controls the scale of the vertices
uniform float scale;

void main() {
	// Outputs the positions/coordinates of all vertices
	gl_Position = vec4(aPos.x + aPos.x * scale, aPos.y + aPos.y * scale, aPos.z + aPos.z * scale, 1.0);
	// Assigns the colors from the vertex data to "color"
	color = aColor;
	// Assigns the texture coordinates from the vertex data to "texCoord"
	texCoord = aTex;
}
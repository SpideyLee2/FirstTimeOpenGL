#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the color of the light from the main function
uniform vec4 lightColor;

void main() {
	// Outputs color of light
	FragColor = lightColor;
}
#version 330 core
// Outputs colors in RGBA
out vec4 FragColor;

// Inputs the color from the vertex shader
in vec3 color;

// Inputs the mapping of a texture
in vec2 texCoord;

// Controls which texture unit to use
uniform sampler2D tex0;

void main() {
	// Colors the rendered object with a texture
	FragColor = texture(tex0, texCoord);
}
#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Imports the color from the vertex shader
in vec3 color;

// Imports the texture coordinates from the vertex shader
in vec2 texCoord;

// Imports the normal (not necessarily normalized) from the vertex shader
in vec3 normal;

// Imports the current position from the vertex shader
in vec3 currPos;

// Gets the texture unit from the main function
uniform sampler2D tex0;
// Gets the texture unit for the previous texture's specular map from the main function
uniform sampler2D tex1;

// Gets the color of the light from the main function
uniform vec4 lightColor;
// Gets the position of the light from the main function
uniform vec3 lightPos;
// Gets the position of the camera from the main function
uniform vec3 camPos;

void main() {
	// AMBIENT LIGHTING
	float ambient = 0.20f;

	// DIFFUSE LIGHTING
	// Stores the normalized normal of the triangle
	vec3 normal = normalize(normal);
	// Stores the direction the light is hitting the triangle from
	vec3 lightDirection = normalize(lightPos - currPos);
	// The larger the angle between normal and lightDirection, the less intense the light is.
	// The dot product of these two vectors is equal to the cosine of the angle, since they're normalized.
	// Don't want negative colors, so minimum is 0.0f
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// SPECULAR LIGHTING
	// The max intesity of a specular light
	float specularLight = 0.50f;
	// Stores the normalized direction the camera is facing
	vec3 viewDirection = normalize(camPos - currPos);
	// Stores the direction of the light reflection
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	// Stores how much specular light there is at a certain angle
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	// Stores the specular value
	float specular = specAmount * specularLight;

	// Outputs textured and lit color
	FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient) + texture(tex1, texCoord).r * specular;
}
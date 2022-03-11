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

vec4 PointLight() {
	vec3 lightVec = lightPos - currPos;

	// Intesity of light with respect to distance from surface
	float dist = length(lightVec);
	float a = 3.0;
	float b = 0.7;
	float intensity = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// AMBIENT LIGHTING
	float ambient = 0.20f;

	// DIFFUSE LIGHTING
	// Stores the normalized normal of the triangle
	vec3 norm = normalize(normal);
	// Stores the direction the light is hitting the triangle from
	vec3 lightDirection = normalize(lightVec);
	// The larger the angle between normal and lightDirection, the less intense the light is.
	// The dot product of these two vectors is equal to the cosine of the angle, since they're normalized.
	// Don't want negative colors, so minimum is 0.0f
	float diffuse = max(dot(norm, lightDirection), 0.0f);

	// SPECULAR LIGHTING
	// The max intesity of a specular light
	float specularLight = 0.50f;
	// Stores the normalized direction the camera is facing
	vec3 viewDirection = normalize(camPos - currPos);
	// Stores the direction of the light reflection
	vec3 reflectionDirection = reflect(-lightDirection, norm);
	// Stores how much specular light there is at a certain angle
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	// Stores the specular value
	float specular = specAmount * specularLight;

	// Outputs textured and lit color
	return (texture(tex0, texCoord) * (diffuse + ambient) + texture(tex1, texCoord).r * specular * intensity) * lightColor;
}

vec4 DirecLight() {
	// AMBIENT LIGHTING
	float ambient = 0.20f;

	// DIFFUSE LIGHTING
	vec3 norm = normalize(normal);
	// Light comes from above
	vec3 lightDirection = normalize(vec3(1.0f, 1.0f, 0.0f));
	float diffuse = max(dot(norm, lightDirection), 0.0f);

	// SPECULAR LIGHTING
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - currPos);
	vec3 reflectionDirection = reflect(-lightDirection, norm);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	// Outputs textured and lit color
	return (texture(tex0, texCoord) * (diffuse + ambient) + texture(tex1, texCoord).r * specular) * lightColor;
}

vec4 SpotLight() {
	// Controls how large the area that is lit up is
	float outerCone = 0.90f;
	float innerCone = 0.95f;

	// AMBIENT LIGHTING
	float ambient = 0.20f;

	// DIFFUSE LIGHTING
	vec3 norm = normalize(normal);
	vec3 lightDirection = normalize(lightPos - currPos);
	float diffuse = max(dot(norm, lightDirection), 0.0f);

	// SPECULAR LIGHTING
	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - currPos);
	vec3 reflectionDirection = reflect(-lightDirection, norm);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

	// Calculates the intensity of the currPos based on its angle to the center of the light cone
	float angle = dot(vec3(0.0f, -1.0f, 0.0f), -lightDirection);
	float intensity = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

	// Outputs textured and lit color
	return (texture(tex0, texCoord) * (diffuse * intensity + ambient) + texture(tex1, texCoord).r * specular * intensity) * lightColor;
}

void main() {
	FragColor = SpotLight();
}
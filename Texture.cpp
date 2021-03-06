#include "Texture.h"

Texture::Texture(const char* image, const char* texType, GLuint slot, GLenum format, GLenum pixelType) : type(texType) {
	// Flips the image right-side-up, reads the image from a file, and stores it in bytes.
	int widthImg, heightImg, numColorCh;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColorCh, 0);

	// Generates openGL texture object.
	glGenTextures(1, &ID);
	// Insert texture into texture unit slot
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Sets the binded texture to render using GL_NEAREST (pixelated)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Repeats the texture over the horizontal axis
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//// Fills in the blank space with white
	//float flatColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, flatColor);

	// Assigns the image data to a texture.
	// glTexImage2D(type of texture, level, color channels of texture, width, height, 
	//				LEGACY(just put 0), color channels of image, data type of pixels, image data)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, format, pixelType, bytes);
	// Generates the mipmaps of the texture (used when texture is smaller/further away)
	glGenerateMipmap(GL_TEXTURE_2D);

	// Deletes the image data as it is already in the openGL texture object.
	stbi_image_free(bytes);

	// Unbinds the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::TexUnit(Shader& shader, const char* uniform, GLuint unit) {
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	shader.Activate();
	glUniform1i(texUni, unit);
}

void Texture::Bind() {
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete() {
	glDeleteTextures(1, &ID);
}

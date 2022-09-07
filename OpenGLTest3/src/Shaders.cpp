
#include "Shaders.h"


Shaders::CompileShaderStatus Shaders::CompileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = &source[0];
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)(alloca(length * (sizeof(char))));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile shader\n";
		std::cout << message << "\n";
		glDeleteShader(id);
		return { 0,1 };

	}
	return { id, 0 };
};

Shaders::ShaderProgramSource Shaders::ParseShader(const std::string& filepath) {
	std::ifstream stream(filepath);

	std::string line;
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;
	while (getline(stream, line)) {
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			ss[(int)type] << line << "\n";
		}
	}
	return { ss[0].str(), ss[1].str() };

};

Shaders::CompileShaderStatus Shaders::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	Shaders::CompileShaderStatus vs = Shaders::CompileShader(GL_VERTEX_SHADER, vertexShader);
	Shaders::CompileShaderStatus fs = Shaders::CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	if (vs.error || fs.error) {
		return { 0,1 };
	}
	glAttachShader(program, vs.id);
	glAttachShader(program, fs.id);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs.id);
	glDeleteShader(fs.id);

	return { program, 0 };
}


unsigned int Shaders::loadTexture(const char* filename) {
	int width, height, nrChannels;
	unsigned int texture;
	unsigned char* data = SOIL_load_image(filename, &width, &height, &nrChannels, 0);
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
	return texture;
}

void Shaders::defineTextureInputs() {
	//define how opengl reads texture data
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//Mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
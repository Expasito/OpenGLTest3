#include "Headers.h"

namespace Shaders {
	/*struct ShaderProgramSource {
		std::string VertexSource;
		std::string FragmentSource;
	};*/
	static struct SPS {
		std::string VertexSource;
		std::string FragmentSource;
	};
	unsigned int CompileShader(unsigned int type, const std::string& source);
	Shaders::SPS ParseShader(const std::string& filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

}

//struct Shaders::ShaderProgramSource {
//	std::string VertexSource;
//	std::string FragmentSource;
//};
//
unsigned int Shaders::CompileShader(unsigned int type, const std::string& source) {
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
		return 0;

	}
	return id;
};

Shaders::SPS Shaders::ParseShader(const std::string& filepath) {
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

unsigned int Shaders::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	unsigned int program = glCreateProgram();
	unsigned int vs = Shaders::CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = Shaders::CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}
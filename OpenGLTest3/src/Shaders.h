#pragma once

#include "Headers.h"

namespace Shaders {
	struct ShaderProgramSource {
		std::string VertexSource;
		std::string FragmentSource;
	};
	struct CompileShaderStatus {
		unsigned int id;
		bool error;
	};

	CompileShaderStatus CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filepath);
	CompileShaderStatus CreateShader(const std::string& vertexShader, const std::string& fragmentShader);


	unsigned int loadTexture(const char * filename);
	void defineTextureInputs();
}
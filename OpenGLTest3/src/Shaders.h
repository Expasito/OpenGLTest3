#pragma once

#include "Headers.h"

namespace Shaders {
	struct ShaderProgramSource {
		std::string VertexSource;
		std::string FragmentSource;
	};

	unsigned int CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

}
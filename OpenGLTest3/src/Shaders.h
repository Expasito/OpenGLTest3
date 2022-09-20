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
	struct TextData {
		int width, height, elms = 4;
		unsigned int associateTexture;
		unsigned int* pixels = nullptr;
		float u1, v1, u2, v2;
		void bindText() {
			glBindTexture(GL_TEXTURE_2D, associateTexture);
			pixels = (unsigned int*)(malloc(sizeof(unsigned int) * width * height * 4));
			glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_INT, this->pixels);
		};

		TextData(int width, int height, unsigned int associateTexture) : width(width), height(height), associateTexture(associateTexture) 
		{ bindText(); };
	};


	CompileShaderStatus CompileShader(unsigned int type, const std::string& source);
	ShaderProgramSource ParseShader(const std::string& filepath);
	CompileShaderStatus CreateShader(const std::string& vertexShader, const std::string& fragmentShader);


	TextData loadTexture(const char * filename);
	void defineTextureInputs();
}
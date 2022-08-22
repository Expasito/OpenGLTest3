#include "Headers.h"

#include "GLFW/glfw3.h"

void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

//converts the shaders from .shader to 2 strings that can be used
static ShaderProgramSource parseShader(const std::string& file) {
	std::ifstream stream(file);
	std::string line;
	enum class ShaderType {
		NONE=-1, VERTEX=0, FRAGMENT = 1
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
	return {ss[0].str(),ss[1].str()};
	//while(std::fstream(loc).)
	//std::cout << a << "\n";
}


int main() {
	ShaderProgramSource source = parseShader("../OpenGLTest3/res/shaders/Shader.shader");
	std::cout << source.FragmentSource;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Opengl window", nullptr, nullptr);
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSetKeyCallback(window, keyCallBack);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glViewport(0, 0, 800, 600);

	float verticies[] = {
		-0.5f,-.5f, 0.0f,
		5.f,-.5f,0.0f,
		0.0f,.5f,0.0f 
	};

	unsigned int VBO;
	glGenBuffers(1, & VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);



	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	//delete window;

}
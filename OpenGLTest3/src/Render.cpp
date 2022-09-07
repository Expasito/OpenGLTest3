#include "Render.h"


GLFWwindow* Render::init() {
	//Basic glfw init methods
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//create window pointer
	GLFWwindow* wind = glfwCreateWindow(800, 600, "Opengl window", nullptr, nullptr);

	//check to make sure glfw and window are working
	if (!glfwInit() || wind==NULL) {
		std::cout << "Failed to create Window. Failed under Render::init()\n";
		exit(EXIT_FAILURE);
	}
	return wind;
}

void Render::callBacks(GLFWwindow* wind, GLFWkeyfun keyfun, GLFWframebuffersizefun buffersizefun) {
	//other functions needed
	glfwMakeContextCurrent(wind);
	gladLoadGL(glfwGetProcAddress);
	glfwSetKeyCallback(wind, keyfun);
	glfwSetFramebufferSizeCallback(wind, buffersizefun);
	glfwSwapInterval(0);
	glEnable(GL_DEPTH_TEST);


	glViewport(0, 0, 800, 600);
}
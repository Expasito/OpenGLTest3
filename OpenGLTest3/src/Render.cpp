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

void Render::callBacks(GLFWwindow* wind, GLFWkeyfun keyCallBack, GLFWframebuffersizefun buffersizefun, GLFWmousebuttonfun mouseButtonCallback, GLFWscrollfun scrollCallBack, GLFWcursorposfun mouseCallBack) {
	//other functions needed
	glfwMakeContextCurrent(wind);
	gladLoadGL(glfwGetProcAddress);
	glfwSetKeyCallback(wind, keyCallBack);
	glfwSetFramebufferSizeCallback(wind, buffersizefun);
	glfwSetMouseButtonCallback(wind, mouseButtonCallback);
	glfwSetScrollCallback(wind, scrollCallBack);
	glfwSetCursorPosCallback(wind, mouseCallBack);
	glfwSwapInterval(0);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glfwSetInputMode(wind, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPos(wind, 400, 300);
}
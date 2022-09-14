#include "Headers.h"
#include "Shaders.h"
#include "Render.h"
#include "Object.h"
#include <random>
#include "Camera.h"

//#include "../Entities/Entity.h"

#define clearMemory glfwDestroyWindow(window); glfwTerminate();

//Create Camera
Camera camera(0, 0, -10);

//Create model, view, projection matrices
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection;

static bool mousePressed=false;

static bool left = false, right = false, forward = false, backward = false, up=false, down=false;


void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	left = glfwGetKey(window, GLFW_KEY_A);
	right = glfwGetKey(window, GLFW_KEY_D);
	forward = glfwGetKey(window, GLFW_KEY_W);
	backward = glfwGetKey(window, GLFW_KEY_S);
	up = glfwGetKey(window, GLFW_KEY_SPACE);
	down = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
	

}

void mouseButtonCallBack(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		if (mousePressed) {
			mousePressed = false;
		}
		else if (!mousePressed) {
			mousePressed = true;
		}
	}
}

void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
	camera.zoom(xoffset, yoffset);

}

void mouseCallBack(GLFWwindow* window, double xpos, double ypos) {
	camera.rotate(xpos, ypos);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


//Define static class variables to be reused


float randomInRange() {
	return (float)((float)std::rand() / (float)RAND_MAX) * 2 - 1;
}



int main() {

	//Create window and set key callback functions
	GLFWwindow* window = Render::init();
	Render::callBacks(window, keyCallBack, framebuffer_size_callback, mouseButtonCallBack, scrollCallBack, mouseCallBack);
	
	//Set random generator
	std::srand(std::time(0));



	//define how the shaders read texture inputs
	Shaders::defineTextureInputs();


	//load textures begins here
	unsigned int texture0 = Shaders::loadTexture("../OpenGLTest3/res/textures/Default.png");
	unsigned int texture1 = Shaders::loadTexture("../OpenGLTest3/res/textures/Texture1.png");
	unsigned int texture2 = Shaders::loadTexture("../OpenGLTest3/res/textures/Texture2.png");
	unsigned int texture3 = Shaders::loadTexture("../OpenGLTest3/res/textures/Deor.png");
	unsigned int texture4 = Shaders::loadTexture("../OpenGLTest3/res/textures/skybox2.png");

	


	Shaders::ShaderProgramSource source = Shaders::ParseShader("../OpenGLTest3/res/shaders/Shader.shader");
	Shaders::CompileShaderStatus shaderRet = Shaders::CreateShader(source.VertexSource, source.FragmentSource);
	//Exit program if shader error
	if (shaderRet.error) {
		clearMemory
			exit(1);
	}

	unsigned int shader = shaderRet.id;
	glUseProgram(shader);


	Render::initEntities();


	//Get uniform locations
	unsigned int translateDataLoc = glGetUniformLocation(shader, "translateData");
	unsigned int rotateDataLoc = glGetUniformLocation(shader, "rotateData");
	unsigned int scaleDataLoc = glGetUniformLocation(shader, "scaleData");

	Render::uniforms.push_back({translateDataLoc,"translateData"});
	Render::uniforms.push_back({rotateDataLoc,"rotateData"});
	Render::uniforms.push_back({scaleDataLoc,"scaleData"});

	//Get uniforms for camera data
	unsigned int modelLoc = glGetUniformLocation(shader, "model");
	unsigned int viewLoc = glGetUniformLocation(shader, "view");
	unsigned int projectionLoc = glGetUniformLocation(shader, "projection");

	Render::uniforms.push_back({modelLoc, "model"});
	Render::uniforms.push_back({viewLoc, "view"});
	Render::uniforms.push_back({projectionLoc, "projection"});



	float timer = 0;
	
	Entity e;
	e.addComponent<TransformComponent>(glm::vec3(1,1,1));
	e.addComponent<TextureComponent>(texture0);

	Entity e2;
	e2.addComponent<TransformComponent>()->translate = glm::vec3(4, 4, 4);
	e2.addComponent<TextureComponent>()->texture = texture2;

	Render::activateSkybox();

	//main run loop
	while (!glfwWindowShouldClose(window)) {
		auto t1 =std::chrono::high_resolution_clock::now();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Render::draw(Render::skybox);

		if (!mousePressed) {
			timer += .001;
			model = glm::mat4(1.0f);
			model = glm::rotate(model, (float)timer * glm::radians(5.0f), glm::vec3(0.5f, 1.0f, 0.0f));

		}


		//Adjust camera position
		camera.translate(left, right, up, down,forward,backward);

		view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);


		projection = glm::perspective(glm::radians(camera.fov), 800.0f / 600.0f, 0.01f, 1000.0f);


		//Send camera data to shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

		Render::draw(&e);
		Render::draw(&e2);

		glfwSwapBuffers(window);
		
		glfwPollEvents();
		auto t2 = std::chrono::high_resolution_clock::now();
		auto ms_init = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
		//std::cout << ms_init.count() << "ms\n";
		
	}

	//Macro for making program memory safe
	clearMemory;

}
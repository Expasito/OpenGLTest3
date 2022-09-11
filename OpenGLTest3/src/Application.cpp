#include "Headers.h"
#include "Shaders.h"
#include "Render.h"
#include "Object.h"
#include <random>
#include "Camera.h"

#define clearMemory delete[] objs; glfwDestroyWindow(window); glfwTerminate();

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

float Object::vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

unsigned int Object::indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
};

float randomInRange() {
	return (float)((float)std::rand() / (float)RAND_MAX) * 2 - 1;
}



int main() {
	
	//Create window and set key callback functions
	GLFWwindow* window = Render::init();
	Render::callBacks(window, keyCallBack, framebuffer_size_callback, mouseButtonCallBack,scrollCallBack, mouseCallBack);

	//Set random generator
	std::srand(std::time(0));
	
	
	//Create an array of objects to be drawn
	const int objsCount = 101;
	Object* objs = new Object[objsCount];
	for (int i = 0; i < objsCount-1; i++) {
		objs[i] = Object();
	}
	objs[100] = Object();



	const float textureDx = .01;
	//modify texture cords by moving +- .01
	for (int i = 3; i < 36*5; i += 5) {
		if (Object::vertices[i] == 1.0f)
			Object::vertices[i] -= textureDx;
		if (Object::vertices[i+1] == 1.0f)
			Object::vertices[i+1] -= textureDx;
		if (Object::vertices[i] == 0.0f)
			Object::vertices[i] += textureDx;
		if (Object::vertices[i + 1] == 0.0f)
			Object::vertices[i + 1] += textureDx;
	}


	

	//define how the shaders read texture inputs
	Shaders::defineTextureInputs();


	//load textures begins here
	unsigned int texture1 = Shaders::loadTexture("../OpenGLTest3/res/textures/Texture1.png");
	unsigned int texture2 = Shaders::loadTexture("../OpenGLTest3/res/textures/Texture2.png");
	unsigned int texture3 = Shaders::loadTexture("../OpenGLTest3/res/textures/Deor.png");


	//shader stuff begins here
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Object::vertices), Object::vertices, GL_STATIC_DRAW);

	


	Shaders::ShaderProgramSource source = Shaders::ParseShader("../OpenGLTest3/res/shaders/Shader.shader");
	Shaders::CompileShaderStatus shaderRet = Shaders::CreateShader(source.VertexSource, source.FragmentSource);
	//Exit program if shader error
	if (shaderRet.error) {
		clearMemory
		exit(1);
	}
	
	unsigned int shader = shaderRet.id;
	glUseProgram(shader);

	//get position data to shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//get texture data to shader
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//define which texture to use
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);



	//Get uniform locations
	unsigned int translateDataLoc = glGetUniformLocation(shader, "translateData");
	unsigned int rotateDataLoc = glGetUniformLocation(shader, "rotateData");
	unsigned int scaleDataLoc = glGetUniformLocation(shader, "scaleData");


	//Get uniforms for camera data
	unsigned int modelLoc = glGetUniformLocation(shader, "model");
	unsigned int viewLoc = glGetUniformLocation(shader, "view");
	unsigned int projectionLoc = glGetUniformLocation(shader, "projection");
	


	//create random values for transform data
	for (int i = 0; i < objsCount-1; i++) {
		objs[i].translate.x = randomInRange()*4;
		objs[i].translate.y = randomInRange() * 4;
		objs[i].translate.z = randomInRange()*4;
		objs[i].rotate.x = randomInRange();
		objs[i].rotate.y = randomInRange();
		objs[i].rotate.z = randomInRange();
		objs[i].scale.x = randomInRange();
		objs[i].scale.y = randomInRange();
		objs[i].scale.z = randomInRange();
	}

	objs[100].scale.x = 100;
	objs[100].scale.y = 100;
	objs[100].scale.z = 100;

	float timer = 0;
	
	//main run loop
	while (!glfwWindowShouldClose(window)) {
		auto t1 =std::chrono::high_resolution_clock::now();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


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

		
		glBindTexture(GL_TEXTURE_2D, texture1);
		for (int i = 0; i < objsCount-1; i++) {
			glUniform3fv(translateDataLoc, 1, glm::value_ptr(objs[i].translate));
			glUniform3fv(rotateDataLoc, 1, glm::value_ptr(objs[i].rotate*(float)glfwGetTime()));
			glUniform3fv(scaleDataLoc, 1, glm::value_ptr(objs[i].scale));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glm::mat4 model2 = glm::mat4(1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform3fv(translateDataLoc, 1, glm::value_ptr(objs[100].translate));
		glUniform3fv(rotateDataLoc, 1, glm::value_ptr(objs[100].rotate * (float)glfwGetTime()));
		glUniform3fv(scaleDataLoc, 1, glm::value_ptr(objs[100].scale));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		
		glfwPollEvents();
		auto t2 = std::chrono::high_resolution_clock::now();
		auto ms_init = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
		//std::cout << ms_init.count() << "ms\n";
		
	}

	//Macro for making program memory safe
	clearMemory

}
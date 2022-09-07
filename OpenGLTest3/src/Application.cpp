#include "Headers.h"
#include "Shaders.h"
#include "Render.h"
#include "Object.h"
#include <random>

#define clearMemory delete[] objs; glfwDestroyWindow(window); glfwTerminate();


void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}


//Define static class variables to be reused
float Object::vertices[] =  {
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
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
	Render::callBacks(window, keyCallBack, framebuffer_size_callback);

	//Set random generator
	std::srand(std::time(0));
	
	
	//Create an array of objects to be drawn
	const int objsCount = 10000;
	Object* objs = new Object[objsCount];
	for (int i = 0; i < objsCount; i++) {
		objs[i] = Object();
	}




	//define how the shaders read texture inputs
	Shaders::defineTextureInputs();


	//load textures begins here
	unsigned int texture = Shaders::loadTexture("../OpenGLTest3/res/textures/Texture1.png");
	unsigned int texture2 = Shaders::loadTexture("../OpenGLTest3/res/textures/Texture2.png");


	//shader stuff begins here
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Object::vertices), Object::vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Object::indices), Object::indices, GL_STATIC_DRAW);

	
	


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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//get color data to shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//get texture data to shader
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	//define which texture to use
	glBindTexture(GL_TEXTURE_2D, texture2);



	//Get uniform locations
	unsigned int translateDataLoc = glGetUniformLocation(shader, "translateData");
	unsigned int rotateDataLoc = glGetUniformLocation(shader, "rotateData");
	unsigned int scaleDataLoc = glGetUniformLocation(shader, "scaleData");



	//create random values for transform data
	for (int i = 0; i < objsCount; i++) {
		objs[i].translate.x = randomInRange();
		objs[i].translate.y = randomInRange();
		objs[i].rotate.x = randomInRange();
		objs[i].rotate.y = randomInRange();
		objs[i].rotate.z = randomInRange();
		objs[i].scale.x = randomInRange();
		objs[i].scale.y = randomInRange();
		objs[i].scale.z = randomInRange();
	}

	
	
	//main run loop
	while (!glfwWindowShouldClose(window)) {
		auto t1 =std::chrono::high_resolution_clock::now();
		glClear(GL_COLOR_BUFFER_BIT);
		
		for (int i = 0; i < objsCount; i++) {
			glUniform3fv(translateDataLoc, 1, glm::value_ptr(objs[i].translate));
			glUniform3fv(rotateDataLoc, 1, glm::value_ptr(objs[i].rotate*(float)glfwGetTime()));
			glUniform3fv(scaleDataLoc, 1, glm::value_ptr(objs[i].scale));
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);
		
		glfwPollEvents();
		auto t2 = std::chrono::high_resolution_clock::now();
		auto ms_init = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
		std::cout << ms_init.count() << "ms\n";
	}

	//Macro for making program memory safe
	clearMemory

}
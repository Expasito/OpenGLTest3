#include "Headers.h"
#include "Shaders.h"
#include "Render.h"


void keyCallBack(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}



int main() {
	GLFWwindow* window = Render::init();
	Render::callBacks(window, keyCallBack, framebuffer_size_callback);

	
	
	

	float verticies[] = {
		-0.5f,-.5f, 0.0f, 1.0f,0.0f,0.0f,
		0.5f,-.5f,0.0f, 0.0f,1.0f,0.0f,
		0.0f,.5f,0.0f, 0.0f,0.0f,1.0f,
		1.0f,.75f,-.25f,1.0f,1.0f,1.0f
	};

	unsigned int indicies[] = {
		0,1,3,2,3,1
	};

	//shader stuff begins here
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, & VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	
	


	Shaders::ShaderProgramSource source = Shaders::ParseShader("../OpenGLTest3/res/shaders/Shader.shader");
	unsigned int shader = Shaders::CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);

	//get position data to shader
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//get color data to shader
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	
	//uniforms here
	int vertexColorLocation = glGetUniformLocation(shader, "color");
	//glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);



	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		//glUniform4f(vertexColorLocation, 0.0f, glfwGetTime()/2, 0.0f, 1.0f);

		glDrawElements(GL_TRIANGLES, 6,GL_UNSIGNED_INT,0);
		glfwSwapBuffers(window);
		glfwPollEvents();
		
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	//delete window;

}
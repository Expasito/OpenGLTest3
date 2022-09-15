#include "Headers.h"
#include "Shaders.h"
#include "Render.h"
#include "Object.h"
#include <random>
#include "Camera.h"



#define clearMemory glfwDestroyWindow(window); glfwTerminate();

//Create Camera
Camera camera(0, 0, -10);

//Create model, view, projection matrices
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection;

static bool leftMouseButton = false, middleMouseButton = false, rightMouseButton = false;

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
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		leftMouseButton = action;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
		middleMouseButton = action;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		rightMouseButton = action;
	}

}

void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
	camera.zoom(xoffset, yoffset);

}

void mouseCallBack(GLFWwindow* window, double xpos, double ypos) {
	camera.rotate(xpos, ypos,rightMouseButton);
}
void resizeSceneWindow(float windowx, float windowy) {
	if (Render::windowHeight != windowx || Render::windowWidth != windowy) {
		Render::windowWidth = windowx;
		Render::windowHeight = windowy;
		glBindTexture(GL_TEXTURE_2D, Render::colorTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Render::windowWidth, Render::windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, Render::colorTexture, 0);

		glBindTexture(GL_TEXTURE_2D, Render::depthTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Render::windowWidth, Render::windowHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, Render::depthTexture, 0);
		glViewport(0, 0, Render::windowWidth, Render::windowHeight);
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	resizeSceneWindow(width, height);

}


float randomInRange() {
	return (float)((float)std::rand() / (float)RAND_MAX) * 2 - 1;
}


int main() {
	std::vector<Entity*> entities;
	//Create window and set key callback functions
	GLFWwindow* window = Render::init();
	Render::callBacks(window, keyCallBack, framebuffer_size_callback, mouseButtonCallBack, scrollCallBack, mouseCallBack);
	
	//Set random generator
	std::srand(std::time(0));



	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::GetIO().ConfigFlags |= 1 << 6;
	static bool yes = true;
	
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 330";
	ImGui_ImplOpenGL3_Init(glsl_version);


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

	unsigned int colorLoc = glGetUniformLocation(shader, "color");

	Render::uniforms.push_back({ colorLoc,"color" });


	float timer = 0;
	

	Render::activateSkybox();

	bool show_demo_window = false;
	bool show_another_window = true;
	bool other_window = true;
	bool show_window = true;
	bool last_window = true;

	struct scene {
		std::vector<Entity*> entityData;
		std::vector<unsigned int> textures;
	};

	static scene sceneData;
	sceneData.textures = { texture0,texture1,texture2,texture3,texture4 };


	ImGuiWindowFlags window_flags = 0;

	//Uncomment for other window options
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	show_window = NULL;


	//Frame buffer and rendering is here
	Render::prepareFramebuffer();




	//TimeDeltaTime uses
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	//main run loop
	while (!glfwWindowShouldClose(window)) {
		auto t1 =std::chrono::high_resolution_clock::now();
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		camera.speed = camera.baseSpeed * deltaTime;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, Render::fbo);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Render::draw(Render::skybox);

		
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		//ActivateDockingSpace(&yes);
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		{
			static float f = 10.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!", &show_window, window_flags);

			ImGui::Text("Entities: %d",entities.size());
			ImGui::Checkbox("Components Window", &other_window);
			ImGui::Checkbox("Entities Window", &show_another_window);

			ImGui::SliderFloat("Camera Speed", &f, 0.0f, 20.0f);

			if (ImGui::Button("Create Entity")) {
				for (int i = 0; i < 1; i++) {
				Entity* e3 = new Entity();
				entities.push_back(e3);
				sceneData.entityData.push_back(e3);

				}
			}

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();

			camera.baseSpeed = f;
		}
		static bool selection[10]{};
		static int selected = -1;
		if (show_another_window)
		{
			ImGui::Begin("Entities",&show_another_window,window_flags); 
			
			if (ImGui::TreeNode("Entities"))
			{
				for (int n = 0; n < entities.size(); n++)
				{
					char buf[32];
					sprintf_s(buf, "EntityID: %d", entities.at(n)->getId());
					if (ImGui::Selectable(buf, selected == n))
						selected = n;
				}
				ImGui::TreePop();
			}
			ImGui::End();
		}
		//framebuffer window
		if (last_window)
		{
			ImGui::Begin("Scene");
			{
				float width = ImGui::GetContentRegionAvail().x;
				float height = ImGui::GetContentRegionAvail().y;
				resizeSceneWindow(width, height);

				ImGui::Image(
					(ImTextureID)Render::colorTexture,
					ImGui::GetContentRegionAvail(),
					ImVec2(0, 1),
					ImVec2(1, 0)
				);
			}
			ImGui::End();
		}


		bool inputs_step = true;
		const float   f32_zero = 0.f, f32_one = 1.f, f32_lo_a = -10000000000.0f, f32_hi_a = +10000000000.0f;
		static ImGuiSliderFlags flags = ImGuiSliderFlags_None;
		if (other_window)
		{
			ImGui::Begin("Data", &other_window, window_flags);
			if (selected != -1) {
				//struct 
				std::string str = "EntityId: " + std::to_string(entities.at(selected)->getId());
				ImGui::Text(str.c_str());
				ImGui::Text("Components: ");
				Entity* et = sceneData.entityData.at(selected);
				if (et->hasComponent<TransformComponent>()) {
					if (ImGui::TreeNode(std::string("Transform").c_str()))
					{
						ImGui::Text("Translate");
						ImGui::DragFloat("X", &(et->getComponent<TransformComponent>()->translate.x), -0.01f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
						ImGui::DragFloat("Y", &(et->getComponent<TransformComponent>()->translate.y), 0.01f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
						ImGui::DragFloat("Z", &(et->getComponent<TransformComponent>()->translate.z), -0.01f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
						ImGui::Text("Rotate");
						ImGui::DragFloat("X ", &(et->getComponent<TransformComponent>()->rotate.x), 0.01f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
						ImGui::DragFloat("Y ", &(et->getComponent<TransformComponent>()->rotate.y), 0.01f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
						ImGui::DragFloat("Z ", &(et->getComponent<TransformComponent>()->rotate.z), 0.01f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
						ImGui::Text("Scale");
						ImGui::DragFloat("X  ", &(et->getComponent<TransformComponent>()->scale.x), 0.01f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
						ImGui::DragFloat("Y  ", &(et->getComponent<TransformComponent>()->scale.y), 0.01f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
						ImGui::DragFloat("Z  ", &(et->getComponent<TransformComponent>()->scale.z), 0.01f, -FLT_MAX, +FLT_MAX, "%.3f", flags);
						if (ImGui::Button("Remove Transform")) {
							et->removeComponent<TransformComponent>();
						}
						ImGui::TreePop();

					}
					
				}
				if (et->hasComponent<TextureComponent>()) {
					if (ImGui::TreeNode(std::string("Texture").c_str()))
					{
						int temp = et->getComponent<TextureComponent>()->texture;
						for (int n = 0; n < sceneData.textures.size(); n++)
						{
							char buf[32];
							sprintf_s(buf, "Texture: %d", sceneData.textures.at(n));
							if (ImGui::Selectable(buf, temp == sceneData.textures.at(n)))
								temp = sceneData.textures.at(n);
						}
						et->getComponent<TextureComponent>()->texture = temp;
						if (ImGui::Button("Remove Texture")) {
							et->removeComponent<TextureComponent>();
						}
						ImGui::TreePop();

					}

				}
				if (et->hasComponent<ColorComponent>()) {
					if (ImGui::TreeNode(std::string("Color").c_str()))
					{
						glm::vec3 temp = et->getComponent<ColorComponent>()->color;
						ImVec4 temp2 = ImVec4(temp.x,temp.y,temp.z, 1.00f);
						ImGui::ColorEdit3("clear color", (float*)&temp2);
						temp.x = temp2.x;
						temp.y = temp2.y;
						temp.z = temp2.z;
						et->getComponent<ColorComponent>()->color = temp;
						if (ImGui::Button("Remove Color")) {
							et->removeComponent<ColorComponent> ();
						}
						ImGui::TreePop();

					}

				}
				ImGui::NewLine();
				if (ImGui::TreeNode(std::string("Add Other Components").c_str()))
				{
					if (!et->hasComponent<TransformComponent>()) {
						if (ImGui::Button("Transform")) {
							et->addComponent<TransformComponent>(glm::vec3(0,0,0));
						}
					}
					if (!et->hasComponent<TextureComponent>()) {
						if (ImGui::Button("Texture")) {
							et->addComponent<TextureComponent>(0);
						}
					}
					if (!et->hasComponent<ColorComponent>()) {
						if (ImGui::Button("Color")) {
							et->addComponent<ColorComponent>(glm::vec3(1,1,1));
						}
					}
					ImGui::TreePop();

				}
				ImGui::NewLine();
				ImGui::NewLine();
				if (ImGui::Button(std::string("Delete Entity").c_str()))
				{
					sceneData.entityData.erase(sceneData.entityData.begin() + selected);
					entities.erase(entities.begin()+selected);
					selected =- 1;

				}

				
			}
			else {
				ImGui::Text("No Entity Selected");
			};
			ImGui::End();
		}

		//Adjust camera position
		camera.translate(left, right, up, down,forward,backward);

		view = glm::lookAt(camera.cameraPos, camera.cameraPos + camera.cameraFront, camera.cameraUp);


		projection = glm::perspective(glm::radians(camera.fov), (float)Render::windowWidth/(float)Render::windowHeight, 0.01f, 1000.0f);


		//Send camera data to shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));


		for (Entity* en : entities) {
			Render::draw(en);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		
		glfwPollEvents();
		auto t2 = std::chrono::high_resolution_clock::now();
		auto ms_init = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
		
	}

	//Macro for making program memory safe
	clearMemory;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	for (Entity* ent : entities) {
		delete ent;
	}

}
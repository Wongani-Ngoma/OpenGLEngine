#include "Engine.h"

namespace Engine {
	float deltaTime;
	float lastFrame = 0.0f;

	unsigned int shadowMapWidth = 1024, shadowMapHeight = 1024;
	unsigned int shadowMapFBO;
	unsigned int shadowMap;

	unsigned int pointShadowMapFBO;
	unsigned int depthCubemap;

	int renderShadows = 1;

	float farPlane = 75.0;

	bool _editorMode = false;
}

void Engine::init() {

	BackEnd::init();
	Physics::init();
	Renderer::init();
	PostProcess::init();
	Game::init();
}

void Engine::run() {

	glGenFramebuffers(1, &shadowMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);

	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float clampColor[] = {1.0, 1.0, 1.0, 1.0};
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glm::mat4 orthgonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, farPlane);
	glm::mat4 lightView = glm::lookAt(glm::vec3(5.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 lightSpaceMatrix = orthgonalProjection * lightView;

	
	Shader& shadowMapShader = Renderer::getShaderByName("shadowMapShader");
	shadowMapShader.activate();
	shadowMapShader.setMat4("lightProjection", lightSpaceMatrix);
	
	////////////////////////////////////////////////////////////
	// point light stuff
	glm::mat4 perspectiveProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 75.0f);
	glm::mat4 spotLightView = glm::lookAt(glm::vec3(0.0, 5.0, 0.0), glm::vec3(0.0), glm::vec3(0.0, 0.0, 1.0));
	glm::mat4 spotLightProjection = perspectiveProjection * spotLightView;


	glGenFramebuffers(1, &pointShadowMapFBO);
	glGenTextures(1, &depthCubemap);

	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, pointShadowMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glm::vec3 pointLightPos(5.0);

	//matrices for all 6 faces
	glm::mat4 shadowProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 75.0f);
	glm::mat4 shadowTransforms[] = {
		shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
		shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
		shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)),
		shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)),
		shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)),
		shadowProjection * glm::lookAt(pointLightPos, pointLightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)),
	};

	Shader& shadowCubeMapShader = Renderer::getShaderByName("shadowCubeMapShader");
	shadowCubeMapShader.activate();
	for (int i = 0; i < 6; i++) {
		shadowCubeMapShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
	}
	shadowCubeMapShader.set3Float("lightPos", pointLightPos);
	shadowCubeMapShader.setFloat("farPlane", 1000.0);

	unsigned int counter = 0;

	// for some fucked reason if theres a condition that makes the loop run at 60fps physics simulations slow down
	// this can be fixed by using 1 / 30 as the physics timestep and run the engineat 120fps
	// todo: find a way to make this runat 60fps without making physics simulatons slow
	// this issue didint exist before redesigning to engine
	while(!BackEnd::shouldWindowClose()) {
		double currentTime = glfwGetTime();
		deltaTime = currentTime - lastFrame;
		counter++;

		Engine::processInput();

		// step physics
		Physics::stepPhysics(1.0 / 60.0);

		//////////////////////////////////////
		////// IMPORTANT /////////////////////
		// Process input should be called before game update, maily cause fuckng player view camera needs to  it's view angles updated (which happens in it's proces input method) before position update, if it doestn hapenin in this order motion is jaggerd
		// even user input should run periodically (60hz in this case), or else bad things will happen
		//////////////////////////////////////
		if (!_editorMode) {
			Game::processInput(1.0f / 60.0f);
		} else {
			Editor::processInput(Game::getActiveCamera());
		}

		Game::updateGameObjects();

		if(renderShadows) Renderer::getShaderByName("mainShader").setInt("renderShadows", 1);

		if (renderShadows) {
			Game::shadowRenderPass();
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, BackEnd::getScreenWidth(), BackEnd::getScreenHeight());
		// set the framebuffer in PostProvess.cpp to be the active one so that all opengl calls should operate on
		// that framebuffer
		PostProcess::setActiveFrameBuffer();

		BackEnd::update();

		if (renderShadows) {
			Shader& mainShader = Renderer::getShaderByName("mainShader");
			mainShader.activate();
			mainShader.setMat4("lightProjection", lightSpaceMatrix);
			mainShader.setFloat("farPlane", farPlane);


			glActiveTexture(GL_TEXTURE0 + 2); // Bind to third uint cause the first two are taken by diffuse and specular maps
			glBindTexture(GL_TEXTURE_2D, shadowMap);
			Renderer::getShaderByName("mainShader").setInt("shadowMap", 2);

			/*
			glActiveTexture(GL_TEXTURE0 + 2); // Bind to third uint cause the first two are taken by diffuse and specular maps  
			glBindTexture(GL_TEXTURE_CUBE_MAP, Renderer::getCubeMap());
			mainShader.setInt("shadowCubeMap", 2);
			*/
		}

		Game::renderPass();

		PostProcess::renderPostProcessedScene();

		Game::setTitle(deltaTime, counter);

		BackEnd::newFrame();

		Mouse::resetOffsets();
		glfwPollEvents();
	}
}

void Engine::processInput() {
	if (Keyboard::key(GLFW_KEY_LEFT_ALT) && Keyboard::keyWentDown(GLFW_KEY_ENTER)) {
		if (BackEnd::isWindowed()) {
			BackEnd::toogleFullScreen();
		}
		else {
			BackEnd::toogleWindowed();
		}
		PostProcess::updateFramebuffer();
	}

	if (Keyboard::keyWentDown(GLFW_KEY_E)) {
		if (!_editorMode) {
			glfwSetInputMode(BackEnd::getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			_editorMode = true;
		} else {
			glfwSetInputMode(BackEnd::getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			_editorMode = false;
		}
	}
	
	if (_editorMode) {
		Editor::processInput(Game::getActiveCamera());
	}

}

void Engine::shutdown() {
	AssetManager::cleanup();
	PostProcess::cleanup();
	glfwTerminate();
}

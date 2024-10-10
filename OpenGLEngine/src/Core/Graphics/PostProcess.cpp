#include "PostProcess.h"

namespace PostProcess {
	// vertices of rect where post processes scene is rendered as a texture
	float rectangleVertices[] = {
		//coords     texCoords
		-1.0,  1.0,  0.0, 1.0,
		-1.0, -1.0,  0.0, 0.0,
		 1.0, -1.0,  1.0, 0.0,

		-1.0,  1.0,  0.0, 1.0,
		 1.0, -1.0,  1.0, 0.0,
		 1.0,  1.0,  1.0, 1.0
	};

	// buffers to hold actual rect
	unsigned int rectVAO, rectVBO;

	// custom framebuffer to where the scene if drawn to before post processing and presentation to screen
	FrameBuffer fbo;
}

void PostProcess::init() {
	// setup rect
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// setup framebuffer
	fbo.create(BackEnd::getScreenWidth(), BackEnd::getScreenHeight());
	fbo.createColorAttachment("ca1", GL_RGB);
	fbo.createDepthAttachment();
	fbo.checkErrors();
}

void PostProcess::cleanup() {
	fbo.cleanUp();
	glDeleteBuffers(1, &rectVBO);
	glDeleteVertexArrays(1, &rectVAO);
}

void PostProcess::updateFramebuffer() {
	fbo = FrameBuffer();
	fbo.create(BackEnd::getScreenWidth(), BackEnd::getScreenHeight());
	fbo.createColorAttachment("ca1", GL_RGB);
	fbo.createDepthAttachment();
	fbo.checkErrors();
}

void PostProcess::setActiveFrameBuffer() {
	fbo.Bind(GL_FRAMEBUFFER);
}

void PostProcess::renderPostProcessedScene() {
	glDisable(GL_DEPTH_TEST); // The post-processed scene should be drawn in front of everything else
	glDisable(GL_CULL_FACE); // face culling sometimes fucks with the post-porcessed scene rect 

	// bind back to the default framebuffer, all the rendering done earier was done to the custom framebuffer
	// the rect will now be drawn to the default framebuffer using the custom framebuffer as a texture
	// and it will use the post procesing shader program to add effects
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Renderer::getShaderByName("framebufferShader").activate();
	glBindVertexArray(rectVAO);
	fbo.bindColorAttachment("ca1");
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

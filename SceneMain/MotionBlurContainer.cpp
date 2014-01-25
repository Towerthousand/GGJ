#include "MotionBlurContainer.hpp"

MotionBlurContainer::MotionBlurContainer() {
	target1 = new RenderTarget();
	target1->addTexture(RenderTarget::COLOR0, Texture::RGBA8);
	target1->build();
	target1->getTextureForAttachment(RenderTarget::COLOR0)->setFilter(GL_NEAREST, GL_NEAREST);
	target1->getTextureForAttachment(RenderTarget::COLOR0)->setWrap(GL_CLAMP_TO_EDGE);

	target2 = new RenderTarget();
	target2->addTexture(RenderTarget::COLOR0, Texture::RGBA8);
	target2->build();
	target2->getTextureForAttachment(RenderTarget::COLOR0)->setFilter(GL_LINEAR, GL_LINEAR);
	target2->getTextureForAttachment(RenderTarget::COLOR0)->setWrap(GL_CLAMP_TO_EDGE);

	quad.mesh = Meshes.get("quad");
	quad.program = Programs.get("motionBlurPass");
}

MotionBlurContainer::~MotionBlurContainer() {
	delete target1;
	delete target2;
}


void MotionBlurContainer::draw() const {
	RenderTarget* screen = RenderTarget::getCurrent();
	RenderTarget::bind(target1);

	ContainerObject::draw();

	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//BLUR + SCENE
	RenderTarget::bind(target1);
//	glClear(GL_COLOR_BUFFER_BIT);
	quad.program = Programs.get("motionBlurPass");

	mat4f mat = mat4f(1.0f);
	float t = GLOBALCLOCK.getElapsedTime().asSeconds();
	std::cout << "pene" << std::endl;
	mat = mat4f(1.0f);
	mat = glm::rotate(mat, 30.0f*t, vec3f(0, 0, 1));
	//mat = glm::translate(mat, vec3f(0.2*sin(t*2.42), 0.2*sin(t*3.125), 0));
	quad.program->uniform("MVP")->set(mat);
	quad.program->uniform("alpha")->set(0.6f);
	quad.program->uniform("tex1")->set(target2->getTextureForAttachment(RenderTarget::COLOR0));
	quad.draw();

	mat = mat4f(1.0f);
	mat = glm::rotate(mat, -25.2345623f*t, vec3f(0, 0, 1));
	//mat = glm::translate(mat, vec3f(0.1*sin(t*4), 0.1*sin(t*2.325), 0));
	quad.program->uniform("MVP")->set(mat);
	quad.program->uniform("alpha")->set(0.6f);
	quad.program->uniform("tex1")->set(target2->getTextureForAttachment(RenderTarget::COLOR0));
	quad.draw();

	mat = mat4f(1.0f);
	mat = glm::rotate(mat, 40.0f*float(sin(t)), vec3f(0, 0, 1));
	RenderTarget::bind(screen);
	glClear(GL_COLOR_BUFFER_BIT);
	quad.program = Programs.get("motionBlurPass");
	quad.program->uniform("MVP")->set(mat);
	quad.program->uniform("alpha")->set(1.0f);
	quad.program->uniform("tex1")->set(target1->getTextureForAttachment(RenderTarget::COLOR0));
	quad.draw();

	RenderTarget* aux = target1;
	target1 = target2;
	target2 = aux;
}

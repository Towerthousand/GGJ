#include "LightParticleEmitter.hpp"
#include "ParticleEmitter.hpp"

LightParticleEmitter::LightParticleEmitter(vec3f col) : ParticleEmitter(1000), col(col) {
}

Particle LightParticleEmitter::makeParticle(float frameTimePassed, float deltaTime, vec3f position) {
	(void) frameTimePassed;
	(void) deltaTime;
	vec3f vel = glm::sphericalRand(3.0f);
	Particle pt;
	pt.life = 2;
	pt.startSize = 0.1;
    pt.endSize = 0;
	pt.startCol = vec4f(col, 0.7);
	pt.endCol = vec4f(col, 0);
	pt.v = vec3f(0.1);//glm::length(position-oldWorldPos) > 0? glm::normalize(position - oldWorldPos )*-2.0f + vel:vel;
	pt.p = position + (glm::length(position-oldWorldPos) > 0? (position - oldWorldPos)*2.0f:vec3f(0.0f));
	//pt.a = glm::sphericalRand(10.0f);
	pt.texIndex = 1;
	return pt;
}

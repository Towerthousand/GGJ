#include "LightParticleEmitter.hpp"
#include "ParticleEmitter.hpp"

LightParticleEmitter::LightParticleEmitter(vec3f col) : ParticleEmitter(5), col(col) {
}

Particle LightParticleEmitter::makeParticle(float frameTimePassed, float deltaTime, vec3f position) {
	(void) frameTimePassed;
	(void) deltaTime;
    //vec3f vel = glm::sphericalRand(3.0f);
    vec3f vel = vec3f(0,3.0,0);
	Particle pt;
    pt.life = 1;// + (rand()%100)/99.0f;
    pt.startSize = 0.08 + ((rand()%4)/100.0f);
    pt.endSize = 0;
    pt.startCol = vec4f(col, 0.7);
    pt.endCol = vec4f(col, 0.7);
    pt.v = vel;
    pt.p = position;
    pt.a = vec3f(0,-9.8,0);
	pt.texIndex = 1;
	return pt;
}

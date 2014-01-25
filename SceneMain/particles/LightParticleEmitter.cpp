#include "LightParticleEmitter.hpp"
#include "ParticleEmitter.hpp"

LightParticleEmitter::LightParticleEmitter(vec3f col) : ParticleEmitter(20), col(col) {
}

Particle LightParticleEmitter::makeParticle(float frameTimePassed, float deltaTime, vec3f position) {
	(void) frameTimePassed;
	(void) deltaTime;
    //vec3f vel = glm::sphericalRand(3.0f);
	vec3f vel = vec3f(0,1.0,0);
	Particle pt;
	pt.life = 0.4;// + (rand()%100)/99.0f;
	if(rand()%2 == 0) pt.life = 0.00001;
	pt.startSize = 0.01 + ((rand()%4)/200.0f);
	pt.endSize = pt.startSize/2;
    pt.startCol = vec4f(col, 0.7);
    pt.endCol = vec4f(col, 0.7);
    pt.v = vel;
	pt.p = position-vec3f(0,0.1,0);
    pt.a = vec3f(0,-9.8,0);
	pt.texIndex = 1;
	return pt;
}

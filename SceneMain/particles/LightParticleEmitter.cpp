#include "LightParticleEmitter.hpp"
#include "ParticleEmitter.hpp"
#include "ParticleSystem.hpp"

LightParticleEmitter::LightParticleEmitter(vec3f col, int index) : ParticleEmitter(20), col(col), index(index) {
}

Particle LightParticleEmitter::makeParticle(float frameTimePassed, float deltaTime, vec3f position) {
	(void) frameTimePassed;
	(void) deltaTime;

    //vec3f vel = glm::sphericalRand(3.0f);
	vec3f vel = vec3f(0,1.0,0);
	Particle pt;
	pt.life = 0.3;// + (rand()%100)/99.0f;
	if(rand()%2 == 0) pt.life = 0.00001;
	pt.startSize = randomFloat(0.02, 0.05);
	pt.endSize = pt.startSize*0.8;
	pt.startCol = vec4f(col, 1.0);
	pt.endCol = vec4f(col, 0.7);
	pt.v = vel;
	pt.p = position-vec3f(0,0.1,0);
    pt.a = vec3f(0,-9.8,0);
	pt.texIndex = index;
	return pt;
}

void LightParticleEmitter::boom(int count)
{
	vec3f ps = currWorldPos;
	ps.y = floor(ps.y)+0.15;
	for(int i = 0; i < count; i++)
	{
        //vec3f vel = glm::sphericalRand(3.0f);
		Particle pt;
        pt.life = 0.3;
		pt.startSize = randomFloat(0.02, 0.1);
		pt.endSize = pt.startSize*0.3;
		pt.startCol = vec4f(col, 1.0);
		pt.endCol = vec4f(col, 0.7);
		vec2f a = glm::circularRand(1.0f)*randomFloat(0.5, 1.5);
        pt.v = 1.1f*vec3f(a.x, 0, a.y);
		pt.p = ps+vec3f(0, -0.1, 0);
		pt.a = vec3f(0,0,0);
		pt.texIndex = index;
		sys->addParticle(pt);
	}
}

void LightParticleEmitter::boomSide(int count, float vx)
{
	for(int i = 0; i < count; i++)
	{
        //vec3f vel = glm::sphericalRand(3.0f);
		Particle pt;
        pt.life = 0.4f;
		pt.startSize = randomFloat(0.02, 0.1);
		pt.endSize = pt.startSize*0.3;
		pt.startCol = vec4f(col, 1.0);
		pt.endCol = vec4f(col, 0.7);
		vec2f a = glm::circularRand(1.0f)*randomFloat(0.5, 1.5);
		pt.v = vec3f(vx*0.3, a.x, a.y);
		pt.p = currWorldPos+vec3f(0, 0, 0);
		pt.a = vec3f(0,-8,0);
		pt.texIndex = index;
		sys->addParticle(pt);
	}
}

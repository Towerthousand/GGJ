#ifndef COMMONS_HPP
#define COMMONS_HPP
#include "VBE/includes.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/noise.hpp"
#include <queue>
#include <thread>
#include <mutex>
#include <SFML/Network.hpp>

constexpr int CHUNKSIZE_POW2 = 4;
constexpr int CHUNKSIZE = int(pow(2,CHUNKSIZE_POW2)); //in voxels
constexpr int CHUNKSIZE_MASK = CHUNKSIZE-1;
constexpr int WORLDSIZE = 16; //in chunks
constexpr int WORLDSIZE_MASK = WORLDSIZE-1;
constexpr int MINLIGHT = 3;
constexpr int MAXLIGHT = 16;
constexpr int TEXSIZE = 8;

std::string toString(float n);
std::string toString(vec3f n);


inline float randomFloat(float min, float max)
{
    float f = (float(rand()) / RAND_MAX);
    return min + f*(max-min);
}

inline int randomInt(int min, int max)
{
	if(max <= min) return max;

	return min + rand() % (max-min+1);
}


#endif // COMMONS_HPP

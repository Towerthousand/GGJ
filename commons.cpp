#include "commons.hpp"


std::string toString(float n)
{
    std::stringstream ss;
    ss << n;
    return ss.str();
}

std::string toString(vec3f n)
{
    return toString(n.x) + " " + toString(n.y) + " " + toString(n.z);
}


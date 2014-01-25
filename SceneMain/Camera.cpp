#include "Camera.hpp"
#include "Player.hpp"

Camera::Camera(const std::string& cameraName, const vec3f& pos, const vec3f& rot)
    : pos(pos), rot(rot), projection(1.0f), view(1.0f) {
    this->setName(cameraName);
    this->setUpdatePriority(2);
}

Camera::~Camera() {
}

void Camera::update(float deltaTime) {

	vec3f newPos = targetPlayer->pos + vec3f(0, 0, 8);
	float fac = exp(-deltaTime*20);
	pos = pos*fac+newPos*(1-fac);
	lookPos = targetPlayer->pos;

	transform = glm::translate(mat4f(1.0f),pos);
	view = glm::lookAt(pos, lookPos, vec3f(0, 1, 0));
	frustum.calculate(projection*view);
}

vec3f Camera::getWorldPos() const {
    return vec3f(fullTransform*vec4f(0,0,0,1));
}

vec3f Camera::getForward() const {
    mat4f m = view*fullTransform;
    return -glm::normalize(vec3f(m[0][2],m[1][2],m[2][2]));
}

const Frustum&Camera::getFrustum() const {
    return frustum;
}

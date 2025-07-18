#include "camera.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

void Camera::update(float deltaTime)
{
	glm::vec3 dir{ 0.f };
	if (moveForward) dir.z -= 1.f;
	if (moveBackward) dir.z += 1.f;
	if (moveLeft) dir.x -= 1.f;
	if (moveRight) dir.x += 1.f;

	if (glm::length2(dir) > 0.f)
	{
		dir = glm::normalize(dir) * movementSpeed * deltaTime;
		glm::mat4 cameraRotation = getRotationMatrix();
		position += glm::vec3(cameraRotation * glm::vec4(dir, 0.f));
	}
	
}

void Camera::clampPitch()
{
	if (pitch > maxPitchAngle) pitch = maxPitchAngle;
	if (pitch < -maxPitchAngle) pitch = -maxPitchAngle;
}

void Camera::processSDLEvent(SDL_Event& e)
{
    if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) {
        bool down = (e.type == SDL_MOUSEBUTTONDOWN);
        if (e.button.button == SDL_BUTTON_LEFT)  m_leftMouseDown = down;
        if (e.button.button == SDL_BUTTON_RIGHT) m_rightMouseDown = down;
    }

    if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP) {
        bool isDown = (e.type == SDL_KEYDOWN);
        switch (e.key.keysym.sym) {
        case SDLK_w: moveForward = isDown; break;
        case SDLK_s: moveBackward = isDown; break;
        case SDLK_a: moveLeft = isDown; break;
        case SDLK_d: moveRight = isDown; break;
        }
    }

    if (e.type == SDL_MOUSEMOTION) {
        if (m_leftMouseDown) {
           yaw += e.motion.xrel * mouseSensitivity;
           clampPitch();
		}
        if (m_rightMouseDown) {
            yaw += e.motion.xrel * mouseSensitivity;
            pitch -= e.motion.yrel * mouseSensitivity;
            clampPitch();
        }
    }



    if (e.type == SDL_MOUSEWHEEL && m_rightMouseDown) {
        float delta = e.wheel.y * 0.5f;    
        movementSpeed = glm::clamp(movementSpeed + delta,
            minSpeed, maxSpeed);
    }
}


void Camera::resetInput() {
    moveForward = moveBackward = moveLeft = moveRight = false;
    m_leftMouseDown = m_rightMouseDown = false;
}


glm::mat4 Camera::getViewMatrix() const
{
	glm::mat4 cameraTranslation = glm::translate(glm::mat4(1.f), position);
	glm::mat4 cameraRotation = getRotationMatrix();
	return glm::inverse(cameraTranslation * cameraRotation);
}

glm::mat4 Camera::getRotationMatrix() const
{
	glm::quat qPitch = glm::angleAxis(pitch, glm::vec3{ 1.f, 0.f, 0.f });
	glm::quat qYaw = glm::angleAxis(yaw, glm::vec3{ 0.f, -1.f, 0.f });

	return glm::toMat4(qYaw * qPitch);

}

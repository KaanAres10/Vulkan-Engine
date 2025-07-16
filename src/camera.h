
#include <vk_types.h>
#include <SDL_events.h>
#include <glm/glm.hpp>

class Camera {
public:
	glm::vec3 position;

	// vertical rotation
	float pitch{ 0.f };
	
	// horizontal rotation
	float yaw{ 0.f };

	float movementSpeed = 5.f;
	float mouseSensitivity = 0.0002f;
	float maxPitchAngle = glm::radians(89.f);

	bool moveForward = false;
	bool moveBackward = false;
	bool moveLeft = false;
	bool moveRight = false;


	glm::mat4 getViewMatrix() const;
	glm::mat4 getRotationMatrix() const;

	void processSDLEvent(SDL_Event& e);

	void update(float deltaTime);

	void clampPitch();
};

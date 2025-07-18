
#include <vk_types.h>
#include <SDL_events.h>
#include <glm/glm.hpp>

class Camera {
public:
	glm::vec3 position;

	bool m_leftMouseDown = false;
	bool m_rightMouseDown = false;

	// vertical rotation
	float pitch{ 0.f };
	
	// horizontal rotation
	float yaw{ 0.f };

	float movementSpeed = 5.f;
	float mouseSensitivity = 0.002f;
	float maxPitchAngle = glm::radians(89.f);

	bool moveForward = false;
	bool moveBackward = false;
	bool moveLeft = false;
	bool moveRight = false;


	float minSpeed = 0.1f;   
	float maxSpeed = 50.f;

	void resetInput();


	glm::mat4 getViewMatrix() const;
	glm::mat4 getRotationMatrix() const;

	void processSDLEvent(SDL_Event& e);

	void update(float deltaTime);

	void clampPitch();
};

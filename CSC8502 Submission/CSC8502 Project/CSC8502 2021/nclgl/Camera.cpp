#include "Camera.h"
#include "Window.h"
#include <algorithm>

void Camera::UpdateCamera(float dt) {
	pitch -= (Window::GetMouse()->GetRelativePosition().y);
	yaw -= (Window::GetMouse()->GetRelativePosition().x);

	pitch = std::min(pitch, 90.0f);
	pitch = std::max(pitch, -90.0f);

	if (yaw < 0) {
		yaw += 360.0f;
	}

	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

	Matrix4 rotation = Matrix4::Rotation(yaw, Vector3(0, 1, 0));

	Vector3 forward = rotation * Vector3(0, 0, -1);
	Vector3 right = rotation * Vector3(1, 0, 0);

	float speed = 2000.0f * dt;

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_C)) {
		automated = true;
		position = heightmapSize * Vector3(1.0f, 2.0f, 1.0f);
		yaw = 0;
		pitch = -20;
		zneg = true;
		zpos = false;
		xpos = false;
		xneg = false;
	}
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_V)) {
		automated = false;
	}

	if (!automated) {
		if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
			position += forward * speed;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
			position -= forward * speed;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
			position -= right * speed;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
			position += right * speed;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
			position.y += speed;
		}

		if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
			position.y -= speed;
		}
	}
	else {
		if (zneg) {
			position += Vector3(0, 0, -1) * speed;
			if (position.z < 0) {
				position.z += 10.0f;
				xneg = true;
				zneg = false;
			}
		}
		if (zpos) {
			position += Vector3(0, 0, 1) * speed;
			if (position.z > heightmapSize.z) {
				position.z -= 10.0f;
				xpos = true;
				zpos = false;
			}
		}
		if (xneg) {
			position += Vector3(-1, 0, 0) * speed;
			if (position.x < 0) {
				position.x += 10.0f;
				zpos = true;
				xneg = false;
			}
		}
		if (xpos) {
			position += Vector3(1, 0, 0) * speed;
			if (position.x > heightmapSize.x) {
				position.x -= 10.0f;
				zneg = true;
				xpos = false;
			}
		}
		yaw += 0.193;
	}
	std::cout << position.x << " " << position.z << std::endl;
}

Matrix4 Camera::BuildViewMatrix() {
	return		Matrix4::Rotation(-pitch, Vector3(1, 0, 0)) *
		Matrix4::Rotation(-yaw, Vector3(0, 1, 0)) *
		Matrix4::Translation(-position);
};
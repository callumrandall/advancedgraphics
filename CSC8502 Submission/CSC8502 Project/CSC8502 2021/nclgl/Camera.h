#pragma once
#include "Matrix4.h"
#include "Vector3.h"

class Camera {
public:
	Camera(void) {
		yaw = 0.0f;
		pitch = 0.0f;
		position = heightmapSize * Vector3(1.0f, 2.0f, 1.0f);
		automated = false;
		zneg = false;
		zpos = false;
		xneg = false;
		xpos = false;
	};

	Camera(float pitch, float yaw, Vector3 position) {
		this->pitch = pitch;
		this->yaw = yaw;
		this->position = position;
		automated = false;
		zneg = false;
		zpos = false;
		xneg = false;
		xpos = false;
	}

	~Camera(void) {};

	void UpdateCamera(float dt = 1.0f);

	Matrix4 BuildViewMatrix();

	Vector3 GetPosition() const { return position; }

	void SetPosition(Vector3 val) { position = val; }

	float GetYaw() const { return yaw; }

	void SetYaw(float y) { yaw = y; }

	float GetPitch() const { return pitch; }

	void SetPitch(float p) { pitch = p; }

	bool automated;

	bool zneg;
	bool zpos;
	bool xneg;
	bool xpos;

	Vector3 heightmapSize;
protected:
	float yaw;
	float pitch;
	Vector3 position; // Set to 0 ,0 ,0 by Vector3 constructor ;)
};

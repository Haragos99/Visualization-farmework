#pragma once
#include "algebra.h"
#define M_PI       3.14159265358979323846   // pi
class Camera {
	vec3 wEye, wLookat, wVup;
	vec3 wEyePrev, wLookatPrev, wVupPrev;
	float fov, asp, fp, bp;
public:
	Camera(){}
	Camera(int width, int height) : wEye(0.0, 0.0, 2.0), wLookat(0.0, 0.0, 0.0),
		wVup(0.0, 1.0, 0.0)
	{
		asp = (float)width / height;
		fov = 50.0f * (float)M_PI / 180.0f;
		fp = 0.1; bp = 10;
	}
	mat4 V();
	mat4 P();

	void SetAspectRatio(float r) { asp = r; }

	void rotateAroundLookAt(const vec3& eulerAngle);
};
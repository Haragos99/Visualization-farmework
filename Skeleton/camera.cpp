#include "camera.h"


mat4 Camera::P() {
	return mat4(
		1 / (tan(fov / 2) * asp), 0, 0, 0,
		0, 1 / tan(fov / 2), 0, 0,
		0, 0, -(fp + bp) / (bp - fp), -1,
		0, 0, -2 * fp * bp / (bp - fp), 0);
}


mat4 Camera::V() {
	vec3 w = normalize(wEye - wLookat);
	vec3 u = normalize(cross(wVup, w));
	vec3 v = cross(w, u);
	return TranslateMatrix(wEye * (-1)) * mat4(u.x, v.x, w.x, 0,
		u.y, v.y, w.y, 0,
		u.z, v.z, w.z, 0,
		0, 0, 0, 1);
}

void Camera::rotateAroundLookAt(const vec3& eulerAngle) {
	vec3 w = normalize(wEye - wLookat);
	vec3 u = normalize(cross(wVup, w));
	mat4 rotX = RotationMatrix(eulerAngle.x, u);	// Horizontal
	mat4 rotY = RotationMatrix(eulerAngle.y, wVup);	// Vertical
	vec3 lEye = wEye - wLookat;

	// Rotate around vertical axis (Safe from  gimbal lock):
	vec4 lEye4 = vec4{ lEye.x, lEye.y, lEye.z, 1.0f } *rotY;
	lEye = vec3{ lEye4.x, lEye4.y, lEye4.z };
	wEye = lEye + wLookat;

	// Rotate around horizontal axis (Possible gimbal lock):
	lEye4 = vec4{ lEye.x, lEye.y, lEye.z, 1.0f } *rotX;
	lEye = vec3{ lEye4.x, lEye4.y, lEye4.z };
	if (fabsf(dot(normalize(lEye), normalize(wVup))) < 0.95f) {	// Update eye position only if it is safe from gimbal lock
		wEye = lEye + wLookat;
	}
}


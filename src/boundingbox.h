#pragma once
#include <GL/glew.h>
#include<math.h>
class BoundingBox {
	unsigned int vao;
	float x, y, z;
public:
	BoundingBox(float x, float y, float z);
	void Render();
};
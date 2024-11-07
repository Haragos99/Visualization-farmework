#pragma once
#include <GL/glew.h>
#include "algebra.h"
#include "GPUProgram.h"
#define M_PI       3.14159265358979323846   // pi

class Texture3D
{
	unsigned int textureId;
	long sx, sy, sz;
	float Rho(float r) { return cos(M_PI * 12.0 * cos(M_PI * r / 2.0)); }
	float MarschnerLobb(float x, float y, float z) {
		return (1.0 - sin(M_PI * z / 2.0) + (1.0 + Rho(sqrt(x * x + y * y))) / 4.0) / 2.5;
	}
public:
	Texture3D(int sx, int sy, int sz);

	void Bind(GPUProgram& gpushader);

};

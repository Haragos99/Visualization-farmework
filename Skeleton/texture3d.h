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
	Texture3D(int sx, int sy, int sz) : sx(sx), sy(sy), sz(sz)
	{
		float* data = new float[sx * sy * sz]; if (!data) return;
		for (int k = 0; k < sz; k++)
			for (int j = 0; j < sy; j++)
				for (int i = 0; i < sx; i++)
				{
					float x = 2.0 * (float)i / (sx - 1) - 1.0;
					float y = 2.0 * (float)j / (sy - 1) - 1.0;
					float z = 2.0 * (float)k / (sz - 1) - 1.0;
					long index = sx * sy * k + sx * j + i;
					data[index] = MarschnerLobb(x, y, z);
				}
		glGenTextures(1, &textureId); glBindTexture(GL_TEXTURE_3D, textureId);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, sx, sy, sz, 0, GL_RED, GL_FLOAT, data);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		delete data;
	}


	void Bind(GPUProgram& gpushader)
	{
		unsigned int shader = gpushader.getId();
		int samplerUnit = 1;
		int location = glGetUniformLocation(shader, "samplerUnitVolume");
		glUniform1i(location, samplerUnit);
		glActiveTexture(GL_TEXTURE0 + samplerUnit);
		glBindTexture(GL_TEXTURE_3D, textureId);
		vec3 size(sx, sy, sz);
		location = glGetUniformLocation(shader, "volumeSize");
		glUniform3fv(location, 1, &size.x);
	}




	
};

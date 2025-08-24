#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>
#include "algebra.h"
#include "GPUProgram.h"
#include "texture.h"
#include "camera.h"
#include "framebuffer.hpp"
#include "boundingbox.h"
#include "texture3d.h"

class Scene
{
	BoundingBox* boundingBox;
	int bufferWidth, bufferHeight;
	Framebuffer* backbuffer;
	GPUProgram gpuProgram1;
	GPUProgram gpuProgram0;
	Texture3D* volume;
	float stepSize;
	float minStepSize;
public:
	Camera camera;
	Scene() {}

	Scene(int width, int height);

	void ResizeFrameBuffer(int width, int height);

	void DecreaseStepSize();

	float getStepSize() { return stepSize; }

	void IncreaseStepSize();

	void Render();

	~Scene();
};
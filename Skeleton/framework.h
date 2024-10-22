//=============================================================================================
// Collection of programs from lecture slides.
// Framework for assignments. Valid from 2020.
//
// Do not change it if you want to submit a homework.
// In the homework, file operations other than printf are prohibited.
//=============================================================================================
		// M_PI
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string>

#if defined(__APPLE__)
#include <GLUT/GLUT.h>
#include <OpenGL/gl3.h>
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/glew.h>		// must be downloaded
#include <GL/freeglut.h>	// must be downloaded unless you have an Apple
#endif
#include "algebra.h"
#include "GPUProgram.h"
#include "texture.h"
#include "camera.h"
#include "framebuffer.hpp"
#include "boundingbox.h"
// Resolution of screen
const unsigned int windowWidth = 600, windowHeight = 600;

//--------------------------
class Scene
{
	BoundingBox* boundingBox;
	int bufferWidth, bufferHeight;
	Framebuffer* backbuffer;
	GPUProgram gpuProgram1;
	GPUProgram gpuProgram0;
	float stepSize;
public:
	Camera camera;
	Scene(){}
	Scene(int width, int height) // konstruktor: memóriapuffer és befoglaló doboz létrehozása
	{
		bufferWidth = width; bufferHeight = height;
		backbuffer = new Framebuffer(bufferWidth, bufferHeight, 1, false, true);
		boundingBox = new BoundingBox(1.0, 1.0, 0.25);
		gpuProgram0.create("vertexSource.glsl", "fragmentSource.glsl", "outColor");
		camera = Camera(width, height);
		gpuProgram1.create("vertexSource2.glsl", "fragmentSource2.glsl", "outColor");
		stepSize = 0.002;
	}
	~Scene() // destruktor: memóriapuffer és befoglaló doboz lebontása
	{
		delete boundingBox;
		delete backbuffer;
	}
	void ResizeFrameBuffer(int width, int height) // újra kell allokálni a memóriapuffert
	{
		delete backbuffer;
		bufferWidth = width; bufferHeight = height;
		backbuffer = new Framebuffer(bufferWidth, bufferHeight, 1, false, true);
	}
	void Render() // színtér renderelése
	{
		glDisable(GL_DEPTH_TEST); // z-bufferelés kikapcsolása
		backbuffer->clear(); // memóriapuffer törlése
		backbuffer->setRenderTarget(); // memóriapuffer bekapcsolása
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK); // csak a hátlapok renderelése
		gpuProgram0.Use(); // gpuProgram0 aktiválása a hátlapok rendereléséhez
		mat4 MVP = camera.V() * camera.P();
		gpuProgram0.setUniform(MVP, "MVP"); // MVP mátrix feltöltése gpuProgram0-hoz
		boundingBox->Render(); // befoglaló doboz hátlapjainak renderelése
		backbuffer->disableRenderTarget(); // memóriapuffer kikapcsolása
		glClearColor(0.8, 0.8, 0.9, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_FRONT); // csak az elõlapok renderelése
		gpuProgram1.Use(); // gpuProgram1 aktiválása a hátlapok rendereléséhez
		gpuProgram1.setUniform(stepSize, "stepSize");
		int samplerUnit = 0; // az elsõ lépés eredményét 2D textúraként töltjük fel
		int location = glGetUniformLocation(gpuProgram1.getId(), "samplerUnitBackBuffer");
		glUniform1i(location, samplerUnit);
		glActiveTexture(GL_TEXTURE0 + samplerUnit);
		glBindTexture(GL_TEXTURE_2D, backbuffer->getColorBuffer(0));
		gpuProgram1.setUniform(MVP, "MVP"); // MVP mátrix feltöltése gpuProgram1-hez
		boundingBox->Render(); // befoglaló doboz elõlapjainak renderelése
	}

	
};


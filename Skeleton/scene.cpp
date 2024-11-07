#include "scene.h"


Scene::Scene(int width, int height) // construktor:  create memorybufferr and bondingbox
{
	bufferWidth = width; bufferHeight = height;
	backbuffer = new Framebuffer(bufferWidth, bufferHeight, 1, false, true);
	boundingBox = new BoundingBox(1.0, 1.0, 0.25);
	gpuProgram0.create("vertexSource.glsl", "fragmentSource.glsl", "outColor");
	camera = Camera(width, height);
	gpuProgram1.create("vertexSource2.glsl", "fragmentSource2.glsl", "outColor");
	minStepSize = 0.0001;
	stepSize = minStepSize;
	int sx, sy, sz; sx = sy = sz = 80;
	volume = new Texture3D(sx, sy, sz);
}


void Scene::ResizeFrameBuffer(int width, int height) // recreate memorybuffer 
{
	delete backbuffer;
	bufferWidth = width; bufferHeight = height;
	backbuffer = new Framebuffer(bufferWidth, bufferHeight, 1, false, true);
}

void Scene::DecreaseStepSize() 
{
	stepSize = std::fmax(minStepSize, stepSize / 1.2);
}

void Scene::IncreaseStepSize() 
{
	stepSize *= 1.2;
}


void Scene::Render()
{
	glDisable(GL_DEPTH_TEST); // turn off z-buffer
	backbuffer->clear(); // memorybuffer clear
	backbuffer->setRenderTarget(); // memorybuffer on
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // for rendereing the back face
	gpuProgram0.Use(); // gpuProgram0 activate for render
	mat4 MVP = camera.V() * camera.P();
	gpuProgram0.setUniform(MVP, "MVP"); //set the MVP matrix to GPU
	boundingBox->Render(); //  rendering boundigbox back face
	backbuffer->disableRenderTarget(); // turn off memorybuffer
	glClearColor(0.8, 0.8, 0.9, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT); // for rendereing the front face
	gpuProgram1.Use(); // gpuProgram1 activate for render
	gpuProgram1.setUniform(stepSize, "stepSize");
	int samplerUnit = 0; // the first result update to a 2D texture
	int location = glGetUniformLocation(gpuProgram1.getId(), "samplerUnitBackBuffer");
	glUniform1i(location, samplerUnit);
	glActiveTexture(GL_TEXTURE0 + samplerUnit);
	glBindTexture(GL_TEXTURE_2D, backbuffer->getColorBuffer(0));
	gpuProgram1.setUniform(MVP, "MVP"); //set the MVP matrix to GPU
	volume->Bind(gpuProgram1);
	boundingBox->Render(); // rendering boundigbox front face
}


Scene::~Scene() 
{
	delete boundingBox;
	delete backbuffer;
	delete volume;
}
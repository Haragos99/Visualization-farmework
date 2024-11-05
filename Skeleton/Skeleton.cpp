#include "framework.h"
#include "opengltimer.hpp"




OpenGLTimer* timer = 0;
Scene* sceen;
int currentWindowWidth = windowWidth;
int currentWindowHeight = windowHeight;
// Initialization, create an OpenGL context
void onInitialization() {
	glViewport(0, 0, windowWidth, windowHeight);
	sceen = new Scene(windowWidth, windowHeight);
	timer = new OpenGLTimer();

}

// Window has become invalid: Redraw
void onDisplay() {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	if (timer) {
		timer->start(); 
	}

	sceen->Render();
	float t = 1.0;
	if (timer) t = timer->stop();
	glutSwapBuffers();


	float fps = 1000.0 / t;
	if (fps < 30.0) sceen->IncreaseStepSize();
	if (fps > 60.0) sceen->DecreaseStepSize();
	char title[128];
	sprintf(title, "Volume Rendering (fps: %.2f, step size: %.4f)", fps, sceen->GetStepSize());
	glutSetWindowTitle(title);
}

// Key of ASCII code pressed
void onKeyboard(unsigned char key, int pX, int pY) {
	if (key == 'd') glutPostRedisplay();         // if d, invalidate display, i.e. redraw
}

// Key of ASCII code released
void onKeyboardUp(unsigned char key, int pX, int pY) {
}

// Move mouse with key pressed
vec2 mousePrevPosition;
bool isFirstMove = true;

// Move mouse with key pressed
void onMouseMotion(int pX, int pY) {	// pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / windowWidth - 1;	// flip y axis
	float cY = 1.0f - 2.0f * pY / windowHeight;
	printf("Mouse moved to (%3.2f, %3.2f)\n", cX, cY);

	vec2 mousePosition = vec2{ cX, cY };
	if (isFirstMove) {
		isFirstMove = false;
		mousePrevPosition = mousePosition;
	}
	sceen->camera.rotateAroundLookAt(vec3{ mousePosition.y - mousePrevPosition.y, -mousePosition.x + mousePrevPosition.x, 0.0f });
	mousePrevPosition = mousePosition;
	glutPostRedisplay();
	
}

// Mouse click event
void onMouse(int button, int state, int pX, int pY) { // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / windowWidth - 1;	// flip y axis
	float cY = 1.0f - 2.0f * pY / windowHeight;

	char * buttonStat;
	switch (state) {
	case GLUT_DOWN: buttonStat = "pressed"; break;
	case GLUT_UP:   buttonStat = "released"; isFirstMove = true;break;
	}

	switch (button) {
	case GLUT_LEFT_BUTTON:   printf("Left button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);   break;
	case GLUT_MIDDLE_BUTTON: printf("Middle button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY); break;
	case GLUT_RIGHT_BUTTON:  printf("Right button %s at (%3.2f, %3.2f)\n", buttonStat, cX, cY);  break;
	}
}

// Idle event indicating that some time elapsed: do animation here
void onIdle() {
	long time = glutGet(GLUT_ELAPSED_TIME); // elapsed time since the start of the program
}


void onReshape(int width, int height)
{
	currentWindowWidth = width;
	currentWindowHeight = height;
	glViewport(0, 0, width, height);
	
	sceen->camera.SetAspectRatio((float)width / height);
	sceen->ResizeFrameBuffer(width, height);
	glutPostRedisplay();
}

void onExit()
{
	
}
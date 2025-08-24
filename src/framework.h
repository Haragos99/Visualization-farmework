
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
#include "opengltimer.hpp"
#include "scene.h"

// Resolution of screen
const unsigned int windowWidth = 600, windowHeight = 600;

// Initialization
void onInitialization();

// Window has become invalid: Redraw
void onDisplay();

// Key of ASCII code pressed
void onKeyboard(unsigned char key, int pX, int pY);

// Key of ASCII code released
void onKeyboardUp(unsigned char key, int pX, int pY);

// Move mouse with key pressed
void onMouseMotion(int pX, int pY);

// Mouse click event
void onMouse(int button, int state, int pX, int pY);

// Idle event indicating that some time elapsed: do animation here
void onIdle();


void onReshape(int width, int height);


	


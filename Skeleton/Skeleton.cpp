#include "framework.h"


class BoundingBox {
	unsigned int vao;
	float x, y, z;
public:
	BoundingBox(float x, float y, float z) : x(x), y(y), z(z) {
		float m = max(x, max(y, z)) * 2.0;
		float x1 = (float)x / m, x0 = -x1, y1 = (float)y / m, y0 = -y1, z1 = (float)z / m, z0 = -z1;
		float vertices[] = { // középre igazítás
		x0, y0, z0,  x1, y0, z0,  x1, y1, z0,  x0, y1, z0,
		x0, y0, z1,  x0, y1, z1,  x1, y1, z1,  x1, y0, z1,
		x0, y0, z0,  x0, y0, z1,  x1, y0, z1,  x1, y0, z0,
		x0, y1, z0,  x1, y1, z0,  x1, y1, z1,  x0, y1, z1,
		x0, y0, z0,  x0, y1, z0,  x0, y1, z1,  x0, y0, z1,
		x1, y0, z0,  x1, y0, z1,  x1, y1, z1,  x1, y1, z0

		
		};
		x0 = 0.5 - x / m; x1 = 0.5 + x / m; y0 = 0.5 - y / m; y1 = 0.5 + y / m; z0 = 0.5 - z / m; z1 = 0.5 + z / m;
		float colors[] = { // normalizált színkomponensek
		x0, y0, z0,  x1, y0, z0,  x1, y1, z0,  x0, y1, z0,
		x0, y0, z1,  x0, y1, z1,  x1, y1, z1,  x1, y0, z1,
		x0, y0, z0,  x0, y0, z1,  x1, y0, z1,  x1, y0, z0,
		x0, y1, z0,  x1, y1, z0,  x1, y1, z1,  x0, y1, z1,
		x0, y0, z0,  x0, y1, z0,  x0, y1, z1,  x0, y0, z1,
		x1, y0, z0,  x1, y0, z1,  x1, y1, z1,  x1, y1, z0
		
		};
		glGenVertexArrays(1, &vao); glBindVertexArray(vao); // VAO aktiválása
		unsigned int vbo[2]; glGenBuffers(2, vbo); // kettõ VBO ID 
		// pozíciók feltöltése a GPU-ra:
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0); glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		// színek feltöltése a GPU-ra:
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1); glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}
	void Render() {
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
		glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
	}
};







GPUProgram gpuProgram; // vertex and fragment shaders
unsigned int vao;	   // virtual world on the GPU
BoundingBox* boundingBox = 0;
int currentWindowWidth = windowWidth;
int currentWindowHeight = windowHeight;
Camera camera(currentWindowWidth, currentWindowHeight);


// Initialization, create an OpenGL context
void onInitialization() {
	glViewport(0, 0, windowWidth, windowHeight);


	boundingBox = new BoundingBox(1.0, 1.0, 1.0);
	// create program for the GPU
	gpuProgram.create("vertexSource.glsl", "fragmentSource.glsl", "outColor");
}

// Window has become invalid: Redraw
void onDisplay() {
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	gpuProgram.Use();
	mat4 MVP = camera.V() * camera.P();
	gpuProgram.setUniform(MVP, "MVP");
	boundingBox->Render();
	glutSwapBuffers();
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
	camera.rotateAroundLookAt(vec3{ mousePosition.y - mousePrevPosition.y, -mousePosition.x + mousePrevPosition.x, 0.0f });
	glutPostRedisplay();
	mousePrevPosition = mousePosition;
}

// Mouse click event
void onMouse(int button, int state, int pX, int pY) { // pX, pY are the pixel coordinates of the cursor in the coordinate system of the operation system
	// Convert to normalized device space
	float cX = 2.0f * pX / windowWidth - 1;	// flip y axis
	float cY = 1.0f - 2.0f * pY / windowHeight;

	char * buttonStat;
	switch (state) {
	case GLUT_DOWN: buttonStat = "pressed"; break;
	case GLUT_UP:   buttonStat = "released"; break;
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
	camera.SetAspectRatio((float)width / height);
	glutPostRedisplay();
}

void onExit()
{
	if (boundingBox) delete boundingBox;
}
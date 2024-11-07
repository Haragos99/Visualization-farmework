#include "boundingbox.h"

BoundingBox::BoundingBox(float x, float y, float z) : x(x), y(y), z(z) {
	float m =  fmax(x, fmax(y, z)) * 2.0;
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



void BoundingBox::Render() {
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 4);
}
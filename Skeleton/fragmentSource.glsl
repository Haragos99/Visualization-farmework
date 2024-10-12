#version 330
precision highp float;

uniform vec3 color;
out vec4 outColor;
in vec3 vertexColor;
void main() {
	outColor = vec4(vertexColor, 1);
}
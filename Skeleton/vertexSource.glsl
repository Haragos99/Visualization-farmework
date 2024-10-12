#version 330
precision highp float;
uniform mat4 MVP;
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vc;

out vec3 vertexColor;
void main() {
	gl_Position = vec4(vp.x, vp.y, vp.z, 1) * MVP;
	vertexColor = vc;
}
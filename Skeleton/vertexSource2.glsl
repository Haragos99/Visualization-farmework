#version 330
precision highp float;
uniform mat4 MVP;
layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 vc;
out vec3 entryPoint;
out vec4 pixelPosition;
void main()
{
	vec4 vpTrans =
		vec4(vp.x, vp.y, vp.z, 1) * MVP;
	pixelPosition = vpTrans;
	gl_Position = vpTrans;
	entryPoint = vc;
}
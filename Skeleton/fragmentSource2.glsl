#version 330
precision highp float;

uniform sampler2D samplerUnitBackBuffer;
uniform float stepSize;

float pi = 3.14159265;









float rho(float r)
{
	return cos(pi * 2.0 + pi * 12.0 * cos(pi * r / 2.0));
}
float MarschnerLobb(vec3 position)
{
	position = (position - vec3(0.5, 0.5, 0.5)) * 2.0;
	vec2 r = position.xy;
	return (1.0 - sin(pi * position.z / 2.0) + (1.0 + rho(length(r))) / 4.0) / 2.5;
}


vec3 MarschnerLobbGradient(vec3 position)
{
	float eps = 0.001;
	vec3 dx = vec3(eps, 0.0, 0.0);
	vec3 dy = vec3(0.0, eps, 0.0);
	vec3 dz = vec3(0.0, 0.0, eps);
	float gx = MarschnerLobb(position + dx) - MarschnerLobb(position - dx);
	float gy = MarschnerLobb(position + dy) - MarschnerLobb(position - dy);
	float gz = MarschnerLobb(position + dz) - MarschnerLobb(position - dz);
	return normalize(vec3(gx, gy, gz));
}



in vec3 entryPoint;
in vec4 pixelPosition;
out vec4 outColor;
void main() {
	vec4 color = vec4(0.7, 0.7, 0.8, 1.0);
	vec2 textCoord = (pixelPosition.xy / pixelPosition.w + 1.0) / 2.0;
	vec3 exitPoint = texture(samplerUnitBackBuffer, textCoord).xyz;
	vec3 rayDir = exitPoint - entryPoint;
	float rayLength = length(rayDir);
	const int maxNumberOfSamples = 4096; // a minták számának maximuma egy sugárra
	int numberOfSamples = 0;
	float threshold = 0.5;
	if (rayLength > 0.01) {
		rayDir /= rayLength;
		vec3 position = entryPoint;
		for (float t = 0.0; t < rayLength; t += stepSize) {
			position += rayDir * stepSize;
			numberOfSamples++; // minták számlálása
			if (numberOfSamples > maxNumberOfSamples) break;

			float density = MarschnerLobb(position);
			if (abs(density - threshold) < 0.01)
			{
				vec3 normal = MarschnerLobbGradient(position); // tesztjel gradiensének kiértékelése
				vec3 kd = vec3(0.6, 0.6, 0.9); // diffúz együttható
				vec3 ks = vec3(1.0, 1.0, 1.0); // spekuláris együttható
				color.xyz =
					kd * abs(dot(rayDir, normal)) + // diffúz komponens
					ks * pow(abs(dot(rayDir, normal)), 100.0); // spekuláris komponens
				break;
			}
		}
	}
	outColor = color;
}

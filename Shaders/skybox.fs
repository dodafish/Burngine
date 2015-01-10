#version 330

layout(location = 0) out vec4 outFragmentColor;
layout(location = 1) out vec3 outFragmentNormal;

in vec3 passUv;

uniform samplerCube gCubeMap;

void main() {
	outFragmentColor = texture(gCubeMap, normalize(passUv));
	outFragmentNormal = vec4(0.0);
}
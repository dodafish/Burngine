#version 330

layout(location = 0) out vec4 outFragmentColor;

in vec3 passUv;

uniform samplerCube gCubeMap;

void main() {
	outFragmentColor = texture(gCubeMap, passUv);
}
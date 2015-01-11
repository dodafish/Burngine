#version 330

layout(location = 0) out vec4 outFragmentColor;
layout(location = 1) out vec3 outFragmentNormal;
layout(location = 2) out vec3 outFragmentPosition;
layout(location = 3) out vec4 outFragmentUnshaded;

in vec3 passUv;

uniform samplerCube gCubeMap;

void main() {
	outFragmentColor = texture(gCubeMap, normalize(passUv)).rgba;
	outFragmentNormal = vec3(0.0);
	outFragmentPosition = vec3(0.0);
	outFragmentUnshaded = vec4(0.0, 0.0, 0.0, 0.0);
}
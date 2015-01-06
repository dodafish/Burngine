#version 330
layout(location = 0) in vec3 inVertexPosition;

uniform mat4 gModelMatrix;
uniform mat4 gViewMatrix;
uniform mat4 gProjectionMatrix;

out vec4 passFragmentPosition;

void main() {
	passFragmentPosition = gViewMatrix * gModelMatrix * vec4(inVertexPosition, 1.0);
	gl_Position = gProjectionMatrix * passFragmentPosition;
}
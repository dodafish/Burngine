#version 330

layout(location = 0) in vec3 inVertexPosition;
layout(location = 2) in vec2 inVertexUv;

out vec2 passVertexUv;

uniform mat4 gModelMatrix;
uniform mat4 gProjectionMatrix;

void main() {
	gl_Position = gProjectionMatrix * gModelMatrix * vec4(inVertexPosition, 1.0);
	passVertexUv = inVertexUv;
}
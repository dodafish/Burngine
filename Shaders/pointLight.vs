#version 330

layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec2 inVertexUv;

out vec2 passVertexUv;

void main() {
	gl_Position = vec4(inVertexPosition, 1.0);
	passVertexUv = inVertexUv;
}
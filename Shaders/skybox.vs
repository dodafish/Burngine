#version 330

layout(location = 0) in vec3 inVertexPosition;

out vec3 passUv;

void main() {
	gl_Position = vec4(inVertexPosition.xyzz);
	passUv = inVertexPosition;
}
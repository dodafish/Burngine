#version 330

layout(location = 0) in vec3 inVertexPosition;

out vec3 passUv;

uniform mat4 gModelMatrix;
uniform mat4 gViewMatrix;
uniform mat4 gProjectionMatrix;

void main() {
	gl_Position = (gProjectionMatrix * gViewMatrix * gModelMatrix * vec4(inVertexPosition, 1.0)).xyww;
	passUv = inVertexPosition;
	passUv.y *= -1.0;
}
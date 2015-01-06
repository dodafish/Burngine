#version 330

layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec3 inVertexNormal;
layout(location = 2) in vec2 inVertexUv0;
layout(location = 3) in vec2 inVertexUv1;
layout(location = 4) in vec2 inVertexUv2;
layout(location = 5) in vec2 inVertexUv3;
layout(location = 6) in vec2 inVertexUv4;
layout(location = 7) in vec2 inVertexUv5;
layout(location = 8) in vec2 inVertexUv6;
layout(location = 9) in vec2 inVertexUv7;

out vec2 passVertexUv[8];
out vec3 passVertexNormal;
out vec3 passVertexPosition;

uniform mat4 gModelMatrix;
uniform mat4 gViewMatrix;
uniform mat4 gProjectionMatrix;

void main() {
	gl_Position = gProjectionMatrix * gViewMatrix * gModelMatrix * vec4(inVertexPosition, 1.0);
	passVertexNormal = gModelMatrix * vec4(inVertexNormal, 0.0);
	passVertexPosition = gModelMatrix * vec4(inVertexPosition, 1.0);
	
	passVertexUv[0] = inVertexUv0;
	passVertexUv[1] = inVertexUv1;
	passVertexUv[2] = inVertexUv2;
	passVertexUv[3] = inVertexUv3;
	passVertexUv[4] = inVertexUv4;
	passVertexUv[5] = inVertexUv5;
	passVertexUv[6] = inVertexUv6;
	passVertexUv[7] = inVertexUv7;
}
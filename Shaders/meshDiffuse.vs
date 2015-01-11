#version 330

layout(location = 0) in vec3 inVertexPosition;
layout(location = 1) in vec3 inVertexNormal;
layout(location = 2) in vec2 inVertexUv;
layout(location = 3) in vec3 inVertexTangent;
layout(location = 4) in vec3 inVertexBitangent;

out vec2 passVertexUv;
out vec3 passVertexNormal;
out vec3 passVertexPosition;
out vec3 passVertexTangent;
out vec3 passVertexBitangent;

uniform mat4 gModelMatrix;
uniform mat4 gViewMatrix;
uniform mat4 gProjectionMatrix;

void main() {
	gl_Position = 			gProjectionMatrix * gViewMatrix * gModelMatrix * vec4(inVertexPosition, 1.0);
	passVertexNormal = 		gModelMatrix * vec4(inVertexNormal, 0.0);
	passVertexPosition = 	gModelMatrix * vec4(inVertexPosition, 1.0);
	passVertexUv = 			inVertexUv;
	passVertexTangent = 	inVertexTangent;
	passVertexBitangent = 	inVertexBitangent;
}
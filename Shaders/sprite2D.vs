#version 330

layout(location = 0) in vec2 inVertexPosition;
layout(location = 1) in vec2 inVertexUv;

out vec2 passVertexUv;

uniform mat4 gModelMatrix;
uniform mat4 gViewMatrix;
uniform mat4 gProjectionMatrix;

uniform vec2 gUvStart;
uniform vec2 gUvEnd;

void main() {
	gl_Position = gProjectionMatrix * gViewMatrix * gModelMatrix * vec4(inVertexPosition, 0.0, 1.0);
	passVertexUv = inVertexUv;
	
	// Calculate uvEnd
	passVertexUv *= gUvEnd;
	// Calculate uvStart
	passVertexUv *= (gUvEnd - gUvStart) / gUvEnd;
	passVertexUv += gUvStart;
}
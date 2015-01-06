#version 330

in vec2 passVertexUv[8];
in vec3 passVertexNormal;
in vec3 passVertexPosition;

layout(location = 0) out vec4 outFragmentColor;
layout(location = 1) out vec3 outFragmentNormal;
layout(location = 2) out vec3 outFragmentPosition;

uniform vec4 gColor;
uniform sampler2D gTextureSampler;

void main() {
	outFragmentColor = texture(gTextureSampler, passVertexUv[0]).rgba * gColor;
	outFragmentNormal = normalize(passVertexNormal) * 0.5 + 0.5;
	outFragmentPosition = passVertexPosition;
}
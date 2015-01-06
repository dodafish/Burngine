#version 330

in vec2 passVertexUv;

layout(location = 0) out vec4 outFragmentColor;

uniform vec4 gColor;
uniform sampler2D gTextureSampler;

void main() {
	outFragmentColor = texture(gTextureSampler, passVertexUv).rgba * gColor;
}
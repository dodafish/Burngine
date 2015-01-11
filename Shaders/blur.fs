#version 330

layout (location = 0) out vec4 final;

in vec2 passBlurCoordinates[2];

uniform sampler2D gSampler;

void main(){

	vec4 sum = vec4(0.0);
	
	sum += texture(gSampler, passBlurCoordinates[0]).rgba * 0.5;
	sum += texture(gSampler, passBlurCoordinates[1]).rgba * 0.5;
	
	final = sum;
}
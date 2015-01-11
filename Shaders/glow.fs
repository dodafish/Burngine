#version 330

in vec2 passVertexUv;
out vec4 final;
uniform sampler2D gSampler;

void main(){

	// Sample from texture and use brightest color-comp as alpha value
	vec3 pixel = texture2D(gSampler, passVertexUv).rgb;
	float m = max(max(pixel.r, pixel.g), pixel.b);
	m = pow(m, 2) * 3.0 - 0.4;
	final = vec4(pixel + vec3(0.1), m);
	
}
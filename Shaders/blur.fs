#version 330

in vec2 passBlurCoordinates[5];
out vec4 final;
uniform sampler2D gSampler;

vec2 clampToEdge(vec2 uv){
	vec2 newUv = uv;
	newUv.x = clamp(newUv.x, 0.0, 1.0);
	newUv.y = clamp(newUv.y, 0.0, 1.0);
	return newUv;
}

void main(){

	vec4 sum = vec4(0.0);
	
	//sum += texture2D(gSampler, clampToEdge(passBlurCoordinates[0])).rgba * 0.15;
	sum += texture2D(gSampler, clampToEdge(passBlurCoordinates[1])).rgba * 0.5;
	//sum += texture2D(gSampler, clampToEdge(passBlurCoordinates[2])).rgba * 0.3;
	sum += texture2D(gSampler, clampToEdge(passBlurCoordinates[3])).rgba * 0.5;
	//sum += texture2D(gSampler, clampToEdge(passBlurCoordinates[4])).rgba * 0.15;
	
	final = sum;
	
}
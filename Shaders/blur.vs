#version 330

layout(location = 0) in vec2 inVertexPosition;
layout(location = 1) in vec2 inVertexUv;

out vec2 passBlurCoordinates[2]; // 2 Samples

uniform mat4 gModelMatrix;
uniform mat4 gProjectionMatrix;

uniform bool gIsSecondPass;
uniform vec2 gSamplerDimensions;
uniform float gBlurScale;

void main() {
	gl_Position = gProjectionMatrix * gModelMatrix * vec4(inVertexPosition, 0.0, 1.0);
	
	if(!gIsSecondPass){
		float pixelOff = 1.0 / gSamplerDimensions.x;
		pixelOff *= gBlurScale;
		
		passBlurCoordinates[0] = vec2(inVertexUv.x - 0.5 * pixelOff, inVertexUv.y);
		passBlurCoordinates[1] = vec2(inVertexUv.x + 0.5 * pixelOff, inVertexUv.y);
	}else{
		float pixelOff = 1.0 / gSamplerDimensions.y;
		pixelOff *= gBlurScale;
		
		passBlurCoordinates[0] = vec2(inVertexUv.x, inVertexUv.y - 0.5 * pixelOff);
		passBlurCoordinates[1] = vec2(inVertexUv.x, inVertexUv.y + 0.5 * pixelOff);
	}
	
}
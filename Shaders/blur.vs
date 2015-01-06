#version 330

layout(location = 0) in vec3 inVertexPosition;
layout(location = 2) in vec2 inVertexUv;

out vec2 passBlurCoordinates[5]; // Always 5 Samples -> minimum radius of 4 pixels

uniform mat4 gProjectionMatrix;

uniform bool gIsSecondPass;
uniform vec2 gSamplerDimensions;
uniform float gBlurScale;

void main() {
	gl_Position = gProjectionMatrix * vec4(inVertexPosition, 1.0);
	
	if(!gIsSecondPass){
		float pixelOff = 1.0 / gSamplerDimensions.x;
		pixelOff *= gBlurScale;
		//passBlurCoordinates[0] = vec2(inVertexUv.x - 3.5 * pixelOff, inVertexUv.y);
		passBlurCoordinates[1] = vec2(inVertexUv.x - 0.5 * pixelOff, inVertexUv.y);
		//passBlurCoordinates[2] = vec2(inVertexUv.x, inVertexUv.y);
		passBlurCoordinates[3] = vec2(inVertexUv.x + 0.5 * pixelOff, inVertexUv.y);
		//passBlurCoordinates[4] = vec2(inVertexUv.x + 3.5 * pixelOff, inVertexUv.y);
	}else{
		float pixelOff = 1.0 / gSamplerDimensions.y;
		pixelOff *= gBlurScale;
		//passBlurCoordinates[0] = vec2(inVertexUv.x, inVertexUv.y - 3.5 * pixelOff);
		passBlurCoordinates[1] = vec2(inVertexUv.x, inVertexUv.y - 0.5 * pixelOff);
		//passBlurCoordinates[2] = vec2(inVertexUv.x, inVertexUv.y);
		passBlurCoordinates[3] = vec2(inVertexUv.x, inVertexUv.y + 0.5 * pixelOff);
		//passBlurCoordinates[4] = vec2(inVertexUv.x, inVertexUv.y + 3.5 * pixelOff);
	}
	
}
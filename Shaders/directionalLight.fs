#version 330

in vec2 passVertexUv;

layout(location = 0) out vec3 outDiffuseLighting;
layout(location = 1) out vec3 outSpecularLighting;

uniform vec3 gLightColor;
uniform float gLightIntensity;
uniform vec3 gLightDirection;
uniform vec3 gCameraPosition;

uniform sampler2D gPositionSampler;
uniform sampler2D gNormalSampler;

uniform sampler2D gShadowMapSampler_WIDE;
uniform sampler2D gShadowMapSampler_MEDIUM;
uniform sampler2D gShadowMapSampler_SMALL;
uniform mat4 gShadowViewMatrix;
uniform mat4 gShadowProjectionMatrix_WIDE;
uniform mat4 gShadowProjectionMatrix_MEDIUM;
uniform mat4 gShadowProjectionMatrix_SMALL;

float ChebyshevUpperBound(vec2 moments, float t)
{

	//Surface is fully lit if the current fragment is before the light occluder
	if(t >= (moments.x)){
		return 1.0;
	}
	
	//Compute variance.
	float variance = moments.y - (moments.x*moments.x);
	variance = max(variance, 0.004);
	
	//Compute probabilistic upper bound.
	float d =  t - moments.x;
	float p_max = variance / (variance + d*d);
	
	return p_max;
}

float linstep(float min, float max, float v){
	return clamp((v - min) / (max - min), 0.0, 1.0);
}

float ReduceLightBleeding(float p_max, float Amount)  
{
	//Remove the [0, Amount] tail and linearly rescale (Amount, 1].  
	return linstep(Amount, 1.0, p_max);
}

float ShadowContribution(vec2 LightTexCoord, float DistanceToLight, sampler2D shadowMap)
{
	//Read the moments from the variance shadow map.
	vec2 Moments = texture(shadowMap, LightTexCoord).xy;
	
	//Compute the Chebyshev upper bound.
	float pmax = ChebyshevUpperBound(Moments, DistanceToLight);

	return ReduceLightBleeding(pmax, 0.1);
}

bool isInsideTexture(vec2 uv){
	if(uv.x < 0.02 || uv.x > 0.98)
		return false;
	if(uv.y < 0.02 || uv.y > 0.98)
		return false;
	return true;
}

void main() {
	
	vec3 fragmentNormal = texture(gNormalSampler, passVertexUv).rgb * 2.0 - 1.0;
	// Skip empty pixels (i.e. nothing was drawn there)
	if(fragmentNormal == vec3(-1.0)){
		outDiffuseLighting = vec3(0.0);
		outSpecularLighting = vec3(0.0);
		return;
	}
	
	vec3 fragmentPosition = texture(gPositionSampler, passVertexUv).rgb;
	
	vec3 l = normalize(gLightDirection);
	float factor = clamp(dot(l, fragmentNormal), 0.0, 1.0);
	
	float shadowFactor = 1.0;
	float compare = (gShadowViewMatrix * vec4(fragmentPosition, 1.0)).z;
	
	vec2 shadowCoord = (gShadowProjectionMatrix_SMALL * gShadowViewMatrix * vec4(fragmentPosition, 1.0)).xy * 0.5 + 0.5;
	if(isInsideTexture(shadowCoord))
		shadowFactor = ShadowContribution(shadowCoord, compare, gShadowMapSampler_SMALL);
	else{
		shadowCoord = (gShadowProjectionMatrix_MEDIUM * gShadowViewMatrix * vec4(fragmentPosition, 1.0)).xy * 0.5 + 0.5;
		if(isInsideTexture(shadowCoord))
			shadowFactor = ShadowContribution(shadowCoord, compare, gShadowMapSampler_MEDIUM);
		else{
			shadowCoord = (gShadowProjectionMatrix_WIDE * gShadowViewMatrix * vec4(fragmentPosition, 1.0)).xy * 0.5 + 0.5;
			if(isInsideTexture(shadowCoord))
				shadowFactor = ShadowContribution(shadowCoord, compare, gShadowMapSampler_WIDE);
		}
	}
	
	outDiffuseLighting = shadowFactor * gLightColor * gLightIntensity * factor;
	
	vec3 E = normalize(gCameraPosition - fragmentPosition);
	vec3 R = reflect(-l, fragmentNormal);
	float cosAlpha = clamp( dot( E,R ), 0.0,1.0 );
	
	outSpecularLighting = shadowFactor * gLightColor * gLightIntensity * factor * pow(cosAlpha, 50);
	
}
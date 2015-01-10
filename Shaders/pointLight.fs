#version 330

in vec2 passVertexUv;

layout(location = 0) out vec3 outDiffuseLighting;
layout(location = 1) out vec3 outSpecularLighting;

uniform vec3 gLightPosition;
uniform vec3 gLightColor;
uniform float gLightIntensity;
uniform vec3 gCameraPosition;

uniform sampler2D gPositionSampler;
uniform sampler2D gNormalSampler;

void main() {
	
	vec3 fragmentNormal = texture(gNormalSampler, passVertexUv).rgb * 2.0 - 1.0;
	// Skip empty pixels (i.e. nothing was drawn there)
	if(fragmentNormal == vec3(-1.0)){
		outDiffuseLighting = vec3(1.0);
		outSpecularLighting = vec3(0.0);
		return;
	}
	
	vec3 fragmentPosition = texture(gPositionSampler, passVertexUv).rgb;
	
	vec3 l = normalize(gLightPosition - fragmentPosition);
	float factor = clamp(dot(l, fragmentNormal), 0.0, 1.0);
	float dist = distance(fragmentPosition, gLightPosition);
	
	outDiffuseLighting = gLightColor * gLightIntensity * factor / (dist * dist);
	//outDiffuseLighting += vec3(0.3);
	
	vec3 E = normalize(gCameraPosition - fragmentPosition);
	vec3 R = reflect(-l, fragmentNormal);
	float cosAlpha = clamp( dot( E,R ), 0.0,1.0 );
	
	outSpecularLighting = gLightColor * gLightIntensity * factor * pow(cosAlpha, 5) / (dist * dist);
	
}
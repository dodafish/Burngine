#version 330

in vec2 passVertexUv;
in vec3 passVertexNormal;
in vec3 passVertexPosition;
in vec3 passVertexTangent;
in vec3 passVertexBitangent;

layout(location = 0) out vec4 outFragmentColor;
layout(location = 1) out vec3 outFragmentNormal;
layout(location = 2) out vec3 outFragmentPosition;
layout(location = 3) out vec4 outFragmentUnshaded;

uniform vec3 gDiffuseColor;

uniform sampler2D gDiffuseTexture;
uniform sampler2D gNormalTexture;
uniform samplerCube gReflectionCubeMap;

// Fake booleans
uniform int gUseDiffuseTexture;
uniform int gUseNormalTexture;
uniform int gUseReflectionCubeMap;

// Additional information
uniform vec3 gCameraPosition;	// Eye-vector

void main() {

	///////////////////////////////////////////////////////////////////////////
	if(gUseDiffuseTexture == 1)
		outFragmentColor = 	vec4(texture(gDiffuseTexture, passVertexUv).rgb, 1.0);
	else
		outFragmentColor = vec4(gDiffuseColor, 1.0);
	
	///////////////////////////////////////////////////////////////////////////
	vec3 fragmentNormal = vec3(0.0);
	if(gUseNormalTexture == 1){
		vec3 texel = texture(gNormalTexture, passVertexUv).rgb * 2.0 - 1.0;
		mat3 tbn = mat3(normalize(passVertexTangent), 
						normalize(passVertexBitangent), 
						normalize(passVertexNormal));
		fragmentNormal = normalize(tbn * texel);
	}else{
		fragmentNormal = normalize(passVertexNormal);
	}
	outFragmentNormal = fragmentNormal * 0.5 + 0.5;
	
	///////////////////////////////////////////////////////////////////////////
	outFragmentPosition = 	passVertexPosition;
	
	///////////////////////////////////////////////////////////////////////////
	if(gUseReflectionCubeMap == 1){
		vec3 reflection = reflect(normalize(passVertexPosition - gCameraPosition), fragmentNormal);
		reflection.y *= -1.0;
		
		float fresnel = 4.2;
		float aoi = dot(normalize(passVertexPosition - gCameraPosition), fragmentNormal);
		float alpha = 1.0 - pow(aoi, 2) * fresnel - (0.1 * fresnel);

		outFragmentUnshaded = vec4(texture(gReflectionCubeMap, reflection).rgb, alpha);
	}else{
		outFragmentUnshaded = vec4(0.0, 0.0, 0.0, 0.0);
	}
}
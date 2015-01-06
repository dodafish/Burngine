#version 330

in vec2 passVertexUv[8];
in vec3 passVertexNormal;
in vec3 passVertexPosition;
in vec3 passTangents[2];
in mat3 passModelMatrix;

layout(location = 0) out vec4 outFragmentColor;
layout(location = 1) out vec3 outFragmentNormal;
layout(location = 2) out vec3 outFragmentPosition;

struct TextureStack {
	vec3 baseColor;
	sampler2D sampler[8];
	float blending[8];
	int operator[8];
	int uvChannel[8];
	bool isLoaded[8];
};

uniform TextureStack gDiffuseStack;
uniform TextureStack gNormalStack;

void main() {

	vec3 diffusePart = gDiffuseStack.baseColor;
	for(int i = 0; i != 8; ++i){
		if(gDiffuseStack.isLoaded[i]){
			vec3 texel = texture(	gDiffuseStack.sampler[i], 
									passVertexUv[gDiffuseStack.uvChannel[i]]).rgb * gDiffuseStack.blending[i];
			
			
			
			int op = gDiffuseStack.operator[i];
			if(op == 0)
				diffusePart *= texel;
			else if(op == 1)
				diffusePart += texel;
			else if(op == 2)
				diffusePart -= texel;
			else if(op == 3)
				diffusePart /= texel;
			else if(op == 4)
				diffusePart = (diffusePart+texel)-(diffusePart*texel);
			else if(op == 5)
				diffusePart += (texel - 0.5);
			else
				diffusePart = texel;
			
		}
	}

	vec3 normal = passVertexNormal;
	vec3 originalNormal = normal;
	for(int i = 0; i != 8; ++i){
		if(gNormalStack.isLoaded[i]){
			vec3 texel = texture(	gNormalStack.sampler[i], 
									passVertexUv[gNormalStack.uvChannel[i]]).rgb * 2 - 1;
			
			mat3 tbn = mat3(passTangents[0], passTangents[1], originalNormal);
			texel = tbn * texel;
			
			int op = gNormalStack.operator[i];
			if(op == 0)
				normal *= texel;
			else if(op == 1)
				normal += texel;
			else if(op == 2)
				normal -= texel;
			else if(op == 3)
				normal /= texel;
			else if(op == 4)
				normal = (normal+texel)-(normal*texel);
			else if(op == 5)
				normal += (texel - 0.5);
			else
				normal = texel;
			
		}
	}
	
	//normal = passModelMatrix * normal;
	
	outFragmentColor = vec4(diffusePart, 1.0);
	outFragmentNormal = normalize(normal) * 0.5 + 0.5;
	outFragmentPosition = passVertexPosition;
}
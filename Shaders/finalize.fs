#version 330

in vec2 passVertexUv;
out vec4 final;

uniform sampler2D gColorSampler; // Diffuse Color
uniform sampler2D gDiffuseSampler; // Diffuse Lighting
uniform sampler2D gSpecularSampler; // Specular Lighting
uniform sampler2D gUnshadedSampler; // Unshaded color

void main(){

	vec4 color = texture(gColorSampler, passVertexUv).rgba;
	vec3 diffuse = texture(gDiffuseSampler, passVertexUv).rgb;
	vec3 specular = texture(gSpecularSampler, passVertexUv).rgb;
	vec4 unshaded = texture(gUnshadedSampler, passVertexUv).rgba;
	
	final = color;
	final.rgb *= diffuse;
	final.rgb += specular;
	
	vec3 unshadedAlpha = unshaded.rgb * unshaded.a;
	vec3 finalAlpha = final.rgb * (1.0 - unshaded.a);
	final.rgb = unshadedAlpha + finalAlpha;
}
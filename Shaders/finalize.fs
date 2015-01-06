#version 330

in vec2 passVertexUv;
out vec4 final;

uniform sampler2D gColorSampler; // Diffuse Color
uniform sampler2D gDiffuseSampler; // Diffuse Lighting
uniform sampler2D gSpecularSampler; // Specular Lighting

void main(){

	vec4 color = texture2D(gColorSampler, passVertexUv).rgba;
	vec4 diffuse = vec4(texture2D(gDiffuseSampler, passVertexUv).rgb, 1.0);
	vec4 specular = vec4(texture2D(gSpecularSampler, passVertexUv).rgb, 1.0);
	
	final = specular + diffuse * color;
	
}
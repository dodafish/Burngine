#version 330

layout(location = 0) out vec2 outFragmentDepths;

in vec4 passFragmentPosition;

uniform bool gUseRawZ;

void main() {

	float d = 0.0;
	if(gUseRawZ)
		d = passFragmentPosition.z;
	else
		d = length(passFragmentPosition.xyz);
	
	outFragmentDepths.x = d;
	
	//Compute partial derivatives of depth.
	float dx = dFdx(d);
	float dy = dFdy(d);
	//Compute second moment over the pixel extents.
	outFragmentDepths.y = d*d + 0.25*(dx*dx + dy*dy);

}
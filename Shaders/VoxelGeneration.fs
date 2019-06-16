#version 430
layout (location=0) out uvec4 frag_int;

uniform float near;
uniform float far;

uniform int resolution;

uniform int lookUpTable[24*4];

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}



void main() {
	float linearDepth=LinearizeDepth(gl_FragCoord.z);
	float voxelSize = 1.0f/float(resolution);
	uint depthIndex = uint(floor(linearDepth/voxelSize));
	
	frag_int = uvec4(lookUpTable[depthIndex*4],lookUpTable[depthIndex*4+1],lookUpTable[depthIndex*4+2],lookUpTable[depthIndex*4+3]);
	//frag_int = uvec4(pow(2,depthIndex),0,0,0);
}
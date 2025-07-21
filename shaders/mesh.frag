#version 450

#extension GL_GOOGLE_include_directive : require 
#include "input_structures.glsl"

layout (location = 0) in vec3 vWorldPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec3 inColor;
layout (location = 3) in vec2 inUV;


struct PointLight {
	vec4 position;
	vec4 color;
	vec3 attenutaion; // k_c, k_l, k_q
	float padding;
};


layout(std430, set = 0, binding = 2) readonly buffer PointLightBuffer {
	uint lightCount;
	float pad[3];
	PointLight lights[];
} uLightsSSBO;

layout (location = 0) out vec4 outFragColor;

void main()
{
	vec3 N = normalize(inNormal);
	vec3 V = normalize(sceneData.cameraPosition.xyz - vWorldPos);
	vec3 baseColor = inColor * texture(colorTex, inUV).xyz;

	vec3 accum  = sceneData.ambientColor.xyz * baseColor;


	// loop all lights
	for (uint i = 0; i < uLightsSSBO.lightCount; i++) {
		PointLight light = uLightsSSBO.lights[i];

		vec3 ligthDir = light.position.xyz - vWorldPos;
		float d = length(ligthDir);
		ligthDir = normalize(ligthDir);

		// physical attenutaion 
		float kc = light.attenutaion.x;
		float kl = light.attenutaion.y;
		float kq = light.attenutaion.z;
	    float att = 1.0 / (kc + kl  * d + kq * d * d);

		// diffuse 
		float diffuse = max(dot(N, ligthDir), 0.0);

		// speuclar 
		vec3 H = normalize(ligthDir + V);
		float spec = pow(max(dot(N, H), 0.0), 32.0);

		vec3 radiance = light.color.rgb * att;
		accum += baseColor * diffuse * radiance + spec * radiance;
	} 

	outFragColor = vec4(accum, 1.0f);
}
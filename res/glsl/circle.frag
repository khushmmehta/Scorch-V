#version 460 core

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 v_UV;

layout(location = 0) out vec4 outColor;

void main()
{
	float uvDistance = v_UV.x * v_UV.x + v_UV.y * v_UV.y;
	outColor = vec4(fragColor, 1 - smoothstep(0.975, 1, uvDistance));
}

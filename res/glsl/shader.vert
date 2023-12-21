#version 460

layout (binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 inTransform;

layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec2 v_UV;

void main() {
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition + inTransform, 1.0);
    fragColor = inColor;

    v_UV = inUV;
}

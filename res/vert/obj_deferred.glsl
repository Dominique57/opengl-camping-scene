#version 460 core

layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

out vec2 texCoords;
out vec3 fragPos;
out vec3 fragNormal;

uniform mat4 model;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    vec4 worldPos = model * vec4(inPos, 1);
    // http://www.lighthouse3d.com/tutorials/glsl-12-tutorial/the-normal-matrix/
    vec3 scaledNormal = mat3(transpose(inverse(view_matrix * model))) * inNormal;

    // We send to fragShader: worldPos, texCoords, and normal (all in world space)
    fragPos = worldPos.xyz;
    texCoords = inTexCoords;
    fragNormal = scaledNormal;

    gl_Position = projection_matrix * view_matrix * worldPos;
}
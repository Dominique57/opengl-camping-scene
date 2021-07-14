#version 460 core

uniform mat4 transform_matrix;
uniform vec3 lightPos;

in vec3 vPosition;
in vec3 vColor;
in vec3 vNormal;

out vec3 color;
out vec3 normal;
out vec3 toLight;

void main() {
    vec3 lightPos = lightPos;

    vec4 position = vec4(vPosition, 1.0f);
    gl_Position = transform_matrix * position;

    // float val = abs(float(int(transform_matrix[2][3]) % 255) / 255);
    // color = vec3(val, val, val);
    color = vColor;
    normal = normalize(vNormal);
    toLight = normalize(lightPos - vPosition);
}

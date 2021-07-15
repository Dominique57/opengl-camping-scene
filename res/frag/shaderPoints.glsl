#version 460 core

in vec3 color;
out vec4 out_color;

void main() {
    vec2 temp = gl_PointCoord - vec2(0.5);
    float f = dot(temp, temp);
    if (f > 0.25)
        discard;
    out_color = vec4(temp, 1, 1);
    out_color = vec4(0.9, 0.5, 0.1, 1 - (f * 4));
    out_color = vec4(color, 1 - (f * 4));
}

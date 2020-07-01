#version 420
// ^nice^

in vec3 pass_normal;
in vec3 pass_fragPos;
in vec2 pass_uvCoords;

uniform sampler2D texture0;

out vec4 out_Color;

void main() {
    out_Color = texture2D(texture0, pass_uvCoords);
}

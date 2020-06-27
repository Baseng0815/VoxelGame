#version 420

in vec2 pass_uvCoords;

out vec4 out_Color;

uniform sampler2D texture0;

void main() {
    out_Color = texture(texture0, pass_uvCoords);
}

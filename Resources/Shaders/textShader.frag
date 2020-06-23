#version 420

in vec2 pass_texCoords;

out vec4 out_Color;

uniform sampler2D texture0;
uniform vec4 textColor;

void main() {
    vec4 textured = vec4(1, 1, 1, texture(texture0, pass_texCoords).r);
    out_Color = textured * textColor;
}

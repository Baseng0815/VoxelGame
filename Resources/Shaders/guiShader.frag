#version 420

in vec2 pass_texCoords;

out vec4 out_Color;

uniform sampler2D texture0;
uniform vec4 color;

// 0 ^= full color
uniform float textureVisibility;

void main() {
    vec4 textured = vec4(1, 1, 1, texture(texture0, pass_texCoords).r);
    out_Color = textureVisibility * textured + (1 - textureVisibility) * color;
}

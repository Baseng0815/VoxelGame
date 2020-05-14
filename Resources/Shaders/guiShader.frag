#version 420

in vec2 pass_texCoords;

out vec4 out_Color;

uniform sampler2D texture0;
uniform vec4 color;

// 0 ^= full color
uniform float blendFactor = 0;

void main() {
    vec4 textured = texture(texture0, pass_texCoords);
    // TODO add blending with texture
    //out_Color = mix(color, texture, blendFactor);
    out_Color = color;
}

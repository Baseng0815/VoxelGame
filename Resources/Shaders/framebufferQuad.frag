#version 420
in vec2 pass_uvCoords;

out vec4 color;

uniform sampler2D refractionTexture;
uniform sampler2D reflectionTexture;

void main() {
    color = texture(refractionTexture, pass_uvCoords);
}
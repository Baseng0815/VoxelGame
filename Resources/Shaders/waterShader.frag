#version 420
in vec3 fragCoords;
in vec3 fragNormal;
in vec2 fragTexCoods;

out vec4 color;

uniform sampler2D refractionTexture;
uniform sampler2D reflectionTexture;

void main() {
    color = vec4(0.0f, 0.2f, 0.5f, 1.0f);

    color = mix(color, texture(refractionTexture, fragTexCoods), 0.25);
}
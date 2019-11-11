#version 420
out vec4 out_Color;

in vec2 pass_uvCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

struct Light {
    vec3 position;
    vec3 color;
};

const int NR_LIGHTS = 16;
uniform Light lights[NR_LIGHTS];

void main() {
    // receive data from gBuffer
    vec3 fragPos = texture2D(gPosition, pass_uvCoords).rgb;
    vec3 normal = texture2D(gNormal, pass_uvCoords).rgb;
    vec3 albedo = texture2D(gAlbedo, pass_uvCoords).rgb;

    // 0.1 ^= ambient component
    vec3 lighting = albedo * 0.1;

    for (int i = 0; i < NR_LIGHTS; i++) {
        vec3 lightDir = normalize(lights[i].position - fragPos);
        vec3 diffuse = max(dot(normal, lightDir), 0) * albedo * lights[i] * color;
    }

    out_Color = vec4(lighting, 1);

}

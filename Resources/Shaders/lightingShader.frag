#version 420
out vec4 out_Color;

in vec2 pass_uvCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;

struct Light {
    vec3 dir;
    vec3 color;
};

const int NR_LIGHTS = 1;
uniform Light lights[NR_LIGHTS];

void main() {
    // receive data from gBuffer
    vec3 fragPos = texture(gPosition, pass_uvCoords).rgb;
    vec3 normal = texture(gNormal, pass_uvCoords).rgb;
    vec3 albedo = texture(gAlbedo, pass_uvCoords).rgb;

    // 0.1 ^= ambient component
    vec3 lighting = albedo * 0.1;

    for (int i = 0; i < NR_LIGHTS; i++) {            
        vec3 diffuse = max(dot(normal, normalize(-lights[i].dir)), 0) * albedo * lights[i].color;
        lighting += diffuse;
    }

    out_Color = vec4(lighting, 1);
}

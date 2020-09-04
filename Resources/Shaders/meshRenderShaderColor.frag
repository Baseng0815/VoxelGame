#version 420

in vec3 pass_normal;
in vec3 pass_fragPos;
// in vec2 pass_uvCoords;

struct Material {
    vec4 diffuse;
    vec4 ambient;
    vec4 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

uniform vec3 viewPos;
uniform Light light;
uniform Material material;

out vec4 out_Color;

void main() {
    // diffuse
    vec3 norm = normalize(pass_normal);
    vec3 lightDir = normalize(light.position - pass_fragPos);
    float diff = max(dot(norm, lightDir), 0.0f);

    // specular
    vec3 viewDir = normalize(viewPos - pass_fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32.0f);

    // combine
    vec4 ambient = light.ambient * material.ambient;
    vec4 diffuse = light.diffuse * (diff * material.diffuse);
    vec4 specular = light.specular * (spec * material.specular);

    out_Color = vec4(ambient + diffuse + specular);
}

#version 420

in vec3 pass_normal;
in vec3 pass_fragPos;
in vec2 pass_uvCoords;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float shininess;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant, linear, quadratic;
};

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define MAX_LIGHTS 1

uniform vec3 viewPos;
uniform PointLight pointLights[MAX_LIGHTS];
uniform DirectionalLight dirLight;
uniform Material material;

out vec4 out_Color;

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

void main() {
    vec3 norm = normalize(pass_normal);
    vec3 viewDir = normalize(viewPos - pass_fragPos);

    // directional light
    vec3 result = calcDirLight(dirLight, norm, viewDir);
    // point lights
    /*
    for (int i = 0; i < MAX_LIGHTS; i++)
        result += calcPointLight(pointLights[i], norm, viewDir, pass_fragPos);
    */

    out_Color = vec4(result, 1.0f);
}

// directional light
vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir) {
    // sampled colors
    vec3 diffuseColor = texture2D(material.diffuseMap, pass_uvCoords).xyz;
    vec3 specularColor = texture2D(material.specularMap, pass_uvCoords).xyz;

    // calculations
    // diffuse
    vec3 lightDir = normalize(light.direction);
    float diff = max(dot(normal, lightDir), 0.0f);

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    // combine
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;

    return ambient + diffuse + specular;
}

// point light
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    // sampled colors
    vec3 diffuseColor = texture2D(material.diffuseMap, pass_uvCoords).xyz;
    vec3 specularColor = texture2D(material.specularMap, pass_uvCoords).xyz;

    // calculations
    // diffuse
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0f);

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    // attenuation
    float dist = length(light.position - pass_fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist);

    // combine
    vec3 ambient = light.ambient * diffuseColor;
    vec3 diffuse = light.diffuse * diff * diffuseColor;
    vec3 specular = light.specular * spec * specularColor;

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}
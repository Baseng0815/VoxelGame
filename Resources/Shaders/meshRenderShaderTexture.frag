#version 420

in vec3 pass_normal;
in vec3 pass_fragPos;
in vec2 pass_uvCoords;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;

    float shininess;
};

struct PointLight {
    vec3 position;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;

    float constant, linear, quadratic;
};

struct DirectionalLight {
    vec3 direction;

    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
};

#define MAX_POINT_LIGHTS $
#define MAX_DIR_LIGHTS   $

uniform vec3                viewPos;
uniform DirectionalLight    dirLights[MAX_DIR_LIGHTS];
uniform PointLight          pointLights[MAX_POINT_LIGHTS];
uniform Material            material;
uniform int                 dirLightCount;
uniform int                 pointLightCount;

out vec4 out_Color;

vec4 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec4 diffuseColor, vec4 specularColor);
vec4 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos, vec4 diffuseColor, vec4 specularColor);

void main()
{
    vec3 norm = normalize(pass_normal);
    vec3 viewDir = normalize(viewPos - pass_fragPos);

    // sampled colors
    vec4 diffuseColor = texture2D(material.diffuseMap, pass_uvCoords);
    vec4 specularColor = texture2D(material.specularMap, pass_uvCoords);

    if (diffuseColor.a < 0.1) {
        discard;
    }

    // dir lights
    vec4 result = vec4(0.f);
    for (int i = 0; i < dirLightCount; i++) {
        result += calcDirLight(dirLights[i], norm, viewDir, diffuseColor, specularColor);
    }

    // point lights
    for (int i = 0; i < pointLightCount; i++) {
        result += calcPointLight(pointLights[i], norm, viewDir, pass_fragPos, diffuseColor, specularColor);
    }

    out_Color = result;
}

// directional light
vec4 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec4 diffuseColor, vec4 specularColor)
{
    // calculations
    // diffuse
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(normal, lightDir), 0.0f);

    // specular
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);

    // combine
    // TODO upload vec4 directly
    vec4 ambient = light.ambient * diffuseColor;
    vec4 diffuse = light.diffuse * diff * diffuseColor;
    vec4 specular = light.specular * spec * specularColor;

    return ambient + diffuse + specular;
}

// point light
vec4 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos, vec4 diffuseColor, vec4 specularColor)
{
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
    vec4 ambient = light.ambient * diffuseColor;
    vec4 diffuse = light.diffuse * diff * diffuseColor;
    vec4 specular = light.specular * spec * specularColor;

    /* ambient *= attenuation; */
    /* diffuse *= attenuation; */
    /* specular *= attenuation; */

    return (ambient + diffuse + specular) * attenuation;
}

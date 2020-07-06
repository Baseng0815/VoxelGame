#version 420
// ^nice^

in vec3 pass_normal;
in vec3 pass_fragPos;
in vec2 pass_uvCoords;

struct Material {
    sampler2D diffuse;
    vec3 ambient;
    sampler2D specular;
};

uniform Material material;

out vec4 out_Color;

void main() {
    out_Color = texture2D(material.diffuse, pass_uvCoords);
}

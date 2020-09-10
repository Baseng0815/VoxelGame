#version 420

out vec4 out_Color;

struct Material {
    vec4 ambient;
};

uniform Material material;

void main()
{
    out_Color = material.ambient;
}

#version 420

out vec4 out_Color;

struct Material {
    vec4 color;
};

uniform Material material;

void main()
{
    out_Color = material.color;
}

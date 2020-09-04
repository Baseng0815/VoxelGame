#version 420

in vec3 pass_uvCoords;

out vec4 out_Color;

uniform samplerCube cubeMap;

void main() {
    out_Color = texture(cubeMap, pass_uvCoords);
}

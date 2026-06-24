#version 420

in vec3 pass_uvCoords;

out vec4 out_Color;

uniform samplerCube cubeMap;

void main() {
    vec4 result = texture(cubeMap, pass_uvCoords);

    const float gamma_inverse = 1.0 / 2.2;
    out_Color = vec4(pow(result.rgb, vec3(gamma_inverse)), result.a);
}

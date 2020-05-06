#version 420
// ^nice^

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedo;

in vec3 pass_normal;
in vec3 pass_fragPos;
in vec2 pass_uvCoords;

uniform sampler2D texture0;

out vec4 out_Color;

void main() {
    // store normal
    gNormal = normalize(pass_normal);

    // store position
    gPosition = pass_fragPos;

    // store per-fragment color
    gAlbedo = texture2D(texture0, pass_uvCoords);
}

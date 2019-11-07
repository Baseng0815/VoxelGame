#version 420
// ^nice^

in vec3 pass_normal;
in vec2 pass_uvCoords;
in vec3 pass_fragPos;

out vec4 out_Color;

void main() {
	out_Color = vec4(1, 1, 1, 1);
}
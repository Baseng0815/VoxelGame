#version 420
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvCoords;

out vec2 pass_uvCoords;

void main() {
	pass_uvCoords = uvCoords;
	gl_Position = vec4(position, 1);
}
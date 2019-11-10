#version 420

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvCoords;

out vec3 pass_normal;
out vec3 pass_fragPos;
out vec2 pass_uvCoords;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

void main() {
	pass_fragPos = vec3(modelMatrix * vec4(position, 1.f));

	pass_normal = mat3(transpose(inverse(modelMatrix))) * normal;
	pass_uvCoords = uvCoords;

	gl_Position = projectionMatrix * viewMatrix * vec4(pass_fragPos, 1.f);
}
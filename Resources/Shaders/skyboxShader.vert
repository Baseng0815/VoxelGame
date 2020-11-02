#version 420

layout (location = 0) in vec3 position;

out vec3 pass_uvCoords;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1);
    pass_uvCoords = position;
}

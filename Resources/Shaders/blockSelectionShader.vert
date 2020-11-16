#version 420

layout (location = 0) in vec3 vertex;

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform vec3 selectedBlockPos;

void main() {
    gl_Position = projectionMatrix * viewMatrix * vec4(vertex + selectedBlockPos, 1.0);
}
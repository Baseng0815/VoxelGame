#version 420

layout (location = 0) in vec3 position;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

void main() {
    vec4 worldPos = modelMatrix * vec4(position, 1.f);

    gl_Position = projectionMatrix * viewMatrix * worldPos;
}

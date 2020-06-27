#version 420
layout (location = 0) in vec4 vertex; // vec2 pos; vec2 uv

out vec2 pass_uvCoords;

uniform mat4 projectionMatrix;

void main() {
    gl_Position = projectionMatrix * vec4(vertex.xy, 0.0, 1.0);
    pass_uvCoords = vertex.zw;
}

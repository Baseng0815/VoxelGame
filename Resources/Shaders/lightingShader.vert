#version 420
layout (location = 0) in vec4 vertex; // vec2 pos; vec2 uv

out vec2 pass_uvCoords;

void main() {
    gl_Position = vec4(vertex.xy, 0, 1);
    pass_uvCoords = vertex.zw;
}

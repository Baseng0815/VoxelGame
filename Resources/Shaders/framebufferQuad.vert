#version 420
layout (location = 0) in vec4 vertex; // vec2 pos; vec2 uv

out vec2 pass_uvCoords;

void main() {
    gl_Position = vec4(vertex.xy, 0.0, 1.0);
    pass_uvCoords = vec2(vertex.x / 2 + 0.5f, vertex.y / 2 + 0.5f);    
}
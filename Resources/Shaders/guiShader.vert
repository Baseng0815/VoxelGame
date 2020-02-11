#version 420

layout (location = 0) in vec4 vertex; // vec2 pos; vec2 uv

out vec2 pass_texCoords;

void main() {
	vec2 pos = vertex.xy - vec2(400, 300);
	pos /= vec2(400, 300);
	gl_Position = vec4(pos.xy, 0.0, 1.0);
	pass_texCoords = vertex.zw;
}
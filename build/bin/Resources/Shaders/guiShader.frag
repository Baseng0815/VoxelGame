#version 420

in vec2 pass_texCoords;

out vec4 out_Color;

uniform sampler2D text;
uniform vec3 textColor;

void main() {
	vec4 sampled = vec4(1, 1, 1, texture(text, pass_texCoords).r);
	out_Color = vec4(textColor, 1) * sampled;
}
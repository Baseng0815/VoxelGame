#version 420
// ^nice^

in vec3 pass_normal;
in vec3 pass_fragPos;
in vec2 pass_uvCoords;

out vec4 out_Color;

uniform struct DirectionalLight {
	vec3 dir;
	vec3 color;
	float ambientStrength;
} directionalLight;

uniform sampler2D texture0;

void main() {
	vec3 color = texture2D(texture0, pass_uvCoords).xyz;

	vec3 ambient = directionalLight.ambientStrength * directionalLight.color;

	vec3 norm = normalize(pass_normal);
	float diff = max(dot(norm, directionalLight.dir), 0);
	vec3 diffuse = diff * directionalLight.color;

	out_Color = vec4((ambient + diffuse) * color, 1);
}
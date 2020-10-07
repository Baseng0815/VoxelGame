#version 420
in vec2 pass_uvCoords;

out vec4 color;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main() {
    vec4 sceneColor = texture2D(texture0, pass_uvCoords);
    vec4 waterColor = texture2D(texture1, pass_uvCoords);

    float alpha = waterColor.w;

    color = (1 - alpha) * sceneColor + alpha * waterColor;
}
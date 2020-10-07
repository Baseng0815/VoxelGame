#version 420
in vec2 pass_uvCoords;

out vec4 color;

uniform sampler2D sceneTexture;
uniform sampler2D waterTexture;

uniform sampler2D sceneDepth;
uniform sampler2D waterDepth;

void main() {
    vec4 sceneColor = texture2D(sceneTexture, pass_uvCoords);
    vec4 waterColor = texture2D(waterTexture, pass_uvCoords);

    float alpha = waterColor.w;
        
    // float sceneDepthTexel = texture2D(sceneDepth, pass_uvCoords).z;
    // float waterDepthTexel = texture2D(waterDepth, pass_uvCoords).z;

    color = (1 - alpha) * sceneColor + alpha * waterColor;    
}
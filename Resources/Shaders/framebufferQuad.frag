#version 420
in vec2 pass_uvCoords;

out vec4 color;

uniform sampler2D sceneTexture;
uniform sampler2D waterTexture;

uniform sampler2D sceneDepthTexture;
uniform sampler2D waterDepthTexture;

void main() {
    vec4 sceneColor = texture2D(sceneTexture, pass_uvCoords);
    vec4 waterColor = texture2D(waterTexture, pass_uvCoords);

    float alpha = waterColor.w;
        
    vec4 sceneDepth = texture2D(sceneDepthTexture, pass_uvCoords);
    vec4 waterDepth = texture2D(waterDepthTexture, pass_uvCoords);

    if (sceneDepth.z > waterDepth.z) {
        color = (1 - alpha) * sceneColor + alpha * waterColor;                
        if(sceneColor.w > 0.1) {    
            color = vec4(color.xyz, 1.0);
        }    
    }
    else {
        color = sceneColor;
    }    
}
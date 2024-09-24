#version 400 core
out vec4 FragColor;

in vec2 fragUv;
uniform vec4 color;
uniform vec2 uvOffset = vec2(0.0);


uniform bool bUseTexture = false;
uniform sampler2D texture1;


vec2 clampUv(vec2 InSourceUv,vec2 InOffset)
{
    vec2 resultUv = fract(InSourceUv + InOffset);
    return abs( resultUv);
}


void main() 
{
   
    if(bUseTexture)
    {
        FragColor = texture(texture1,clampUv(fragUv,uvOffset));
    }
    else
    {
        FragColor = color;
    }
}
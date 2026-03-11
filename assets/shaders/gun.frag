#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform bool uUseTexture;

void main()
{
    if (uUseTexture)
        FragColor = texture(uTexture, vTexCoord);
    else
        FragColor = vec4(0.8, 0.6, 0.4, 1.0);
}

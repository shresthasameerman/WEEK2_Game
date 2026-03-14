#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform bool uUseTexture;
uniform vec3 uTint;

void main()
{
    vec4 texColor = texture(uTexture, vTexCoord);
    if (texColor.a < 0.1) discard;
    if (uUseTexture)
        FragColor = vec4(texColor.rgb * uTint, texColor.a);
    else
        FragColor = vec4(0.8 * uTint, 1.0);
}

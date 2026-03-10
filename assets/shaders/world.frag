#version 330 core

in  vec3 vColor;
out vec4 FragColor;

const float fogStart = 2.0f;
const float fogEnd   = 8.0f;
const vec3  fogColor = vec3(0.15f, 0.15f, 0.15f); // grey fog

void main()
{
    float depth = gl_FragCoord.z / gl_FragCoord.w;
    float fogFactor = clamp((fogEnd - depth) / (fogEnd - fogStart), 0.0f, 1.0f);

    vec3 finalColor = mix(fogColor, vColor, fogFactor);
    FragColor = vec4(finalColor, 1.0);
}
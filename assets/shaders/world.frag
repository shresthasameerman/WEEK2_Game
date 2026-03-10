#version 330 core

in  vec3 vColor;
out vec4 FragColor;

uniform float uTime;

const float fogStart = 2.0f;
const float fogEnd   = 8.0f;
const vec3  fogColor = vec3(0.15f, 0.15f, 0.15f);

void main()
{
    // flickering torch light
    float flicker = sin(uTime * 7.0f) * 0.03f
    + sin(uTime * 13.0f) * 0.02f
    + sin(uTime * 31.0f) * 0.01f;

    vec3 litColor = vColor * (1.0f + flicker);

    // fog
    float depth = gl_FragCoord.z / gl_FragCoord.w;
    float fogFactor = clamp((fogEnd - depth) / (fogEnd - fogStart), 0.0f, 1.0f);

    vec3 finalColor = mix(fogColor, litColor, fogFactor);
    // vignette - dark edges
    vec2 uv = gl_FragCoord.xy / vec2(1920.0f, 1080.0f);
    vec2 vigCoord = uv * (1.0f - uv.yx);
    float vignette = pow(vigCoord.x * vigCoord.y * 15.0f, 0.4f);
    finalColor *= vignette;
    FragColor = vec4(finalColor, 1.0);
}
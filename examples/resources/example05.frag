#version 430 core

out vec4 FragColor;

uniform float time;
uniform vec2 res;

void main()
{
    vec2 uv = gl_FragCoord.xy / res;
    float c = sin(time) * 0.5 + 0.5;
    FragColor = vec4(uv, sin(time + uv.x * 3.14) * 0.5 + 0.5, 1.);
}

#version 430 core

out vec3 color;

uniform sampler2D tex0;
uniform vec2 res;

void main(){
    vec2 UV = gl_FragCoord.xy / res;
    color = texture( tex0, UV).xyz;
}

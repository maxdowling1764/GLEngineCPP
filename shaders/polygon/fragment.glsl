#version 330 core
in vec4 position;
in vec3 norm;

uniform float time;
vec3 lightdir = vec3(0.0, 0.0, 1.0);
void main()
{
    float brightness = max(0.0, dot(norm, lightdir));
    gl_FragColor = vec4(vec3(brightness), 1.0);
}
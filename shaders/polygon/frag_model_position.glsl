#version 330 core
in vec4 position;
in vec3 norm;
in vec2 uv;

uniform float time;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_FragColor = vec4(position.xyz, 1.0);
}
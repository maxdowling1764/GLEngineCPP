#version 330 core
in vec4 position;
uniform float time;
void main()
{
    gl_FragColor = vec4(position.xyz, 1.0);
}
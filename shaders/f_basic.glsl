#version 330 core

in vec3 pos;
in vec3 norm;
in vec2 tex;

vec3 lightpos = vec3(3.0f, 0.0f, 3.0f);

//uniform sampler2D textureSampler;

void main()
{
    float intensity = dot(normalize(lightpos - pos), - norm) + 0.2;
    gl_FragColor = vec4(intensity*norm, 1.0f);
}
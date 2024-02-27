#version 330 core
in vec4 position;
in vec3 norm;
in vec2 uv;

uniform float time;

uniform sampler2D diffuse;

vec3 lightdir = vec3(0.0, 0.0, 1.0);
void main()
{
    vec4 diffuseColor = texture2D(diffuse, uv);//vec4(uv, 1.0, 1.0);
    float brightness = max(0.0, dot(norm, lightdir));
    gl_FragColor = vec4(brightness * diffuseColor.xyz, diffuseColor.w);
}
#version 330 core

layout (location = 0) in vec3 v;
layout (location = 1) in vec3 vn;
layout (location = 2) in vec2 vt;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec4 position;
out vec2 uv;

void main()
{
	position = vec4(v.xyz, 1.0);
	uv = vt;
	gl_Position = position;
}
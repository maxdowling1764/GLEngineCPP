#version 330 core

layout (location = 0) in vec3 v;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

out vec4 position;

void main()
{
	position = vec4(v.x, v.y, v.z, 1.0); 
	gl_Position = projection * view * model *position;
}
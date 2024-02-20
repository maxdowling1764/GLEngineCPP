#version 330 core
uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

layout(location = 0) in vec3 v;

out vec4 position;

void main()
{
	vec4 vertex_position = vec4(v, 1.0);
	position = vertex_position;
	mat4 pvm = projection * view * model;
	gl_Position = pvm * vertex_position;
}
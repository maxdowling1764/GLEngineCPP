#version 330 core

uniform vec3 cam_up;
uniform vec3 cam_forward;
uniform vec3 cam_pos;
uniform mat4 model;

layout (location = 0) in vec3 v;

uniform mat4 projection;
// uniform mat4 view;


out vec4 position;

void main()
{
	vertex_position = vec4(v, 1.0);
	vec3 cam_right = cross(cam_up, cam_forward);
	vec4 view_x = vec4(cam_right.x, cam_up.x, cam_forward.x, cam_pos.x);
	vec4 view_y = vec4(cam_right.y, cam_up.y, cam_forward.y, cam_pos.y);
	vec4 view_z = vec4(cam_right.z, cam_up.z, cam_forward.z, cam_pos.z);

	mat4 view = mat4(view_x, view_y, view_z, vec4(vec3(0), 1.0));
	mat4 vpm = projection * view * model;
	gl_Position = vpm * vertex_position;
}
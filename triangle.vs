#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 vertex_UV;

uniform mat4 MVP;

out vec3 fragment_pos;
out vec2 UV;

void main()
{
	gl_Position = MVP * vec4(pos, 1);
	
	fragment_pos = pos;
	UV = vertex_UV;
}
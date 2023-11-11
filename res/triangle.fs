#version 330 core

in vec3 fragment_pos;
in vec2 UV;

out vec3 color;

uniform sampler2D texture_sampler;

void main()
{
//	color = fragment_pos;
	color = texture(texture_sampler, UV).rgb;
}
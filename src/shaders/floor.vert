R"zzz(#version 330 core
in vec4 vertex_position;
uniform vec4 offset;
uniform mat4 view;
uniform mat4 projection;
out vec4 world_position;
void main()
{
	gl_Position =  projection * view * (vertex_position + offset);
	world_position =  vertex_position + offset;
	
}
)zzz";
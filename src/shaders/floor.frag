R"zzz(#version 330 core
in vec4 world_position;
out vec4 fragment_color;
void main()
{
	fragment_color = vec4(0.2,0.4, 0.4, 0.001);
}
)zzz";
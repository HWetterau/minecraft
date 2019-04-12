R"zzz(
#version 330 core
uniform vec4 light_position;
uniform vec3 camera_position;
in vec4 vertex_position;

void main() {
	gl_Position = vertex_position;
}
)zzz"
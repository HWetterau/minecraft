R"zzz(
#version 330 core
uniform mat4 view;
uniform mat4 projection;
in vec4 vertex_position;
uniform float time;

void main() {
	int t = int(time * 40);
	vec4 position = vertex_position;
	position.z += t;
	gl_Position = projection * view * position;
   
}
)zzz"

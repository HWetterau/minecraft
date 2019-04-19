R"zzz(
#version 330 core
uniform mat4 view;
uniform mat4 projection;
in vec3 vertex_position;
out vec4 world_pos;

void main() {
    world_pos = vec4(vertex_position, 1);
    vec4 pos = projection* view * vec4(vertex_position, 1);
	
	gl_Position = pos.xyww;


   
}
)zzz"

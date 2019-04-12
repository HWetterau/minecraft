R"zzz(
#version 330 core
uniform vec4 light_position;
//uniform vec3 camera_position;
uniform vec4 offsets[900];
uniform mat4 projection;
uniform mat4 view;
in vec4 vertex_position;

void main() {
    vec4 offset = offsets[gl_InstanceID];
	gl_Position = projection * view * (offset +vertex_position);
}
)zzz"
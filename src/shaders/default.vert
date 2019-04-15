R"zzz(
#version 330 core
//uniform vec3 camera_position;
uniform vec4 offsets[900];

in vec4 vertex_position;


void main() {
    vec4 offset = offsets[gl_InstanceID];
	gl_Position = (offset +vertex_position);
   
}
)zzz"
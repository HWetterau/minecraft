
R"zzz(#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform mat4 projection;
uniform mat4 view;
uniform vec4 light_position;
flat out vec4 normal;
out vec4 light_direction;
void main()
{
	vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[1].gl_Position) - vec3(gl_in[2].gl_Position);
	normal = vec4(normalize(cross(a,b)), 0);
	for (int n = 0; n < gl_in.length(); n++) {
		light_direction = normalize(light_position - gl_in[n].gl_Position);
		gl_Position = projection * view * gl_in[n].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
}
)zzz";
R"zzz(
#version 330 core
flat in vec4 normal;
in vec4 light_direction;
in vec4 world_pos;
out vec4 fragment_color;
void main() {
	vec4 color;
	if(world_pos.y < 0.0001){ 
		color = vec4(0.5, 0.5, 0.5, 1.0);
	} else if(world_pos.y < 2.0001){
		color = vec4(0.3, 0.2, 0.0, 1.0);
	}else{
		color = vec4(0.02,0.9,0.0,1.0);
	}
		float dot_nl = dot(normalize(light_direction), normalize(normal));
		dot_nl = clamp(dot_nl, 0.0, 1.0);
	fragment_color = dot_nl * color ;
})zzz"
R"zzz(
#version 420 core
flat in vec4 normal;
uniform int p[512];
in vec4 light_direction;
in vec4 world_pos;
out vec4 fragment_color;
float fade (float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float t, float a, float b) { return a + t * (b - a); }
float grad(int hash, float x, float z)
{
    switch(hash & 0x3)
    {
        case 0x0: return  x + z;
        case 0x1: return -x + z;
        case 0x2: return  x - z;
        case 0x3: return -x - z;
        default: return 0; // never happens
    }
}

void main() {

	int X_floor = int(floor(world_pos.x));
	int Z_floor = int(floor(world_pos.z));
	int X = X_floor & 255;
	int Z =	Z_floor & 255;

	float x = world_pos.x - floor(world_pos.x);
	float z = world_pos.z - floor(world_pos.z);

	float u = fade(x);
	float w = fade(z);



	vec4 color;
	if(world_pos.y < 0.0001){ 
		int A = p[X  ], AA = p[A]+Z, AB = p[A+1]+Z,      // HASH COORDINATES OF
		B = p[X+1], BA = p[B]+Z, BB = p[B+1]+Z;      // THE 8 CUBE CORNERS,

		float result = lerp(w, lerp(u, grad(p[AA  ], x  , z   ),  
						grad(p[BA  ], x-1  , z   )),
				lerp(u, grad(p[AA+1], x   , z-1 ),  
						grad(p[BA+1], x-1  , z-1 )));
		
		result = (result + 1) / 2;
		//result += (0.25 *sin(x - z));
		vec4 mult = vec4(result, result, result,1);
		color = mult * vec4(0.5, 0.5, 0.5, 1.0);
	} else if(world_pos.y < 2.0001){
		color = vec4(0.3, 0.2, 0.0, 1.0);
	}else{
		color = vec4(0.02,0.9,0.0,1.0);
	}
		float dot_nl = dot(normalize(light_direction), normalize(normal));
		dot_nl = clamp(dot_nl, 0.0, 1.0);
	fragment_color = dot_nl * clamp(color, 0.0, 1.0) ;
})zzz"
R"zzz(
#version 410 core
uniform int p[512];
uniform float t;
in vec4 world_pos;
out vec4 fragment_color;

float fade (float t) {
	return t * t * t * (t * (t * 6 - 15) + 10);
}

float lerp(float t, float a, float b) { return a + t * (b - a); }
float grad(int hash, float x, float y, float z)
{
	switch(hash & 0xF)
		{
				case 0x0: return  x + y;
				case 0x1: return -x + y;
				case 0x2: return  x - y;
				case 0x3: return -x - y;
				case 0x4: return  x + z;
				case 0x5: return -x + z;
				case 0x6: return  x - z;
				case 0x7: return -x - z;
				case 0x8: return  y + z;
				case 0x9: return -y + z;
				case 0xA: return  y - z;
				case 0xB: return -y - z;
				case 0xC: return  y + x;
				case 0xD: return -y + z;
				case 0xE: return  y - x;
				case 0xF: return -y - z;
				default: return 0; 
		}
}

float perlin(float xx, float yy, float zz){
	int X_floor = int(floor(xx));
	int Y_floor = int(floor(yy));
	int Z_floor = int(floor(zz));
	int X = X_floor & 255;
	int Y = Y_floor & 255;
	int Z =	Z_floor & 255;

	float x = xx - floor(xx);
	float y = yy - floor(yy);
	float z = zz - floor(zz);

	 float u = fade(x), v = fade(y), w = fade(z);
	 int A = p[X ]+Y, AA = p[A]+Z, AB = p[A+1]+Z,      
		 B = p[X+1]+Y, BA = p[B]+Z, BB = p[B+1]+Z;     

	 float result = lerp(w, lerp(v, lerp(u, grad(p[AA  ], x  , y  , z   ),  
											grad(p[BA  ], x-1, y  , z   )), 
									lerp(u, grad(p[AB  ], x  , y-1, z   ),  
											grad(p[BB  ], x-1, y-1, z   ))),
							lerp(v, lerp(u, grad(p[AA+1], x  , y  , z-1 ), 
											grad(p[BA+1], x-1, y  , z-1 )), 
									lerp(u, grad(p[AB+1], x  , y-1, z-1 ),
											grad(p[BB+1], x-1, y-1, z-1 ))));

		result = (result + 1) / 2;
		return result;
}

float octaves(float x, float y, float z, int octaves, float persistence) {
    float total = 0;
    float frequency = 1;
    float amplitude = 1;
    float maxValue = 0;  
    for(int i=0;i<octaves;i++) {
        total += perlin(x * frequency, y * frequency, z * frequency) * amplitude;

        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= 2;
    }
    return total/maxValue;
}

void main() {
	
	vec4 color;

		float result = octaves(world_pos.x,world_pos.y,world_pos.z,3, 0.75);
	
		//result = (sin(world_pos.x*10 - world_pos.z*10 + 20 * result));
		
		result = (result + 1) / 2;
		vec4 mult = vec4(result, result, result,1);
		if (t > 0) {
			color = mult * vec4(0.7, 0.7 , 0.9 + t, 1.0);
		}
		if (t < 0) {
			color = mult * vec4(0.7 + t, 0.7 + t, 0.9 + t, 1.0);
		}
		//color += vec4(0.2, 0.2, 0.2, 1.0);

	fragment_color = clamp(color, 0.0, 1.0) ;
	//fragment_color = vec4(0.1, 0.05, 0.4, 1.0);
})zzz"

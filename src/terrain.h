#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <stdlib.h>     /* srand, rand */
#include <iostream>
#include <algorithm>

using namespace std;

int chunkSize = 30;

class Terrain {
public:


int p[512] = { 151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
   ,151,160,137,91,90,15,
   131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
   190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
   88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
   77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
   102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
   135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
   5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
   223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
   129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
   251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
   49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
   138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
   };

    std::vector<glm::vec4> up_left_offsets;
    std::vector<glm::vec4> up_center_offsets;
    std::vector<glm::vec4> up_right_offsets;
    std::vector<glm::vec4> left_offsets;
    std::vector<glm::vec4> center_offsets;
    std::vector<glm::vec4> right_offsets;
    std::vector<glm::vec4> down_left_offsets;
    std::vector<glm::vec4> down_center_offsets;
    std::vector<glm::vec4> down_right_offsets;
    int big_grid [30][30];


void generate(glm::vec4 world_pos) {

    int seed = floor(world_pos.x/chunkSize) * 653 + floor(world_pos.z/chunkSize); //just chose an arbitrary prime number

    srand(seed);

    //figure out caching or something later
    up_left_offsets.clear();
    up_center_offsets.clear();
    up_right_offsets.clear();
    left_offsets.clear();
    center_offsets.clear();
    right_offsets.clear();
    down_left_offsets.clear();
    down_center_offsets.clear();
    down_right_offsets.clear();
    float amplitude = 4;
	glm::vec2 direction = glm::vec2(2,3);
	float w = .03;
	// float speed = 0.003;
	float amplitude_2 = .3;
	// vec2 direction_2 = vec2(-3,-1);
	// float w_2 = 0.3;
	// float speed_2 = 0.002;
    //center chunk
    int startx = floor(world_pos.x/chunkSize) * chunkSize;
    int startz = floor(world_pos.z/chunkSize) * chunkSize;
    int bedrock = 0;
    int min = 0;
    int max = chunkSize;
    int index = 0;
    cout<<endl;
    cout<<"center: "<<glm::to_string(world_pos+glm::vec4(0,0,0,0))<<endl<<endl;
    compute(world_pos);
    for(int x = min; x < max; ++x){
        for(int z = min; z < max; ++z ){
                // float height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx, z + startz)) * w ));
                // int clampHeight = ceil(height);

                //center_offsets.push_back(glm::vec4(x + startx, clampHeight + bedrock, z +  startz, 0));
                int clampHeight = glm::clamp(big_grid[x][z],-8,8);
                center_offsets.push_back(glm::vec4(x + startx, clampHeight + bedrock, z +  startz, 0));

                // height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx-chunkSize, z + startz-chunkSize)) * w ));
                // up_left_offsets.push_back(glm::vec4(x + startx - chunkSize, ceil(height) + bedrock, z +  startz - chunkSize, 0));
                // height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx, z + startz-chunkSize)) * w ));
                // up_center_offsets.push_back(glm::vec4(x + startx, ceil(height) + bedrock, z +  startz - chunkSize, 0));
                // height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx+chunkSize, z + startz-chunkSize)) * w ));
                // up_right_offsets.push_back(glm::vec4(x + startx + chunkSize, ceil(height) + bedrock, z +  startz - chunkSize, 0));
                
                // height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx-chunkSize, z + startz)) * w ));
                // left_offsets.push_back(glm::vec4(x + startx - chunkSize, ceil(height) + bedrock, z +  startz, 0));
                // height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx+chunkSize, z + startz)) * w ));
                // right_offsets.push_back(glm::vec4(x + startx + chunkSize, ceil(height) + bedrock, z +  startz, 0));

                // height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx-chunkSize, z + startz+chunkSize)) * w ));
                // down_left_offsets.push_back(glm::vec4(x + startx - chunkSize, ceil(height) + bedrock, z +  startz + chunkSize, 0));
                // height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx, z + startz+chunkSize)) * w ));
                // down_center_offsets.push_back(glm::vec4(x + startx, ceil(height) + bedrock, z +  startz + chunkSize, 0));
                // height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx+chunkSize, z + startz+chunkSize)) * w ));
                // down_right_offsets.push_back(glm::vec4(x + startx + chunkSize, ceil(height) + bedrock, z +  startz + chunkSize, 0));
                
                // for(int i = 0; i < clampHeight; ++i){
                //     offsets.push_back(glm::vec4(x, i + bedrock, z, 0));
                // }                         
        }
    }
    // cout<<endl;
    // cout<<"left: "<<glm::to_string(world_pos+glm::vec4(-chunkSize,0,0,0))<<endl<<endl;
    compute(world_pos+glm::vec4(-chunkSize,0,0,0));
     for(int x = min; x < max; ++x){
        for(int z = min; z < max; ++z ){
            int clampHeight = glm::clamp(big_grid[x][z],-8,8);
            left_offsets.push_back(glm::vec4(x + startx - chunkSize,clampHeight + bedrock, z +  startz, 0));

        }
    }
    // // cout<<endl;
    // //  cout<<"right: "<<glm::to_string(world_pos+glm::vec4(chunkSize,0,0,0))<<endl<<endl;
    compute(world_pos+glm::vec4(chunkSize,0,0,0));
     for(int x = min; x < max; ++x){
        for(int z = min; z < max; ++z ){
            int clampHeight = glm::clamp(big_grid[x][z],-8,8);
            right_offsets.push_back(glm::vec4(x + startx + chunkSize,clampHeight + bedrock, z +  startz, 0));

        }
    }
    // // cout<<endl;
    // // cout<<"up_left: "<<glm::to_string(world_pos+glm::vec4(-chunkSize,-chunkSize,0,0))<<endl<<endl;
    compute(world_pos+glm::vec4(-chunkSize,0,-chunkSize,0));
     for(int x = min; x < max; ++x){
        for(int z = min; z < max; ++z ){
            int clampHeight = glm::clamp(big_grid[x][z],-8,8);
            up_left_offsets.push_back(glm::vec4(x + startx - chunkSize,clampHeight + bedrock, z +  startz - chunkSize, 0));

        }
    }
    // // cout<<endl;
    // // cout<<"up_center: "<<glm::to_string(world_pos+glm::vec4(0,-chunkSize,0,0))<<endl<<endl;
    compute(world_pos+glm::vec4(0,0,-chunkSize,0));
     for(int x = min; x < max; ++x){
        for(int z = min; z < max; ++z ){
            int clampHeight = glm::clamp(big_grid[x][z],-8,8);
            up_center_offsets.push_back(glm::vec4(x + startx ,clampHeight + bedrock, z +  startz - chunkSize, 0));

        }
    }
    // // cout<<endl;
    // // cout<<"up_right: "<<glm::to_string(world_pos+glm::vec4(chunkSize,-chunkSize,0,0))<<endl<<endl;
    compute(world_pos+glm::vec4(chunkSize,0,-chunkSize,0));
     for(int x = min; x < max; ++x){
        for(int z = min; z < max; ++z ){
            int clampHeight = glm::clamp(big_grid[x][z],-8,8);
            up_right_offsets.push_back(glm::vec4(x + startx + chunkSize, clampHeight + bedrock, z +  startz- chunkSize, 0));

        }
    }
    // // cout<<endl;
    // // cout<<"down_left: "<<glm::to_string(world_pos+glm::vec4(-chunkSize,chunkSize,0,0))<<endl<<endl;
    compute(world_pos+glm::vec4(-chunkSize,0,chunkSize,0));
     for(int x = min; x < max; ++x){
        for(int z = min; z < max; ++z ){
            int clampHeight = glm::clamp(big_grid[x][z],-8,8);
            down_left_offsets.push_back(glm::vec4(x + startx - chunkSize,clampHeight + bedrock, z +  startz + chunkSize, 0));

        }
    }
    // // cout<<endl;
    // // cout<<"down_center: "<<glm::to_string(world_pos+glm::vec4(0,chunkSize,0,0))<<endl<<endl;
    compute(world_pos+glm::vec4(0,0,chunkSize,0));
     for(int x = min; x < max; ++x){
        for(int z = min; z < max; ++z ){
            int clampHeight = glm::clamp(big_grid[x][z],-8,8);
            down_center_offsets.push_back(glm::vec4(x + startx ,clampHeight + bedrock, z +  startz + chunkSize, 0));

        }
    }
    // // cout<<endl;
    // // cout<<"down_right: "<<glm::to_string(world_pos+glm::vec4(chunkSize,chunkSize,0,0))<<endl<<endl;
    compute(world_pos+glm::vec4(chunkSize,0,chunkSize,0));
     for(int x = min; x < max; ++x){
        for(int z = min; z < max; ++z ){
            int clampHeight = glm::clamp(big_grid[x][z],-8,8);
            down_right_offsets.push_back(glm::vec4(x + startx + chunkSize, clampHeight + bedrock, z +  startz+ chunkSize, 0));

        }
    }


    
	// float height_2 = amplitude_2 * sin(dot(direction_2, vec2(offset[0], offset[2])) * w_2 + time * speed_2);

}
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

float perlin(float x_coord, float y_coord, float z_coord){
    y_coord = 0;
    //which chunk
	int X = (int)floor(x_coord/chunkSize) & 255;
	int Y = (int)floor(y_coord/chunkSize) & 255;
	int Z =	(int)floor(z_coord/chunkSize) & 255;
    
    //relative position in chunk
	float x = x_coord/chunkSize - floor(x_coord/chunkSize);
	float y = y_coord/chunkSize - floor(y_coord/chunkSize);
	float z = z_coord/chunkSize - floor(z_coord/chunkSize);

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
    float amplitude =15;
    float maxValue = 0;  
    for(int i=0;i<octaves;i++) {
        total += perlin(x * frequency, y * frequency, z * frequency) * amplitude;

        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= 2;
    }
    //return total/maxValue;
    return total;
}

void compute(glm::vec4 world_pos){

    int x_start = floor(world_pos.x/chunkSize) * chunkSize;
    int z_start = floor(world_pos.z/chunkSize) * chunkSize;

    for(int x = 0; x < chunkSize; x++){
        for(int z = 0; z < chunkSize; z++){
            big_grid[x][z] =  octaves(x_start+x, 0.0, z_start+z, 3, 0.5) - 10;
        }
    }
}

void grid(glm::vec4 world_pos){
    
   
    int x = floor(world_pos.x/chunkSize);
    int z = floor(world_pos.z/chunkSize);
    int seed = x * 653 + z; //just chose an arbitrary prime number
        cout<<"seed "<<seed<<endl;
     //get edge vectors
    srand(seed);
    glm::vec2 aa = glm::normalize(glm::vec2(rand() % 10 + 1 ,rand() % 10 + 1 ));
    seed = (x + chunkSize) * 653 + z;
    srand(seed);
    glm::vec2 bb = glm::normalize(glm::vec2(rand() % 10 + 1 ,rand() % 10 + 1 ));
    seed = (x + chunkSize) * 653 + z + chunkSize;
    srand(seed);
    glm::vec2 cc = glm::normalize(glm::vec2(rand() % 10 + 1,rand() % 10 + 1 ));
    seed = x  * 653 + z + chunkSize;
    srand(seed);
    glm::vec2 dd = glm::normalize(glm::vec2(rand() % 10 + 1,rand() % 10 + 1));
   
    
    
    for(int u = 0; u < chunkSize; u++){
        for(int v = 0; v < chunkSize; v++){
            glm::vec2 uv = glm::vec2(((double)u)/chunkSize,((double)v)/chunkSize);
            glm::vec2 d = glm::vec2(0,1)-uv;
            glm::vec2 c = glm::vec2(1,1)-uv;
            glm::vec2 b = glm::vec2(1,0)-uv;
            glm::vec2 a = glm::vec2(0,0)-uv;
            // double result =  (1-((double)v)/chunkSize) * ((1-((double)u)/chunkSize)*glm::dot(aa,a)+ ((double)u)/chunkSize * glm::dot(bb,b)) + 
            //                 ((double)v)/chunkSize * ((1-((double)u)/chunkSize)* glm::dot(dd,d) + ((double)u)/chunkSize * glm::dot(cc,c));
            double result = lerp(lerp(glm::dot(aa,a),glm::dot(bb,b),fade(((double)u)/chunkSize)),
                                 lerp(glm::dot(dd,d),glm::dot(cc,c),fade(((double)u)/chunkSize)), 
                                 fade(((double)v)/chunkSize));
    
                            
            big_grid[u][v]= floor(result * 16);
            //cout<< result*16<<" ";
        }
        //cout << endl;
    }
    glm::vec2 dc = glm::normalize(glm::vec2(rand() % 10 + 1 ,rand() % 10 + 1 ));
    glm::vec2 da = glm::normalize(glm::vec2(rand() % 10 + 1 ,rand() % 10 + 1 ));
    glm::vec2 cb = glm::normalize(glm::vec2(rand() % 10 + 1 ,rand() % 10 + 1 ));
    glm::vec2 ab = glm::normalize(glm::vec2(rand() % 10 + 1 ,rand() % 10 + 1 ));
    glm::vec2 mid = glm::normalize(glm::vec2(rand() % 10 + 1 ,rand() % 10 + 1 ));
    int size = chunkSize/2;
    for(int u = 0; u < size; u++){
        for(int v = 0; v < size; v++){
            glm::vec2 uv = glm::vec2(((double)u)/size,((double)v)/size);
            glm::vec2 d = glm::vec2(0,1)-uv;
            glm::vec2 c = glm::vec2(1,1)-uv;
            glm::vec2 b = glm::vec2(1,0)-uv;
            glm::vec2 a = glm::vec2(0,0)-uv;
            // double result =  (1-((double)v)/chunkSize) * ((1-((double)u)/chunkSize)*glm::dot(aa,a)+ ((double)u)/chunkSize * glm::dot(bb,b)) + 
            //                 ((double)v)/chunkSize * ((1-((double)u)/chunkSize)* glm::dot(dd,d) + ((double)u)/chunkSize * glm::dot(cc,c));
            double result = lerp(lerp(glm::dot(aa,a),glm::dot(ab,b),fade(((double)u)/size)),
                                 lerp(glm::dot(da,d),glm::dot(mid,c),fade(((double)u)/size)), 
                                 fade(((double)v)/size));
            big_grid[u][v]+= floor(result * 4);
          
        }
    
    }

}




private:


};

#endif

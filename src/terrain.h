#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <stdlib.h>     /* srand, rand */
#include <iostream>

using namespace std;

int chunkSize = 9;

class Terrain {
public:
void generate(glm::vec4 world_pos, glm::vec4& offsets[]) {
    int seed = floor(world_pos.x/chunkSize) * 653 + floor(world_pos.z/chunkSize); //just chose an arbitrary prime number
    srand(seed);

    offsets.clear();
    float amplitude = 4;
	glm::vec2 direction = glm::vec2(2,3);
	float w = .03;
	// float speed = 0.003;
	float amplitude_2 = .3;
	// vec2 direction_2 = vec2(-3,-1);
	// float w_2 = 0.3;
	// float speed_2 = 0.002;
    int startx = floor(world_pos.x/chunkSize) * chunkSize;
    int startz = floor(world_pos.z/chunkSize) * chunkSize;
    int bedrock = -4;
    int min = 0;
    int max = chunkSize;
    int index = 0;
    for(int x = min; x < max; ++x){
        for(int z = min; z < max; ++z ){
                float height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx, z + startz)) * w ));
                int clampHeight = ceil(height);
                // for(int i = 0; i < clampHeight; ++i){
                //     offsets.push_back(glm::vec4(x, i + bedrock, z, 0));
                // }
                  //offsets.push_back(glm::vec4(x + startx, clampHeight + bedrock, z +  startz, 0));
                offsets[index]=glm::vec4(x + startx, clampHeight + bedrock, z +  startz, 0);
                index++;
        }
    }
    

	// float height_2 = amplitude_2 * sin(dot(direction_2, vec2(offset[0], offset[2])) * w_2 + time * speed_2);

}


private:


};

#endif

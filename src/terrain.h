#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <stdlib.h>     /* srand, rand */
#include <iostream>

using namespace std;

int chunkSize = 30;

class Terrain {
public:
    std::vector<glm::vec4> up_left_offsets;
    std::vector<glm::vec4> up_center_offsets;
    std::vector<glm::vec4> up_right_offsets;
    std::vector<glm::vec4> left_offsets;
    std::vector<glm::vec4> center_offsets;
    std::vector<glm::vec4> right_offsets;
    std::vector<glm::vec4> down_left_offsets;
    std::vector<glm::vec4> down_center_offsets;
    std::vector<glm::vec4> down_right_offsets;


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
    int bedrock = -4;
    int min = 0;
    int max = chunkSize;
    int index = 0;
    for(int x = min; x < max; ++x){
        for(int z = min; z < max; ++z ){
                float height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx, z + startz)) * w ));
                int clampHeight = ceil(height);
                center_offsets.push_back(glm::vec4(x + startx, clampHeight + bedrock, z +  startz, 0));

                height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx-chunkSize, z + startz-chunkSize)) * w ));
                up_left_offsets.push_back(glm::vec4(x + startx - chunkSize, ceil(height) + bedrock, z +  startz - chunkSize, 0));
                height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx, z + startz-chunkSize)) * w ));
                up_center_offsets.push_back(glm::vec4(x + startx, ceil(height) + bedrock, z +  startz - chunkSize, 0));
                height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx+chunkSize, z + startz-chunkSize)) * w ));
                up_right_offsets.push_back(glm::vec4(x + startx + chunkSize, ceil(height) + bedrock, z +  startz - chunkSize, 0));
                
                height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx-chunkSize, z + startz)) * w ));
                left_offsets.push_back(glm::vec4(x + startx - chunkSize, ceil(height) + bedrock, z +  startz, 0));
                height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx+chunkSize, z + startz)) * w ));
                right_offsets.push_back(glm::vec4(x + startx + chunkSize, ceil(height) + bedrock, z +  startz, 0));

                height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx-chunkSize, z + startz+chunkSize)) * w ));
                down_left_offsets.push_back(glm::vec4(x + startx - chunkSize, ceil(height) + bedrock, z +  startz + chunkSize, 0));
                height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx, z + startz+chunkSize)) * w ));
                down_center_offsets.push_back(glm::vec4(x + startx, ceil(height) + bedrock, z +  startz + chunkSize, 0));
                height = amplitude + (amplitude * sin(glm::dot(direction, glm::vec2(x + startx+chunkSize, z + startz+chunkSize)) * w ));
                down_right_offsets.push_back(glm::vec4(x + startx + chunkSize, ceil(height) + bedrock, z +  startz + chunkSize, 0));
                
                // for(int i = 0; i < clampHeight; ++i){
                //     offsets.push_back(glm::vec4(x, i + bedrock, z, 0));
                // }                         
        }
    }
    

	// float height_2 = amplitude_2 * sin(dot(direction_2, vec2(offset[0], offset[2])) * w_2 + time * speed_2);

}
void grid(glm::vec4 world_pos){
    glm::vec2 pos = glm::vec2(world_pos.x,world_pos.z);
    int seed = floor(world_pos.x/chunkSize) * 653 + floor(world_pos.z/chunkSize); //just chose an arbitrary prime number
    //get edge vectors
    srand(seed);
    int big_grid [chunkSize][chunkSize];
    glm::vec2 topLeft = glm::normalize(glm::vec2(rand() % 10,rand() % 10));
    glm::vec2 topRight = glm::normalize(glm::vec2(rand() % 10,rand() % 10));
    glm::vec2 botLeft = glm::normalize(glm::vec2(rand() % 10,rand() % 10));
    glm::vec2 botRight = glm::normalize(glm::vec2(rand() % 10,rand() % 10));
    for(int col = 0; col < chunkSize; col++){
        for(int row = 0; row < chunkSize; row++){

        }
    }

}


private:


};

#endif

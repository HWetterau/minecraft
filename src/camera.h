#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp>
#include "glm/gtc/matrix_access.hpp"
#include <iostream>

class Camera {
public:
	glm::mat4 get_view_matrix() const;
	// FIXME: add functions to manipulate camera objects.
	void w_move_forward();
	glm::vec3 w_can_move();
	void s_move_backward();
	glm::vec3 s_can_move();
	void a_strafe_left();
	glm::vec3 a_can_move();
	void d_strafe_right();
	glm::vec3 d_can_move();
	void up_pan();
	void down_pan();
	void left_drag(double delta_x, double delta_y);
	glm::vec4 get_eye() const;
	void toggle_gravity() { gravity_on = !gravity_on; };
	void jump();
	bool is_jumping() { return jumping; };
	void update_height(int height);
	void change_eye(int height);
;


private:
	bool gravity_on = true;
	float camera_distance_ = 3.0;
	glm::vec3 look_ = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	// glm::vec3 look_ = glm::vec3(0.0f, -1.0/sqrt(2.0), -1.0/sqrt(2.0));
	// glm::vec3 up_ = glm::vec3(0.0f, 1.0/sqrt(2.0), -1.0/sqrt(2.0));
	glm::vec3 eye_ = glm::vec3(0.0f,1.75f, camera_distance_);
	// Note: you may need additional member variables
	glm::vec3 tangent_ = glm::normalize(glm::cross(look_, up_));
	glm::vec3 center_ = eye_ + (camera_distance_ * look_);

	float pitch = 0.0f;
	float yaw = 0.0f;
	float velocity = 0.0f;
	float gravity = -0.1f;
	bool jumping = false;
};

#endif

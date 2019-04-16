#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtx/transform.hpp>
#include "glm/gtc/matrix_access.hpp"

class Camera {
public:
	glm::mat4 get_view_matrix() const;
	// FIXME: add functions to manipulate camera objects.
	void w_move_forward();
	void s_move_backward();
	void a_strafe_left();
	void d_strafe_right();
	void up_pan();
	void down_pan();
	void left_drag(double delta_x, double delta_y);
	glm::vec4 get_eye() const;


private:
	float camera_distance_ = 3.0;
	glm::vec3 look_ = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up_ = glm::vec3(0.0f, 1.0f, 0.0f);
	// glm::vec3 look_ = glm::vec3(0.0f, -1.0/sqrt(2.0), -1.0/sqrt(2.0));
	// glm::vec3 up_ = glm::vec3(0.0f, 1.0/sqrt(2.0), -1.0/sqrt(2.0));
	glm::vec3 eye_ = glm::vec3(0.0f, 6.75f, camera_distance_);
	// Note: you may need additional member variables
	glm::vec3 tangent_ = glm::normalize(glm::cross(look_, up_));
	glm::vec3 center_ = eye_ + (camera_distance_ * look_);
};

#endif

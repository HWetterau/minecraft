#include "camera.h"

namespace {
	float pan_speed = 0.1f;
	float roll_speed = 0.1f;
	float rotation_speed = 0.05f;
	float zoom_speed = 0.1f;
};

// FIXME: Calculate the view matrix
glm::mat4 Camera::get_view_matrix() const
{
	glm::vec3 neg_look = glm::normalize(eye_ - center_);
	glm::vec3 tang = glm::normalize(glm::cross(up_,neg_look));
	glm::vec3 up = glm::cross(neg_look, tang);
	return glm::inverse(glm::mat4(glm::vec4(tang, 0.0), glm::vec4(up, 0.0), glm::vec4(neg_look, 0.0), glm::vec4(eye_, 1.0)));
	//return glm::mat4(1.0);
}

glm::vec4 Camera::get_eye() const
{
	return glm::vec4(eye_, 1);
	//return glm::mat4(1.0);
}

void Camera::w_move_forward() {
	eye_ = eye_ + (zoom_speed * look_);
	center_ = eye_ + (camera_distance_ * look_);

}

void Camera::s_move_backward() {
	eye_ = eye_ - (zoom_speed * look_);
	center_ = eye_ + (camera_distance_ * look_);
}

void Camera::a_strafe_left() {
	eye_ = eye_ - (pan_speed * tangent_);
	center_ = eye_ + (camera_distance_ * look_);
}

void Camera::d_strafe_right() {
	eye_ = eye_ + (pan_speed * tangent_);
	center_ = eye_ + (camera_distance_ * look_);
}

void Camera::up_pan() {
	eye_ = eye_ + (pan_speed * up_);
	center_ = eye_ + (camera_distance_ * look_);
}

void Camera::down_pan() {
	eye_ = eye_ - (pan_speed * up_);
	center_ = eye_ + (camera_distance_ * look_);
}

void Camera::left_drag(double delta_x, double delta_y) {
	// glm::vec3 drag = glm::vec3(-delta_x, delta_y ,0.0f);
	// drag = glm::normalize(drag);
	// glm::vec3 axis =glm::cross(drag,look_);
	// glm::mat4 view = glm::mat4(glm::vec4(tangent_, 0.0), glm::vec4(up_, 0.0), glm::vec4(look_, 0.0), glm::vec4(eye_, 1.0));
	// glm::mat4 rotation = glm::rotate(view, rotation_speed, axis);


	// look_ = glm::vec3(rotation[2]);
	// tangent_ = glm::vec3(rotation[0]);
	// up_ = glm::vec3(rotation[1]);

	// center_ = eye_ + (camera_distance_ * look_);
	glm::mat3 orientation_ = glm::mat3(tangent_, up_, look_);
	glm::vec3 axis = glm::normalize(orientation_ *glm::vec3(delta_y, delta_x, 0.0f));
	orientation_ = glm::mat3(glm::rotate(rotation_speed, axis) * glm::mat4(orientation_));
	tangent_ = glm::column(orientation_, 0);
	up_ = glm::column(orientation_, 1);
	look_ = glm::column(orientation_, 2);
	center_ = eye_ + (camera_distance_ * look_);

}

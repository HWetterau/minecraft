#include "camera.h"
using namespace std;
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
	glm::vec3 move_dir = look_;
	move_dir.y = 0.0;
	move_dir = glm::normalize(move_dir);

	eye_ = eye_ + (zoom_speed * move_dir);
	center_ = eye_ + (camera_distance_ * look_);

}

void Camera::s_move_backward() {
	glm::vec3 move_dir = look_;
	move_dir.y = 0.0;
	move_dir = glm::normalize(move_dir);

	eye_ = eye_ - (zoom_speed * move_dir);
	center_ = eye_ + (camera_distance_ * look_);
}

void Camera::a_strafe_left() {
	glm::vec3 move_dir = tangent_;
	move_dir.y = 0.0;
	move_dir = glm::normalize(move_dir);

	eye_ = eye_ - (pan_speed * move_dir);
	center_ = eye_ + (camera_distance_ * look_);
}

void Camera::d_strafe_right() {
	glm::vec3 move_dir = tangent_;
	move_dir.y = 0.0;
	move_dir = glm::normalize(move_dir);

	eye_ = eye_ + (pan_speed * move_dir);
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
	yaw   += delta_x * 0.5;
	pitch += delta_y * 0.5;  
	if(pitch > 89.0f)
		pitch =  89.0f;
	if(pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	look_ = glm::normalize(front);
	center_ = eye_ + (camera_distance_ * look_);
	tangent_ = glm::normalize(glm::cross(look_, up_));

	// cout << "look " << look_.y << endl;
	// cout << "delta y " << delta_y << endl;
	// if ((delta_y < 0 && look_.y <= -.75) || (delta_y > 0 && look_.y >= .75)) {
	// 	delta_y = 0;
	// 	cout<<"in if "<<endl;
	// 	if(delta_x == 0)
	// 		return;
	// }
	
	// glm::mat3 orientation_ = glm::mat3(tangent_, up_, look_);
	// glm::vec3 axis = glm::normalize(orientation_ *glm::vec3(delta_y, -delta_x, 0.0f));
	// orientation_ = glm::mat3(glm::rotate(rotation_speed, axis) * glm::mat4(orientation_));
	// tangent_ = glm::column(orientation_, 0);
	// up_ = glm::column(orientation_, 1);
	// look_ = glm::column(orientation_, 2);
	// center_ = eye_ + (camera_distance_ * look_);

}



void Camera::jump() {
	if (!jumping) {
		velocity = 0.5f;
		jumping = true;

	}
}

void Camera::update_height() {
	if (velocity < -0.4) {
		jumping = false;
	}
	eye_.y += velocity;
	velocity += gravity;
	center_ = eye_ + (camera_distance_ * look_);
	
}

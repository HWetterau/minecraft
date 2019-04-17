#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

// OpenGL library includes
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <debuggl.h>

#include "camera.h"
#include "terrain.h"

#include <chrono>
#include <algorithm>
#include <sstream>



using namespace std;

int window_width = 800, window_height = 600;

// VBO and VAO descriptors.
enum { kVertexBuffer, kIndexBuffer, kNumVbos };

// These are our VAOs.
enum { kGeometryVao, kNumVaos };

GLuint g_array_objects[kNumVaos];  // This will store the VAO descriptors.
GLuint g_buffer_objects[kNumVaos][kNumVbos];  // These will store VBO descriptors.

// C++ 11 String Literal
// See http://en.cppreference.com/w/cpp/language/string_literal

//Shaders Here

const char* vertex_shader =
#include "shaders/default.vert"
;

const char* geometry_shader =
#include "shaders/default.geom"
;


const char* default_frag_shader =
#include "shaders/default.frag"
;

const char* dirt_frag_shader =
#include "shaders/dirt.frag"
;

const char* grass_frag_shader =
#include "shaders/grass.frag"
;




void CreateCube (std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces) {
	
	double minX = 0;
	double minY = minX;
	double minZ = minX;
	double maxX = 1;
	double maxY = maxX;
	double maxZ = maxX;

		//build the cube
	obj_vertices.push_back(glm::vec4(minX,minY,minZ,1.0f));
	obj_vertices.push_back(glm::vec4(minX,maxY, minZ,1.0f));
	obj_vertices.push_back(glm::vec4(maxX,maxY,minZ,1.0f));
	obj_vertices.push_back(glm::vec4(maxX,minY,minZ,1.0f));
	obj_vertices.push_back(glm::vec4(minX,minY, maxZ,1.0f));
	obj_vertices.push_back(glm::vec4(minX,maxY,maxZ,1.0f));
	obj_vertices.push_back(glm::vec4(maxX,maxY,maxZ,1.0f));
	obj_vertices.push_back(glm::vec4(maxX,minY,maxZ,1.0f));

	obj_faces.push_back(glm::uvec3(0,1,2));
	obj_faces.push_back(glm::uvec3(0,2,3));
	obj_faces.push_back(glm::uvec3(6,5,4));
	obj_faces.push_back(glm::uvec3(7,6,4));
	obj_faces.push_back(glm::uvec3(3,2,6));
	obj_faces.push_back(glm::uvec3(3,6,7));
	obj_faces.push_back(glm::uvec3(4,5,1));
	obj_faces.push_back(glm::uvec3(4,1,0));
	obj_faces.push_back(glm::uvec3(1,5,2));
	obj_faces.push_back(glm::uvec3(5,6,2));
	obj_faces.push_back(glm::uvec3(4,0,3));
	obj_faces.push_back(glm::uvec3(4,3,7));

}




void
ErrorCallback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << "\n";
}


Camera g_camera;
bool g_save_geo = false;
bool wireframe = true;
bool polygon_mode = true;
bool change_chunk = false;


bool border(glm::vec4 oldPos, glm::vec4 newPos) {
	return !( floor(oldPos.x/chunkSize) == floor(newPos.x/chunkSize) &&
			  floor(oldPos.z/chunkSize) == floor(newPos.z/chunkSize));
}

void
KeyCallback(GLFWwindow* window,
            int key,
            int scancode,
            int action,
            int mods)
{
	// Note:
	// This is only a list of functions to implement.
	// you may want to re-organize this piece of code.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else if (key == GLFW_KEY_S && mods == GLFW_MOD_CONTROL && action == GLFW_RELEASE) {
		g_save_geo = true;

	} else if (key == GLFW_KEY_W && action != GLFW_RELEASE) {
		// FIXME: 
		glm::vec4 oldPos = g_camera.get_eye();
		g_camera.w_move_forward();
		change_chunk = border(oldPos,g_camera.get_eye());

	} else if (key == GLFW_KEY_S && mods != GLFW_MOD_CONTROL && action != GLFW_RELEASE) {
		glm::vec4 oldPos = g_camera.get_eye();
		g_camera.s_move_backward();
		change_chunk = border(oldPos,g_camera.get_eye());

	} else if (key == GLFW_KEY_A && action != GLFW_RELEASE) {
		glm::vec4 oldPos = g_camera.get_eye();
		g_camera.a_strafe_left();
		change_chunk = border(oldPos,g_camera.get_eye());

	} else if (key == GLFW_KEY_D && action != GLFW_RELEASE) {
		glm::vec4 oldPos = g_camera.get_eye();
		g_camera.d_strafe_right();
		change_chunk = border(oldPos,g_camera.get_eye());

	} else if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE) {
		// FIXME: Left Right Up and Down

	} else if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE) {

	} else if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE) {
		glm::vec4 oldPos = g_camera.get_eye();
		g_camera.down_pan();
		change_chunk = border(oldPos,g_camera.get_eye());

	} else if (key == GLFW_KEY_UP && action != GLFW_RELEASE) {
		glm::vec4 oldPos = g_camera.get_eye();
		g_camera.up_pan();
		change_chunk = border(oldPos,g_camera.get_eye());
	} else if (key == GLFW_KEY_C && action != GLFW_RELEASE) {
		// FIXME: FPS mode on/off

	} else if (key == GLFW_KEY_F && mods == GLFW_MOD_CONTROL && action == GLFW_RELEASE) {
		// toggle gravity
		g_camera.toggle_gravity();

	} 

}

int g_current_button;
bool g_mouse_pressed;
//ADDED
float last_x_ = 0.0f, last_y_ = 0.0f, current_x_ = 0.0f, current_y_ = 0.0f;

void
MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y)
{	
	last_x_ = current_x_;
	last_y_ = current_y_;
	current_x_ = mouse_x;
	current_y_ = window_height - mouse_y;
	float delta_x = current_x_ - last_x_;
	float delta_y = current_y_ - last_y_;


	// FIXME: left drag

	g_camera.left_drag(delta_x, delta_y);

	if (g_current_button == GLFW_MOUSE_BUTTON_RIGHT) {
		// FIXME: middle drag
	} else if (g_current_button == GLFW_MOUSE_BUTTON_MIDDLE) {
		// FIXME: right drag
	}
}

void
MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	g_mouse_pressed = (action == GLFW_PRESS);
	g_current_button = button;
}

int main(int argc, char* argv[])
{
	std::string window_title = "Minecraft";
	if (!glfwInit()) exit(EXIT_FAILURE);

	glfwSetErrorCallback(ErrorCallback);

	// Ask an OpenGL 4.1 core profile context
	// It is required on OSX and non-NVIDIA Linux
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(window_width, window_height,
			&window_title[0], nullptr, nullptr);
	CHECK_SUCCESS(window != nullptr);
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	CHECK_SUCCESS(glewInit() == GLEW_OK);
	glGetError();  // clear GLEW's error for it
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MousePosCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
	glfwSwapInterval(1);
	const GLubyte* renderer = glGetString(GL_RENDERER);  // get renderer string
	const GLubyte* version = glGetString(GL_VERSION);    // version as a string
	std::cout << "Renderer: " << renderer << "\n";
	std::cout << "OpenGL version supported:" << version << "\n";



	//cubes
	std::vector<glm::vec4> obj_vertices;
	std::vector<glm::uvec3> obj_faces;  

	CreateCube(obj_vertices, obj_faces);

	Terrain terrain;
	// std::vector<glm::vec4> up_left_offsets;
	// std::vector<glm::vec4> up_center_offsets;
	// std::vector<glm::vec4> up_right_offsets;
	// std::vector<glm::vec4> left_offsets;
	// std::vector<glm::vec4> center_offsets;
	// std::vector<glm::vec4> right_offsets;
	// std::vector<glm::vec4> down_left_offsets;
	// std::vector<glm::vec4> down_center_offsets;
	// std::vector<glm::vec4> down_right_offsets;
	// glm::vec4 eye = g_camera.get_eye();

	terrain.generate(g_camera.get_eye());

	//glm::vec4 chunks[9][chunkSize*chunkSize];



	// glm::vec4 min_bounds = glm::vec4(std::numeric_limits<float>::max());
	// glm::vec4 max_bounds = glm::vec4(-std::numeric_limits<float>::max());
	// for (const auto& vert : obj_vertices) {
	// 	min_bounds = glm::min(vert, min_bounds);
	// 	max_bounds = glm::max(vert, max_bounds);
	// }
	// std::cout << "min_bounds = " << glm::to_string(min_bounds) << "\n";
	// std::cout << "max_bounds = " << glm::to_string(max_bounds) << "\n";


	// Setup our VAO array.
	CHECK_GL_ERROR(glGenVertexArrays(kNumVaos, &g_array_objects[0]));

	// Switch to the VAO for Geometry.
	CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kGeometryVao]));

	// Generate buffer objects
	CHECK_GL_ERROR(glGenBuffers(kNumVbos, &g_buffer_objects[kGeometryVao][0]));

	// Setup vertex data in a VBO.
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kVertexBuffer]));
	// NOTE: We do not send anything right now, we just describe it to OpenGL.
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
				sizeof(float) * obj_vertices.size() * 4, obj_vertices.data(),
				GL_STATIC_DRAW));
	CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
	CHECK_GL_ERROR(glEnableVertexAttribArray(0));

	// Setup element array buffer.
	CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kIndexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				sizeof(uint32_t) * obj_faces.size() * 3,
				obj_faces.data(), GL_STATIC_DRAW));

	/*
 	 * By far, the geometry is loaded into g_buffer_objects[kGeometryVao][*].
	 * These buffers are binded to g_array_objects[kGeometryVao]
	 */


	// Setup vertex shader.
	GLuint vertex_shader_id = 0;
	CHECK_GL_ERROR(vertex_shader_id = glCreateShader(GL_VERTEX_SHADER));
	CHECK_GL_ERROR(glShaderSource(vertex_shader_id, 1, &vertex_shader, nullptr));
	glCompileShader(vertex_shader_id);
	CHECK_GL_SHADER_ERROR(vertex_shader_id);

	GLuint geometry_shader_id = 0;
	CHECK_GL_ERROR(geometry_shader_id = glCreateShader(GL_GEOMETRY_SHADER));
	CHECK_GL_ERROR(glShaderSource(geometry_shader_id, 1, &geometry_shader, nullptr));
	glCompileShader(geometry_shader_id);
	CHECK_GL_SHADER_ERROR(geometry_shader_id);

	// Setup fragment shader.
	GLuint default_fragment_shader_id = 0;
	CHECK_GL_ERROR(default_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
	CHECK_GL_ERROR(glShaderSource(default_fragment_shader_id, 1, &default_frag_shader, nullptr));
	glCompileShader(default_fragment_shader_id);
	CHECK_GL_SHADER_ERROR(default_fragment_shader_id);

	// Let's create our program.
	GLuint default_program_id = 0;
	CHECK_GL_ERROR(default_program_id = glCreateProgram());
	CHECK_GL_ERROR(glAttachShader(default_program_id, vertex_shader_id));
	CHECK_GL_ERROR(glAttachShader(default_program_id, geometry_shader_id));
    CHECK_GL_ERROR(glAttachShader(default_program_id, default_fragment_shader_id));

	// Bind attributes.
	CHECK_GL_ERROR(glBindAttribLocation(default_program_id, 0, "vertex_position"));
	CHECK_GL_ERROR(glBindFragDataLocation(default_program_id, 0, "fragment_color"));

	glLinkProgram(default_program_id);
	CHECK_GL_PROGRAM_ERROR(default_program_id);

	// Get the uniform locations.
	GLint projection_matrix_location = 0;
	CHECK_GL_ERROR(projection_matrix_location =
			glGetUniformLocation(default_program_id, "projection"));
	GLint view_matrix_location = 0;
	CHECK_GL_ERROR(view_matrix_location =
			glGetUniformLocation(default_program_id, "view"));
	GLint light_position_location = 0;
	GLint camera_position_location = 0;

	CHECK_GL_ERROR(light_position_location =
			glGetUniformLocation(default_program_id, "light_position"));

	// CHECK_GL_ERROR(camera_position_location =
	// 		glGetUniformLocation(default_program_id, "camera_position"));


int num_trans = 900;
GLuint cube_translation_location[num_trans];
for (int i = 0; i < num_trans; ++i){
	string s =  "offsets["+to_string(i)+"]";
	const char* c = s.c_str();
	CHECK_GL_ERROR(cube_translation_location[i] =
		glGetUniformLocation(default_program_id, c));
}

glm::vec4 ultranslations[num_trans];
glm::vec4 uctranslations[num_trans];
glm::vec4 urtranslations[num_trans];
glm::vec4 ltranslations[num_trans];
glm::vec4 ctranslations[num_trans];
glm::vec4 rtranslations[num_trans];
glm::vec4 dltranslations[num_trans];
glm::vec4 dctranslations[num_trans];
glm::vec4 drtranslations[num_trans];
std::copy(terrain.up_left_offsets.begin(), terrain.up_left_offsets.end(), ultranslations);
std::copy(terrain.up_center_offsets.begin(), terrain.up_center_offsets.end(), uctranslations);
std::copy(terrain.up_right_offsets.begin(), terrain.up_right_offsets.end(), urtranslations);
std::copy(terrain.left_offsets.begin(), terrain.left_offsets.end(), ltranslations);
std::copy(terrain.center_offsets.begin(), terrain.center_offsets.end(), ctranslations);
std::copy(terrain.right_offsets.begin(), terrain.right_offsets.end(), rtranslations);
std::copy(terrain.down_left_offsets.begin(), terrain.down_left_offsets.end(), dltranslations);
std::copy(terrain.down_center_offsets.begin(), terrain.down_center_offsets.end(), dctranslations);
std::copy(terrain.down_right_offsets.begin(), terrain.down_right_offsets.end(), drtranslations);
//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
 //	auto start = chrono::steady_clock::now();
	glm::vec4 light_position = glm::vec4(-10.0f, 20.0f, 10.0f, 1.0f);
	float aspect = 0.0f;
	float theta = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		// Setup some basic window stuff.
		glfwGetFramebufferSize(window, &window_width, &window_height);
		glViewport(0, 0, window_width, window_height);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDepthFunc(GL_LESS);

		// Switch to the Geometry VAO.
		CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kGeometryVao]));

		// FIXME: Upload your vertex data here.
		// Setup vertex data in a VBO.
		CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kVertexBuffer]));

		CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
			sizeof(float) * obj_vertices.size() * 4, obj_vertices.data(),
			GL_STATIC_DRAW));
		CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
		CHECK_GL_ERROR(glEnableVertexAttribArray(0));

		// Setup element array buffer.
		CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kIndexBuffer]));
		CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
					sizeof(uint32_t) * obj_faces.size() * 3,
					obj_faces.data(), GL_STATIC_DRAW));

		if (change_chunk){
			cout<<"change chunk"<<endl;
			terrain.generate(g_camera.get_eye());
			std::copy(terrain.up_left_offsets.begin(), terrain.up_left_offsets.end(), ultranslations);
			std::copy(terrain.up_center_offsets.begin(), terrain.up_center_offsets.end(), uctranslations);
			std::copy(terrain.up_right_offsets.begin(), terrain.up_right_offsets.end(), urtranslations);
			std::copy(terrain.left_offsets.begin(), terrain.left_offsets.end(), ltranslations);
			std::copy(terrain.center_offsets.begin(), terrain.center_offsets.end(), ctranslations);
			std::copy(terrain.right_offsets.begin(), terrain.right_offsets.end(), rtranslations);
			std::copy(terrain.down_left_offsets.begin(), terrain.down_left_offsets.end(), dltranslations);
			std::copy(terrain.down_center_offsets.begin(), terrain.down_center_offsets.end(), dctranslations);
			std::copy(terrain.down_right_offsets.begin(), terrain.down_right_offsets.end(), drtranslations);
			 change_chunk = false;
		}

		// Compute the projection matrix.
		aspect = static_cast<float>(window_width) / window_height;
		glm::mat4 projection_matrix =
			glm::perspective(glm::radians(45.0f), aspect, 0.2f, 100.0f);

		// Compute the view matrix
		// FIXME: change eye and center through mouse/keyboard events.
		glm::mat4 view_matrix = g_camera.get_view_matrix();

		// Use our program.
		CHECK_GL_ERROR(glUseProgram(default_program_id));
		
		// Pass uniforms in.
		CHECK_GL_ERROR(glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE,
					&projection_matrix[0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE,
					&view_matrix[0][0]));
		CHECK_GL_ERROR(glUniform4fv(light_position_location, 1, &light_position[0]));

		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &ultranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.up_left_offsets.size()));
		 

		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &uctranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.up_center_offsets.size()));

		
		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &urtranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.up_right_offsets.size()));

	
		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &ltranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.left_offsets.size()));

		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &ctranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.center_offsets.size()));

		
		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &rtranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.right_offsets.size()));

		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &dltranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.down_left_offsets.size()));
		 

		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &dctranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.down_center_offsets.size()));

		
		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &drtranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.down_right_offsets.size()));



		// Draw our triangles.
		//CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0));

	

		// Poll and swap.
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

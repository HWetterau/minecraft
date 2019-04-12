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

const char* stone_frag_shader =
#include "shaders/stone.frag"
;

const char* dirt_frag_shader =
#include "shaders/dirt.frag"
;

const char* grass_frag_shader =
#include "shaders/grass.frag"
;




void CreateCube (std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces) {
	
	double minX = -0.5;
	double minY = -0.5;
	double minZ = -0.5;
	double maxX = 0.5;
	double maxY = 0.5;
	double maxZ = 0.5;

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
		// FIXME: WASD
		g_camera.w_move_forward();

	} else if (key == GLFW_KEY_S && mods != GLFW_MOD_CONTROL && action != GLFW_RELEASE) {
		g_camera.s_move_backward();

	} else if (key == GLFW_KEY_A && action != GLFW_RELEASE) {
		g_camera.a_strafe_left();

	} else if (key == GLFW_KEY_D && action != GLFW_RELEASE) {
		g_camera.d_strafe_right();

	} else if (key == GLFW_KEY_LEFT && action != GLFW_RELEASE) {
		// FIXME: Left Right Up and Down

	} else if (key == GLFW_KEY_RIGHT && action != GLFW_RELEASE) {

	} else if (key == GLFW_KEY_DOWN && action != GLFW_RELEASE) {
		g_camera.down_pan();

	} else if (key == GLFW_KEY_UP && action != GLFW_RELEASE) {
		g_camera.up_pan();

	} else if (key == GLFW_KEY_C && action != GLFW_RELEASE) {
		// FIXME: FPS mode on/off

	} else if (key == GLFW_KEY_F && mods == GLFW_MOD_CONTROL && action == GLFW_RELEASE) {
		// toggle if edges or faces

		if (polygon_mode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			polygon_mode = false;

		} else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			polygon_mode = true;
		}
	} 

}

int g_current_button;
bool g_mouse_pressed;
//ADDED
bool first;
double first_mouse_x, first_mouse_y;

void
MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y)
{
	if (!g_mouse_pressed){
		first = true;
		return;
	}
	if (g_current_button == GLFW_MOUSE_BUTTON_LEFT) {
		// FIXME: left drag
		if(first){
			first_mouse_x = mouse_x;
			first_mouse_y = mouse_y;
			first = false;
		}
		else {
			double delta_x = mouse_x - first_mouse_x;
			double delta_y = mouse_y - first_mouse_y;
			first_mouse_x = mouse_x;
			first_mouse_y = mouse_y;
			g_camera.left_drag(delta_x, delta_y);
		}
	} else if (g_current_button == GLFW_MOUSE_BUTTON_RIGHT) {
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

	
	// Setup fragment shader.
	GLuint stone_fragment_shader_id = 0;
	CHECK_GL_ERROR(stone_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
	CHECK_GL_ERROR(glShaderSource(stone_fragment_shader_id, 1, &stone_frag_shader, nullptr));
	glCompileShader(stone_fragment_shader_id);
	CHECK_GL_SHADER_ERROR(stone_fragment_shader_id);

	// Let's create our program.
	GLuint stone_program_id = 0;
	CHECK_GL_ERROR(stone_program_id = glCreateProgram());
	CHECK_GL_ERROR(glAttachShader(stone_program_id, vertex_shader_id));
	CHECK_GL_ERROR(glAttachShader(stone_program_id, stone_fragment_shader_id));

	// Bind attributes.
	CHECK_GL_ERROR(glBindAttribLocation(stone_program_id, 0, "vertex_position"));
	CHECK_GL_ERROR(glBindFragDataLocation(stone_program_id, 0, "fragment_color"));

	glLinkProgram(stone_program_id);
	CHECK_GL_PROGRAM_ERROR(stone_program_id);

	// Get the uniform locations.
	// GLint projection_matrix_location = 0;
	// CHECK_GL_ERROR(projection_matrix_location =
	// 		glGetUniformLocation(stone_program_id, "projection"));
	// GLint view_matrix_location = 0;
	// CHECK_GL_ERROR(view_matrix_location =
	// 		glGetUniformLocation(stone_program_id, "view"));
	GLint light_position_location = 0;
	GLint camera_position_location = 0;

	CHECK_GL_ERROR(light_position_location =
			glGetUniformLocation(stone_program_id, "light_position"));

	CHECK_GL_ERROR(camera_position_location =
			glGetUniformLocation(stone_program_id, "camera_position"));


//int num_trans = 40;
// for (int i = 0; i < num_trans; ++i){
// 	string s s["+to_string(i)+"]";
// 	const char* c = s.c_str();
// 	CHECK_GL_ERROR(boat_translation_location[i] =
// 		glGetUniformLocation(boat_program_id, c));
// }

// glm::vec4 translations[num_trans];
// int index = 0;
// for(int x = -15; x < 15; x += 3)
// {
//     for(int z = -20; z < 0; z += 5)
//     {
//         glm::vec4 translation;
//         translation.x = (float)x;
//         translation.z = (float)z;
// 				translation.y = -1.5;
// 				translation.w = 0;
//         translations[index++] = translation;
//     }
// }


 //	auto start = chrono::steady_clock::now();
	glm::vec4 light_position = glm::vec4(-10.0f, 10.0f, 0.0f, 1.0f);
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
		//CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kGeometryVao]));

		// // FIXME: Upload your vertex data here.
		// // Setup vertex data in a VBO.
		// CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kVertexBuffer]));

		// CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
		// 	sizeof(float) * obj_vertices.size() * 4, obj_vertices.data(),
		// 	GL_STATIC_DRAW));
		// CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
		// CHECK_GL_ERROR(glEnableVertexAttribArray(0));

		// // Setup element array buffer.
		// CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kGeometryVao][kIndexBuffer]));
		// CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		// 			sizeof(uint32_t) * obj_faces.size() * 3,
		// 			obj_faces.data(), GL_STATIC_DRAW));


		// // Compute the projection matrix.
		// aspect = static_cast<float>(window_width) / window_height;
		// glm::mat4 projection_matrix =
		// 	glm::perspective(glm::radians(45.0f), aspect, 0.0001f, 1000.0f);

		// // Compute the view matrix
		// // FIXME: change eye and center through mouse/keyboard events.
		// glm::mat4 view_matrix = g_camera.get_view_matrix();

		// // Use our program.
		// CHECK_GL_ERROR(glUseProgram(program_id));

		// // Pass uniforms in.
		// CHECK_GL_ERROR(glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE,
		// 			&projection_matrix[0][0]));
		// CHECK_GL_ERROR(glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE,
		// 			&view_matrix[0][0]));
		// CHECK_GL_ERROR(glUniform4fv(light_position_location, 1, &light_position[0]));

		// // Draw our triangles.
		// CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0));

	

		// Poll and swap.
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

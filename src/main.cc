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
enum { kGeometryVao, kFloorVao, kSkyVao, kMoonVao, kNumVaos};

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

const char* floor_vert_shader =
#include "shaders/floor.vert"
;

const char* floor_frag_shader =
#include "shaders/floor.frag"
;
const char* sky_vertex_shader =
#include "shaders/sky.vert"
;

const char* sky_frag_shader =
#include "shaders/sky.frag"
;

const char* moon_vertex_shader =
#include "shaders/moon.vert"
;

const char* moon_frag_shader =
#include "shaders/moon.frag"
;






void CreateCube (std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces) {

	double minX = 0;
	double minY = minX;
	double minZ = minX;
	double maxX = 1;
	double maxY = maxX;
	double maxZ = maxX;

		//build the cube
	obj_vertices.push_back(glm::vec4(minX,minY, maxZ,1.0f));
	obj_vertices.push_back(glm::vec4(maxX,minY,maxZ,1.0f));
	obj_vertices.push_back(glm::vec4(minX,maxY,maxZ,1.0f));
	obj_vertices.push_back(glm::vec4(maxX,maxY,maxZ,1.0f));

	obj_vertices.push_back(glm::vec4(minX,minY,minZ,1.0f));
	obj_vertices.push_back(glm::vec4(maxX,minY,minZ,1.0f));
	obj_vertices.push_back(glm::vec4(minX,maxY, minZ,1.0f));
	obj_vertices.push_back(glm::vec4(maxX,maxY,minZ,1.0f));

	obj_faces.push_back(glm::uvec3(0,1,2));
	obj_faces.push_back(glm::uvec3(1,3,2));
	obj_faces.push_back(glm::uvec3(2,3,7));
	obj_faces.push_back(glm::uvec3(2,7,6));
	obj_faces.push_back(glm::uvec3(1,7,3));
	obj_faces.push_back(glm::uvec3(1,5,7));
	obj_faces.push_back(glm::uvec3(6,7,4));
	obj_faces.push_back(glm::uvec3(7,5,4));
	obj_faces.push_back(glm::uvec3(2,6,4));
	obj_faces.push_back(glm::uvec3(0,2,4));
	obj_faces.push_back(glm::uvec3(0,4,1));
	obj_faces.push_back(glm::uvec3(1,4,5));

}

void CreateMoon (std::vector<glm::vec4>& obj_vertices, std::vector<glm::uvec3>& obj_faces) {
	
	double minX = 7;
	double minY = minX;
	double minZ = minX;
	double maxX = 7.5;
	double maxY = maxX;
	double maxZ = maxX;

		//build the cube
	obj_vertices.push_back(glm::vec4(minX,minY, maxZ,1.0f));
	obj_vertices.push_back(glm::vec4(maxX,minY,maxZ,1.0f));
	obj_vertices.push_back(glm::vec4(minX,maxY,maxZ,1.0f));
	obj_vertices.push_back(glm::vec4(maxX,maxY,maxZ,1.0f));

	obj_vertices.push_back(glm::vec4(minX,minY,minZ,1.0f));
	obj_vertices.push_back(glm::vec4(maxX,minY,minZ,1.0f));
	obj_vertices.push_back(glm::vec4(minX,maxY, minZ,1.0f));
	obj_vertices.push_back(glm::vec4(maxX,maxY,minZ,1.0f));
	
	obj_faces.push_back(glm::uvec3(0,1,2));
	obj_faces.push_back(glm::uvec3(1,3,2));
	obj_faces.push_back(glm::uvec3(2,3,7));
	obj_faces.push_back(glm::uvec3(2,7,6));
	obj_faces.push_back(glm::uvec3(1,7,3));
	obj_faces.push_back(glm::uvec3(1,5,7));
	obj_faces.push_back(glm::uvec3(6,7,4));
	obj_faces.push_back(glm::uvec3(7,5,4));
	obj_faces.push_back(glm::uvec3(2,6,4));
	obj_faces.push_back(glm::uvec3(0,2,4));
	obj_faces.push_back(glm::uvec3(0,4,1));
	obj_faces.push_back(glm::uvec3(1,4,5));

}

void
CreatePlane(std::vector<glm::vec4>& vertices,
				std::vector<glm::uvec3>& indices)
{
	vertices.push_back(glm::vec4(-20.0,0.001,-20.0,1.0));
	vertices.push_back(glm::vec4(-20.0,0.001,40.0,1.0));
	vertices.push_back(glm::vec4(40.0,0.001,40.0,1.0));
	vertices.push_back(glm::vec4(40.0,0.001,-20.0,1.0));
	indices.push_back(glm::uvec3(0,2,3));
	indices.push_back(glm::uvec3(0,1,2));

}





void
ErrorCallback(int error, const char* description)
{
	std::cerr << "GLFW Error: " << description << "\n";
}

Terrain terrain;
Camera g_camera;
bool g_save_geo = false;
bool wireframe = true;
bool polygon_mode = true;
bool change_chunk = false;
bool w_down = false;
bool a_down = false;
bool s_down = false;
bool d_down = false;
bool day_night = false;

float sky_time = 0.01;
bool sunrise = true;
bool sunset = false;
int times = 0;


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
		// glm::vec4 oldPos = g_camera.get_eye();
		// if(!terrain.collision(glm::vec3(oldPos),g_camera.w_can_move())) {
		// 	g_camera.w_move_forward();
		// 	if(!change_chunk)
		// 		change_chunk = border(oldPos,g_camera.get_eye());
		// }


	} else if (key == GLFW_KEY_S && mods != GLFW_MOD_CONTROL && action != GLFW_RELEASE) {
		// glm::vec4 oldPos = g_camera.get_eye();
		// if(!terrain.collision(glm::vec3(oldPos),g_camera.s_can_move())) {
		// 	g_camera.s_move_backward();
		// 	if(!change_chunk)
		// 		change_chunk = border(oldPos,g_camera.get_eye());
		// }

	} else if (key == GLFW_KEY_A && action != GLFW_RELEASE) {
		// glm::vec4 oldPos = g_camera.get_eye();
		// if(!terrain.collision(glm::vec3(oldPos),g_camera.a_can_move())) {
		// 	g_camera.a_strafe_left();
		// 	if(!change_chunk)
		// 		change_chunk = border(oldPos,g_camera.get_eye());
		// }

	} else if (key == GLFW_KEY_D && action != GLFW_RELEASE) {
		// glm::vec4 oldPos = g_camera.get_eye();
		// if(!terrain.collision(glm::vec3(oldPos),g_camera.d_can_move())) {
		// 	g_camera.d_strafe_right();
		// 	if(!change_chunk)
		// 		change_chunk = border(oldPos,g_camera.get_eye());
		// }

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

	} else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) {

		g_camera.jump();

		// 	glm::vec4 oldPos = g_camera.get_eye();
		// 	g_camera.w_move_forward();
		// 	if(!change_chunk)
		// 		change_chunk = border(oldPos,g_camera.get_eye());
		// }
	} else if (key == GLFW_KEY_N && action == GLFW_RELEASE) {
		day_night = true;
		sky_time = 0.01;
		sunrise = true;
		sunset = false;
		times = 0;
	}

}

int g_current_button;
bool g_mouse_pressed;
//ADDED
bool firstMouse = true;
float last_x_ = 0.0f, last_y_ = 0.0f, current_x_ = 0.0f, current_y_ = 0.0f;

void
MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y)
{
	//supposedly eliminating the jump...
	if(firstMouse)
    {
        last_x_ = mouse_x;
        last_y_ = mouse_y;
        firstMouse = false;
    }

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

	std::vector<glm::vec4> plane_vertices;
	std::vector<glm::uvec3> plane_faces;

	CreatePlane(plane_vertices, plane_faces);

	std::vector<glm::vec4> moon_vertices;
	std::vector<glm::uvec3> moon_faces;  

	CreateMoon(moon_vertices, moon_faces);

	CHECK_GL_ERROR(glGenVertexArrays(kNumVaos, &g_array_objects[0]));

	// Switch to the VAO for Geometry.
	CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kMoonVao]));

	// Generate buffer objects
	CHECK_GL_ERROR(glGenBuffers(kNumVbos, &g_buffer_objects[kMoonVao][0]));

	// Setup vertex data in a VBO.
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kMoonVao][kVertexBuffer]));
	// NOTE: We do not send anything right now, we just describe it to OpenGL.
	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
				sizeof(float) * moon_vertices.size() * 4, moon_vertices.data(),
				GL_STATIC_DRAW));
	CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
	CHECK_GL_ERROR(glEnableVertexAttribArray(0));

	// Setup element array buffer.
	CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kMoonVao][kIndexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				sizeof(uint32_t) * moon_faces.size() * 3,
				moon_faces.data(), GL_STATIC_DRAW));


	GLuint moon_vertex_shader_id = 0;
	CHECK_GL_ERROR(moon_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER));
	CHECK_GL_ERROR(glShaderSource(moon_vertex_shader_id, 1, &moon_vertex_shader, nullptr));
	glCompileShader(moon_vertex_shader_id);
	CHECK_GL_SHADER_ERROR(moon_vertex_shader_id);

	GLuint moon_fragment_shader_id = 0;
	CHECK_GL_ERROR(moon_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
	CHECK_GL_ERROR(glShaderSource(moon_fragment_shader_id, 1, &moon_frag_shader, nullptr));
	glCompileShader(moon_fragment_shader_id);
	CHECK_GL_SHADER_ERROR(moon_fragment_shader_id);

	GLuint moon_program_id = 0;
	CHECK_GL_ERROR(moon_program_id = glCreateProgram());
	CHECK_GL_ERROR(glAttachShader(moon_program_id, moon_vertex_shader_id));
    CHECK_GL_ERROR(glAttachShader(moon_program_id, moon_fragment_shader_id));

	// Bind attributes.
	CHECK_GL_ERROR(glBindAttribLocation(moon_program_id, 0, "vertex_position"));
	CHECK_GL_ERROR(glBindFragDataLocation(moon_program_id, 0, "fragment_color"));

	glLinkProgram(moon_program_id);
	CHECK_GL_PROGRAM_ERROR(moon_program_id);

	GLint moon_projection_matrix_location = 0;
	CHECK_GL_ERROR(moon_projection_matrix_location =
			glGetUniformLocation(moon_program_id, "projection"));
	GLint moon_view_matrix_location = 0;
	CHECK_GL_ERROR(moon_view_matrix_location =
			glGetUniformLocation(moon_program_id, "view"));
	GLint moon_time_location = 0;
	CHECK_GL_ERROR(moon_time_location =
			glGetUniformLocation(moon_program_id, "time"));

	terrain.generate(g_camera.get_eye());
	int height = terrain.getMaxHeight(glm::vec3(g_camera.get_eye()));
	g_camera.change_eye(height);

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
	GLint permutation_location = 0;

	CHECK_GL_ERROR(light_position_location =
			glGetUniformLocation(default_program_id, "light_position"));
	CHECK_GL_ERROR(permutation_location =
			glGetUniformLocation(default_program_id, "p"));




//FLOOR
	CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kFloorVao]));
	CHECK_GL_ERROR(glGenBuffers(kNumVbos, &g_buffer_objects[kFloorVao][0]));
	CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kFloorVao][kVertexBuffer]));

	CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
				sizeof(float) * plane_vertices.size() * 4, plane_vertices.data(),
				GL_STATIC_DRAW));
	CHECK_GL_ERROR(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));
	CHECK_GL_ERROR(glEnableVertexAttribArray(0));

	// Setup element array buffer.
	CHECK_GL_ERROR(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_buffer_objects[kFloorVao][kIndexBuffer]));
	CHECK_GL_ERROR(glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				sizeof(uint32_t) * plane_faces.size() * 3,
				plane_faces.data(), GL_STATIC_DRAW));


	// Setup vertex shader.
	GLuint floor_vertex_shader_id = 0;
	CHECK_GL_ERROR(floor_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER));
	CHECK_GL_ERROR(glShaderSource(floor_vertex_shader_id, 1, &floor_vert_shader, nullptr));
	glCompileShader(floor_vertex_shader_id);
	CHECK_GL_SHADER_ERROR(floor_vertex_shader_id);

	// Setup fragment shader for the floor
	GLuint floor_fragment_shader_id = 0;
	CHECK_GL_ERROR(floor_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
	CHECK_GL_ERROR(glShaderSource(floor_fragment_shader_id, 1,
				&floor_frag_shader, nullptr));
	glCompileShader(floor_fragment_shader_id);
	CHECK_GL_SHADER_ERROR(floor_fragment_shader_id);

	// FIXME: Setup another program for the floor, and get its locations.
	// Note: you can reuse the vertex and geometry shader objects
	GLuint floor_program_id = 0;
	GLint floor_projection_matrix_location = 0;
	GLint floor_view_matrix_location = 0;
	GLint floor_offset_location = 0;

	CHECK_GL_ERROR(floor_program_id = glCreateProgram());
	CHECK_GL_ERROR(glAttachShader(floor_program_id, floor_vertex_shader_id));
	//FIXME: change the fragment shader
	CHECK_GL_ERROR(glAttachShader(floor_program_id, floor_fragment_shader_id));


//bind attributes
	CHECK_GL_ERROR(glBindAttribLocation(floor_program_id, 0, "vertex_position"));
	CHECK_GL_ERROR(glBindFragDataLocation(floor_program_id, 0, "fragment_color"));
	glLinkProgram(floor_program_id);
	CHECK_GL_PROGRAM_ERROR(floor_program_id);

	//uniform locations
	CHECK_GL_ERROR(floor_projection_matrix_location =
			glGetUniformLocation(floor_program_id, "projection"));
	CHECK_GL_ERROR(floor_view_matrix_location =
			glGetUniformLocation(floor_program_id, "view"));
	CHECK_GL_ERROR(floor_offset_location =
			glGetUniformLocation(floor_program_id, "offset"));



	// CHECK_GL_ERROR(camera_position_location =
	// 		glGetUniformLocation(default_program_id, "camera_position"));
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
    138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180,
	151,160,137,91,90,15,
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

float skyboxVertices[] = {
    // positions
		-1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};


//sky setup
CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kSkyVao]));

CHECK_GL_ERROR(glGenBuffers(kNumVbos, &g_buffer_objects[kSkyVao][0]));

// Setup vertex data in a VBO.
CHECK_GL_ERROR(glBindBuffer(GL_ARRAY_BUFFER, g_buffer_objects[kSkyVao][kVertexBuffer]));
// NOTE: We do not send anything right now, we just describe it to OpenGL.
CHECK_GL_ERROR(glBufferData(GL_ARRAY_BUFFER,
			sizeof(float) * sizeof(skyboxVertices), &skyboxVertices,
			GL_STATIC_DRAW));
CHECK_GL_ERROR(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0));
CHECK_GL_ERROR(glEnableVertexAttribArray(0));

GLuint sky_vertex_shader_id = 0;

CHECK_GL_ERROR(sky_vertex_shader_id = glCreateShader(GL_VERTEX_SHADER));
CHECK_GL_ERROR(glShaderSource(sky_vertex_shader_id, 1, &sky_vertex_shader, nullptr));
glCompileShader(sky_vertex_shader_id);
CHECK_GL_SHADER_ERROR(sky_vertex_shader_id);

GLuint sky_fragment_shader_id = 0;

CHECK_GL_ERROR(sky_fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER));
CHECK_GL_ERROR(glShaderSource(sky_fragment_shader_id, 1, &sky_frag_shader, nullptr));
glCompileShader(sky_fragment_shader_id);
CHECK_GL_SHADER_ERROR(sky_fragment_shader_id);

GLuint sky_program_id = 0;
GLuint sky_permutation_location = 0;
GLint sky_projection_matrix_location = 0;
GLint sky_view_matrix_location = 0;
GLint sky_time_location = 0;
CHECK_GL_ERROR(sky_program_id = glCreateProgram());
CHECK_GL_ERROR(glAttachShader(sky_program_id, sky_vertex_shader_id));
CHECK_GL_ERROR(glAttachShader(sky_program_id, sky_fragment_shader_id));

CHECK_GL_ERROR(glBindAttribLocation(sky_program_id, 0, "vertex_position"));
CHECK_GL_ERROR(glBindFragDataLocation(sky_program_id, 0, "fragment_color"));

glLinkProgram(sky_program_id);
CHECK_GL_PROGRAM_ERROR(sky_program_id);

CHECK_GL_ERROR(sky_permutation_location =
		glGetUniformLocation(sky_program_id, "p"));
CHECK_GL_ERROR(sky_view_matrix_location =
		glGetUniformLocation(sky_program_id, "view"));
CHECK_GL_ERROR(sky_projection_matrix_location =
		glGetUniformLocation(sky_program_id, "projection"));
CHECK_GL_ERROR(sky_time_location =
		glGetUniformLocation(sky_program_id, "t"));


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

		//glfwSetInputMode	(window,GLFW_STICKY_KEYS,GL_TRUE);

	//poll motion keys

	glm::vec4 oldPos = g_camera.get_eye();

	if (glfwGetKey(window, GLFW_KEY_W) != GLFW_RELEASE) {
		// glm::vec4 oldPos = g_camera.get_eye();
		if(!terrain.collision(glm::vec3(oldPos),g_camera.w_can_move())) {
			g_camera.w_move_forward();
			// if(!change_chunk)
			// 	change_chunk = border(oldPos,g_camera.get_eye());
		}
	} else if (glfwGetKey(window, GLFW_KEY_S) != GLFW_RELEASE) {
		//glm::vec4 oldPos = g_camera.get_eye();
		if(!terrain.collision(glm::vec3(oldPos),g_camera.s_can_move())) {
			g_camera.s_move_backward();
			// if(!change_chunk)
			// 	change_chunk = border(oldPos,g_camera.get_eye());
		}
	}


	if (glfwGetKey(window, GLFW_KEY_A) != GLFW_RELEASE)
	{
		
		if(!terrain.collision(glm::vec3(oldPos),g_camera.a_can_move())) {
			g_camera.a_strafe_left();
			// if(!change_chunk)
			// 	change_chunk = border(oldPos,g_camera.get_eye());
		}
	} else if (glfwGetKey(window, GLFW_KEY_D) != GLFW_RELEASE) {
		//glm::vec4 oldPos = g_camera.get_eye();
		if(!terrain.collision(glm::vec3(oldPos),g_camera.d_can_move())) {
			g_camera.d_strafe_right();
			// if(!change_chunk)
			// 	change_chunk = border(oldPos,g_camera.get_eye());
		}
	}
	change_chunk = border(oldPos,g_camera.get_eye());
	g_camera.update_height(terrain.getMaxHeight(glm::vec3(g_camera.get_eye())));

			// Compute the projection matrix.
		aspect = static_cast<float>(window_width) / window_height;
		glm::mat4 projection_matrix =
			glm::perspective(glm::radians(45.0f), aspect, 0.2f, 100.0f);


		// Compute the view matrix
		// FIXME: change eye and center through mouse/keyboard events.
		glm::mat4 view_matrix = g_camera.get_view_matrix();
			//FLOOR
		glm::vec4 eyetemp = g_camera.get_eye();

		float tempx = floor(eyetemp.x/20)*20 ; 
		float tempz = floor(eyetemp.z/20)*20;
		glm::vec4 flooroffset = glm::vec4(tempx,0.0f,tempz,0.0f);

		CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kFloorVao]));

		CHECK_GL_ERROR(glUseProgram(floor_program_id));

		CHECK_GL_ERROR(glUniformMatrix4fv(floor_projection_matrix_location, 1, GL_FALSE,
					&projection_matrix[0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(floor_view_matrix_location, 1, GL_FALSE,
					&view_matrix[0][0]));
		
		// cout<<"floor offset "<<glm::to_string(flooroffset)<<endl;
		CHECK_GL_ERROR(glUniform4fv(floor_offset_location, 1, &flooroffset[0]));

		CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, plane_faces.size() * 3, GL_UNSIGNED_INT, 0));



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
			//cout<<"change chunk"<<endl;
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

	


		// Use our program.
		CHECK_GL_ERROR(glUseProgram(default_program_id));

		// Pass uniforms in.
		CHECK_GL_ERROR(glUniformMatrix4fv(projection_matrix_location, 1, GL_FALSE,
					&projection_matrix[0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(view_matrix_location, 1, GL_FALSE,
					&view_matrix[0][0]));
		CHECK_GL_ERROR(glUniform4fv(light_position_location, 1, &light_position[0]));

		// for (int i = 0; i < permutation.length; ++i) {

		// }
		CHECK_GL_ERROR(glUniform1iv(permutation_location, 512, &p[0]));

		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &ctranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.center_offsets.size()));

		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &uctranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.up_center_offsets.size()));

		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &ltranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.left_offsets.size()));

		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &rtranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.right_offsets.size()));

		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &dctranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.down_center_offsets.size()));


		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &ultranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.up_left_offsets.size()));



		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &urtranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.up_right_offsets.size()));


		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &dltranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.down_left_offsets.size()));


		for (int i = 0; i < num_trans; ++i){
					CHECK_GL_ERROR(glUniform4fv(cube_translation_location[i], 1, &drtranslations[i][0]));
				}
		CHECK_GL_ERROR(glDrawElementsInstanced(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0, terrain.down_right_offsets.size()));

	

		// if (g_camera.is_jumping()) {
		// 	g_camera.update_height(get_heights(g_camera.get_eye(),terrain));
		// }
		//g_camera.update_height(terrain.getMaxHeight(glm::vec3(g_camera.get_eye())));
		// Draw our triangles.
		//CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, obj_faces.size() * 3, GL_UNSIGNED_INT, 0));

		glDepthFunc(GL_LEQUAL);
		CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kSkyVao]));
		CHECK_GL_ERROR(glUseProgram(sky_program_id));

		glm::mat4 skyview = glm::mat4(glm::mat3(view_matrix));

		CHECK_GL_ERROR(glUniform1iv(sky_permutation_location, 512, &p[0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(sky_projection_matrix_location, 1, GL_FALSE,
					&projection_matrix[0][0]));
		CHECK_GL_ERROR(glUniformMatrix4fv(sky_view_matrix_location, 1, GL_FALSE,
					&skyview[0][0]));
		CHECK_GL_ERROR(glUniform1f(sky_time_location, sky_time));
		CHECK_GL_ERROR(glDrawArrays(GL_TRIANGLES, 0, 36));
		glDepthFunc(GL_LESS);

		if (day_night) {
			if (sunrise) {
				sky_time += 0.005;
				if (sky_time > 1.5) {
					sunrise = false;
					sunset = true;
					times++;
				}

			} else if (sunset) {
				sky_time -= 0.005;
				if (sky_time < -1.1) {
					sunset = false;
					sunrise = true;
					times++;
				}
				CHECK_GL_ERROR(glBindVertexArray(g_array_objects[kMoonVao]));

				CHECK_GL_ERROR(glUseProgram(moon_program_id));

				CHECK_GL_ERROR(glUniformMatrix4fv(moon_projection_matrix_location, 1, GL_FALSE,
						&projection_matrix[0][0]));
				CHECK_GL_ERROR(glUniformMatrix4fv(moon_view_matrix_location, 1, GL_FALSE,
						&skyview[0][0]));
				CHECK_GL_ERROR(glUniform1f(moon_time_location, sky_time));
				CHECK_GL_ERROR(glDrawElements(GL_TRIANGLES, moon_faces.size() * 3, GL_UNSIGNED_INT, 0));

						

			}

			if (times == 3) {
				day_night = false;
			}
		}
 		



		// Poll and swap.
		glfwPollEvents();
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "../OpenGLSample/Sphere.h"
#include "cylinder.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

// Perspective
bool useOrtho = false;

int graterVerts = 0;
int handleVerts = 0;
int matVerts = 0;
int flourVerts = 0;
int lidVerts = 0;
int juicerHandleVerts = 0;

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile our shader zprogram
	// ------------------------------------
	Shader lightingShader("shaderfiles/6.multiple_lights.vs", "shaderfiles/6.multiple_lights.fs");
	Shader lightCubeShader("shaderfiles/6.light_cube.vs", "shaderfiles/6.light_cube.fs");

	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float graterVertices[] = {
		// positions          // normals           // texture coords

		-0.5f, -0.5f, -0.5f,  0.0f,  0.2f, -1.0f,  0.0f,  0.0f,				//Back Face
		 0.5f, -0.5f, -0.5f,  0.0f,  0.2f, -1.0f,  1.0f,  0.0f,				//Face slope +1y +0.2z
		 0.3f,  0.5f, -0.3f,  0.0f,  0.2f, -1.0f,  0.8f,  1.0f,				//Face normal +0.2y -1z
		 0.3f,  0.5f, -0.3f,  0.0f,  0.2f, -1.0f,  0.8f,  1.0f,
		-0.3f,  0.5f, -0.3f,  0.0f,  0.2f, -1.0f,   0.2f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.2f, -1.0f,   0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.2f, 1.0f,  0.0f,  0.0f,				//Front Face
		 0.5f, -0.5f,  0.5f,  0.0f,  0.2f, 1.0f,  1.0f,  0.0f,				//Face normal +0.2y +1z
		 0.3f,  0.5f,  0.3f,  0.0f,  0.2f, 1.0f,  0.8f,  1.0f,
		 0.3f,  0.5f,  0.3f,  0.0f,  0.2f, 1.0f,  0.8f,  1.0f,
		-0.3f,  0.5f,  0.3f,  0.0f,  0.2f, 1.0f,  0.2f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.2f, 1.0f,  0.0f,  0.0f,

		-0.3f,  0.5f,  0.3f, -1.0f,  0.2f, 0.0f,  0.8f,  2.0f,				//Left Face
		-0.3f,  0.5f, -0.3f, -1.0f,  0.2f, 0.0f,  0.2f,  2.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.2f, 0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.2f, 0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.2f, 0.0f,  1.0f,  0.0f,
		-0.3f,  0.5f,  0.3f, -1.0f,  0.2f, 0.0f,  0.8f,  2.0f,

		 0.3f,  0.5f,  0.3f, 1.0f,  0.2f, 0.0f,  0.2f,  2.0f,				//Right Face
		 0.3f,  0.5f, -0.3f, 1.0f,  0.2f, 0.0f,  0.8f,  2.0f,				
		 0.5f, -0.5f, -0.5f, 1.0f,  0.2f, 0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f,  0.2f, 0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f,  0.2f, 0.0f,  0.0f,  0.0f,
		 0.3f,  0.5f,  0.3f, 1.0f,  0.2f, 0.0f,  0.2f,  2.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f,  0.0f,				//Bottom Face
		 0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f,  0.0f,				//Face Normal -y
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,  0.0f,  0.0f,

		-0.3f,  0.5f, -0.3f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f,				//Top Face
		 0.3f,  0.5f, -0.3f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f,				//Face normal +y
		 0.3f,  0.5f,  0.3f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f,
		 0.3f,  0.5f,  0.3f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f,
		-0.3f,  0.5f,  0.3f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f,
		-0.3f,  0.5f, -0.3f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f,
	};

	float handleVertices[] = {
		 0.3f,  0.5f, -0.3f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,				//Right Face
		 0.3f,  0.5f,  0.3f, 1.0f, 0.0f, -1.0f,  1.0f,  0.0f,				//Face normal +x
		 0.3f,  0.7f,  0.3f, 1.0f, 0.0f, -1.0f,  1.0f,  1.0f,
		 0.3f,  0.5f, -0.3f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		 0.3f,  0.7f, -0.3f, 1.0f, 0.0f, -1.0f,  0.0f,  1.0f,
		 0.3f,  0.7f,  0.3f, 1.0f, 0.0f, -1.0f,  1.0f,  1.0f,

		-0.3f,  0.5f, -0.3f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,				//Left Face
		-0.3f,  0.5f,  0.3f, 1.0f, 0.0f, -1.0f,  1.0f,  0.0f,				//Face Normal -x
		-0.3f,  0.7f,  0.3f, 1.0f, 0.0f, -1.0f,  1.0f,  1.0f,
		-0.3f,  0.5f, -0.3f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.3f,  0.7f, -0.3f, 1.0f, 0.0f, -1.0f,  0.0f,  1.0f,
		-0.3f,  0.7f,  0.3f, 1.0f, 0.0f, -1.0f,  1.0f,  1.0f,

		-0.3f,  0.7f, -0.3f, 0.0f, 1.0f, -0.0f,  1.0f,  0.0f,				//Top Face
		 0.3f,  0.7f, -0.3f, 0.0f, 1.0f, -0.0f,  1.0f,  1.0f,				//Face Normal +y
		 0.3f,  0.7f,  0.3f, 0.0f, 1.0f, -0.0f,  0.0f,  1.0f,
		 0.3f,  0.7f,  0.3f, 0.0f, 1.0f, -0.0f,  0.0f,  1.0f,
		-0.3f,  0.7f,  0.3f, 0.0f, 1.0f, -0.0f,  0.0f,  0.0f,
		-0.3f,  0.7f, -0.3f, 0.0f, 1.0f, -0.0f,  1.0f,  0.0f,
	};
	float matVertices[] = {
		 4.0f,  -0.5f, -4.0f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,				//Mat normal +y
		 4.0f,  -0.5f,  4.0f, 0.0f, 1.0f, 0.0f,  1.0f,  1.0f,
		-4.0f,  -0.5f,  4.0f, 0.0f, 1.0f, 0.0f,  0.0f,  1.0f,
		-4.0f,  -0.5f, -4.0f, 0.0f, 1.0f, 0.0f,  0.0f,  0.0f,
		-4.0f,  -0.5f,  4.0f, 0.0f, 1.0f, 0.0f,  0.0f,  1.0f,
		 4.0f,  -0.5f,  -4.0f, 0.0f, 1.0f, 0.0f,  1.0f,  0.0f,
	};
	float flourVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	float lidVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	Sphere juicer(1.0, 20, 20);

	float juicerHandleVertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	

	// positions all containers
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(-3.0f,  -0.5f, -1.0f),
		glm::vec3(-3.0f, 1.0f, -1.0f),
		glm::vec3(2.0f, -1.5f, 1.0f),
		glm::vec3(4.5f, -1.3f, 1.0f),
		glm::vec3(-5.5f,  0.1f, 1.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	// positions of the point lights
	glm::vec3 pointLightPositions[] = {
		glm::vec3(1.0f,  2.0f,  1.0f),
		glm::vec3(-2.0f, 2.0f, 2.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};
	// first, configure the cube's VAO (and VBO)
	unsigned int graterVBO, graterVAO;
	glGenVertexArrays(1, &graterVAO);
	glGenBuffers(1, &graterVBO);

	graterVerts = sizeof(graterVertices);
	handleVerts = sizeof(handleVertices);
	matVerts = sizeof(matVertices);
	flourVerts = sizeof(flourVertices);
	lidVerts = sizeof(lidVertices);
	juicerHandleVerts = sizeof(juicerHandleVertices);

	glBindBuffer(GL_ARRAY_BUFFER, graterVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(graterVertices), graterVertices, GL_STATIC_DRAW);
	
	glBindVertexArray(graterVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	unsigned int handleVBO, handleVAO;
	glGenVertexArrays(1, &handleVAO);
	glGenBuffers(1, &handleVBO);

	glBindBuffer(GL_ARRAY_BUFFER, handleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(handleVertices), handleVertices, GL_STATIC_DRAW);

	glBindVertexArray(handleVAO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);


	unsigned int matVBO, matVAO;
	glGenVertexArrays(1, &matVAO);
	glGenBuffers(1, &matVBO);

	glBindBuffer(GL_ARRAY_BUFFER, matVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(matVertices), matVertices, GL_STATIC_DRAW);

	glBindVertexArray(matVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int flourVBO, flourVAO;
	glGenVertexArrays(1, &flourVAO);
	glGenBuffers(1, &flourVBO);

	glBindBuffer(GL_ARRAY_BUFFER, flourVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(flourVertices), flourVertices, GL_STATIC_DRAW);

	glBindVertexArray(flourVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int lidVBO, lidVAO;
	glGenVertexArrays(1, &lidVAO);
	glGenBuffers(1, &lidVBO);

	glBindBuffer(GL_ARRAY_BUFFER, lidVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lidVertices), lidVertices, GL_STATIC_DRAW);

	glBindVertexArray(lidVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int juicerVBO, juicerVAO;
	glGenVertexArrays(1, &juicerVAO);
	glGenBuffers(1, &juicerVBO);

	glBindBuffer(GL_ARRAY_BUFFER, juicerVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(juicerHandleVertices), juicerHandleVertices, GL_STATIC_DRAW);

	glBindVertexArray(juicerVAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	unsigned int diffuseMap1 = loadTexture("cheesegrater.png");
	unsigned int diffuseMap2 = loadTexture("BlackPlastic.png");
	unsigned int diffuseMap3 = loadTexture("GrayVinyl.png");
	unsigned int specularMap = loadTexture("cheesegrater.png");
	unsigned int specularMap2 = loadTexture("BlackPlastic.png");
	unsigned int specularMap3 = loadTexture("GrayVinyl.png");
	unsigned int diffuseMap4 = loadTexture("FlourTexture.png");
	unsigned int specularMap4 = loadTexture("FlourTexture.png");
	unsigned int diffuseMap5 = loadTexture("JuicerTexture.png");
	unsigned int specularMap5 = loadTexture("JuicerTexture.png");
	unsigned int diffuseMap6 = loadTexture("LidTexture.png");
	unsigned int specularMap6 = loadTexture("LIdTexture.png");
	unsigned int diffuseMap7 = loadTexture("SaltTexture.png");
	unsigned int specularMap7 = loadTexture("SaltTexture.png");

	// shader configuration
	// --------------------
	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);
	lightingShader.setInt("material.specular", 1);


	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);
		lightingShader.setFloat("material.shininess", 32.0f);

		/*
		   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/
		// directional light
		lightingShader.setVec3("dirLight.direction", 0.0f, -1.0f, 0.0f);
		lightingShader.setVec3("dirLight.ambient", 0.5f, 0.5f, 0.5f);
		lightingShader.setVec3("dirLight.diffuse", 1.0f, 01.0f, 1.0f);
		lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		lightingShader.setVec3("pointLights[0].ambient", 0.1f, 0.1f, 0.1f);
		lightingShader.setVec3("pointLights[0].diffuse", 0.5f, 0.5f, 0.2f);
		lightingShader.setVec3("pointLights[0].specular", 0.5f, 0.5f, 0.2f);
		lightingShader.setFloat("pointLights[0].constant", 1.0f);
		lightingShader.setFloat("pointLights[0].linear", 0.09);
		lightingShader.setFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		lightingShader.setVec3("pointLights[1].ambient", 0.1f, 0.1f, 0.1f);
		lightingShader.setVec3("pointLights[1].diffuse", 0.5f, 0.5f, 0.2f);
		lightingShader.setVec3("pointLights[1].specular", 0.5f, 0.5f, 0.2f);
		lightingShader.setFloat("pointLights[1].constant", 1.0f);
		lightingShader.setFloat("pointLights[1].linear", 0.09);
		lightingShader.setFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		lightingShader.setVec3("pointLights[2].ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("pointLights[2].diffuse", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("pointLights[2].specular", 0.0f, 0.0f, 0.0f);
		lightingShader.setFloat("pointLights[2].constant", 1.0f);
		lightingShader.setFloat("pointLights[2].linear", 0.09);
		lightingShader.setFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		lightingShader.setVec3("pointLights[3].ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("pointLights[3].diffuse", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("pointLights[3].specular", 0.0f, 0.0f, 0.0f);
		lightingShader.setFloat("pointLights[3].constant", 1.0f);
		lightingShader.setFloat("pointLights[3].linear", 0.09);
		lightingShader.setFloat("pointLights[3].quadratic", 0.032);
		// spotLight
		lightingShader.setVec3("spotLight.position", camera.Position);
		lightingShader.setVec3("spotLight.direction", camera.Front);
		lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLight.diffuse", 0.0f, 0.0f, 0.0f);
		lightingShader.setVec3("spotLight.specular", 0.0f, 0.0f, 0.0f);
		lightingShader.setFloat("spotLight.constant", 1.0f);
		lightingShader.setFloat("spotLight.linear", 0.09);
		lightingShader.setFloat("spotLight.quadratic", 0.032);
		lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// view/projection transformations
		glm::mat4 projection;
		if (useOrtho == true)
		{
			float scale = 100;
			projection = glm::ortho(-((float)SCR_WIDTH / scale), ((float)SCR_WIDTH / scale), -((float)SCR_HEIGHT / scale), ((float)SCR_HEIGHT / scale), -5.0f, 100.0f);
		}
		else
		{
			projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		}
		
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

		// bind diffuse map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap1);

		// bind diffuse map
		

		// bind specular map
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap);

		// render containers
		glBindVertexArray(graterVAO);
		//for (unsigned int i = 0; i < 1; i++)
		//{
			// calculate the model matrix for each object and pass it to shader before drawing
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[0]);
			model = glm::scale(model, glm::vec3(2.0f, 3.0f, 1.0f));
			float angle = 20.0f * 0;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, graterVerts);
		//}

		// also draw the lamp object(s)
		/*lightCubeShader.use();
		lightCubeShader.setMat4("projection", projection);
		lightCubeShader.setMat4("view", view);*/

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap2);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap2);

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(handleVAO);
		//for (unsigned int i = 0; i < 0; i++)
		//{
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[0]);
			model = glm::scale(model, glm::vec3(2.0f, 3.0f, 1.0f));
			angle = 20.0f * 0;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, handleVerts);
		//}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap3);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap3);

		// we now draw as many light bulbs as we have point lights.
		glBindVertexArray(matVAO);
		//for (unsigned int i = 0; i < 0; i++)
		//{
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[0]);
		model = glm::scale(model, glm::vec3(2.0f, 3.0f, 1.0f));
		angle = 20.0f * 0;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, matVerts);
		//}

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap4);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap4);

		glBindVertexArray(flourVAO);
		//for (unsigned int i = 0; i < 0; i++)
		//{
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[1]);
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		angle = 20.0f * 2;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, flourVerts);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap6);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap6);

		glBindVertexArray(lidVAO);
		//for (unsigned int i = 0; i < 0; i++)
		//{
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[2]);
		model = glm::scale(model, glm::vec3(2.65f, 0.5f, 2.65f));
		angle = 20.0f * 2;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
		lightingShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, lidVerts);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap5);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap5);
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[3]);
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		angle = 20.0f * 0;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		lightingShader.setMat4("model", model);

		juicer.Draw();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap5);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap5);

		glBindVertexArray(juicerVAO);

		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[4]);
		model = glm::scale(model, glm::vec3(4.0f, 0.5f, 0.3f));
		angle = 20.0f * 0;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, juicerHandleVerts);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap7);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap7);

		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[5]);
		model = glm::scale(model, glm::vec3(0.5f, 0.8f, 0.5f));
		angle = 20.0f * 0;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		lightingShader.setMat4("model", model);

		static_meshes_3D::Cylinder salt(2, 20, 3, true, true, true);
		salt.render();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseMap6);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularMap6);

		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[5]);
		model = glm::scale(model, glm::vec3(0.49f, 1.0f, 0.49f));
		angle = 20.0f * 0;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		lightingShader.setMat4("model", model);

		static_meshes_3D::Cylinder saltTop(2, 20, 3, true, true, true);
		saltTop.render();
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &graterVAO);
	glDeleteVertexArrays(1, &handleVAO);
	glDeleteBuffers(1, &graterVBO);
	glDeleteBuffers(1, &handleVBO);
	glDeleteVertexArrays(1, &matVAO);
	glDeleteBuffers(1, &matVBO);
	glDeleteVertexArrays(1, &flourVAO);
	glDeleteBuffers(1, &flourVBO);
	glDeleteVertexArrays(1, &lidVAO);
	glDeleteBuffers(1, &lidVBO);
	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	float cameraOffset = camera.MovementSpeed * deltaTime;		// Calculate amount of camera movement with current camera movement sensitivity

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)			// 'Q' key now pans up
		camera.Position += camera.Up * cameraOffset;			// modify Up vector of camera position by camera offset
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)			// 'E' key pans down
		camera.Position -= camera.Up * cameraOffset;			// modify Up vector of camera position by camera offset

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)			//Toggle Perspective
		useOrtho = !useOrtho;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//camera.ProcessMouseScroll(yoffset);
	camera.MovementSpeed += yoffset;	// increases or decreases camera movement speed by amount of scroll wheel inuput
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}
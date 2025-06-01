#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"
#include "spheremesh.h"

#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "object.h"
#include "lights.h"

#define M_PI 3.14159265358979323846 

// consts
// ------
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderPath = "src/objVertex.vert";
const char* fragmentShaderPath = "src/objFragment.frag";

const char* textureColorPath = "resorces/Tiles/Tiles130_Color.png";
const char* textureSpecularPath = "resorces/Tiles/Tiles130_AmbientOcclusion.png";
const char* textureNormalPath = "resorces/Tiles/Tiles130_NormalGL.png";
const char* textureDisplacementPath = "resorces/Tiles/Tiles130_Displacement.png";

const char* textureColorBrickPath = "resorces/BrickTexture/brickwall.jpg";
const char* textureNormalBrickPath = "resorces/BrickTexture/brickwall_normal.jpg";
const char* textureDisplacementBrickPath = "resorces/BrickTexture/brickwall_disp.jpg";

const char* textureColorBoxPath = "resorces/BoxTexture/box.png";
const char* textureSpecularBoxPath = "resorces/BoxTexture/box_specular.png";

const char* textureColorSpherePath = "resorces/NoiseTexture/green.png";
const char* textureNormalSpherePath = "resorces/NoiseTexture/noise.png";

const char* lightvsPath = "src/lightVertex.vert";
const char* lightfsPath = "src/lightFragment.frag";

// global var
// ----------
// cube data
std::vector<Vertex> vertices_data = {
	// fața frontală
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f}},
	{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
	{{ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f}},
	{{-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f}},

	// fața din spate
	{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 0.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {0.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 1.0f}},
	{{-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f,  1.0f}, {1.0f, 0.0f}},


	// fața stângă
	{{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
	{{-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
	{{-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},

	// fața dreaptă
	{{ 0.5f,  0.5f,  0.5f}, { 1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
	{{ 0.5f,  0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
	{{ 0.5f, -0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, { 1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.5f}, { 1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, { 1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},

	// fața de jos
	{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},
	{{ 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
	{{ 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
	{{-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
	{{-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f}},

	// fața de sus
	{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
	{{ 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
	{{ 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
	{{-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}},
	{{-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},

};

std::vector<unsigned int> indices = {
	0, 1, 2, 3, 4, 5,
	6, 7, 8, 9, 10, 11,
	12, 13, 14, 15, 16, 17,
	18, 19, 20, 21, 22, 23,
	24, 25, 26, 27, 28, 29,
	30, 31, 32, 33, 34, 35
};

// cubes position
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),  // Primul cub
	glm::vec3(2.0f, 0.0f, 0.0f),  // Al doilea cub
	glm::vec3(4.0f, 0.0f, 0.0f),  // Al treilea cub
	glm::vec3(6.0f, 0.0f, 0.0f),  // Al patrulea cub
	glm::vec3(8.0f, 0.0f, 0.0f),  // Al cincilea cub
	glm::vec3(10.0f, 0.0f, 0.0f), // Al șaselea cub
	glm::vec3(12.0f, 0.0f, 0.0f)  // Al șaptelea cub
};

// lights position
//glm::vec3 pointLightPositions[] = {
//	glm::vec3(0.7f, 0.2f, 2.0f),
//	glm::vec3(2.3f, -3.3f, -4.0f),
//	glm::vec3(-4.0f, 2.0f, -12.0f),
//	glm::vec3(0.0f, 0.0f, -3.0f)
//};

// cubes
vector<Object> cubes;
vector<Object> spheres;

// camera
Camera camera(glm::vec3(0.0, 0.0, 3.0));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// lights
vector<PointLight> pointLights;
DirLight dirLight;
SpotLight spotLight(camera.Position, camera.Front);

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

// function definition
// -------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path);
void ConvertSphereMesh(const SphereMesh& mesh, std::vector<Vertex>& outVertices, std::vector<unsigned int>& outIndices);

int main() {

	// initialization
	// --------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// configure global opengl state
	// -----------------------------
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
	glEnable(GL_DEPTH_TEST);

	// build and create shader
	// -----------------------
	Shader shaderProgram(vertexShaderPath, fragmentShaderPath);
	Shader lightShaderProgram(lightvsPath, lightfsPath);

	// create VAO buffer
	//------------------
	// cube buffer
	std::vector<Texture> textures = {
		{loadTexture(textureColorPath), "texture_diffuse"},
		{loadTexture(textureSpecularPath), "texture_specular"},
		{loadTexture(textureNormalPath), "texture_normal"},
		{loadTexture(textureDisplacementPath), "texture_displacement"}
	};

	std::vector<Texture> texturesTest1 = {
		{loadTexture(textureColorPath), "texture_diffuse"},
	};
	std::vector<Texture> texturesTest2 = {
		{loadTexture(textureColorPath), "texture_diffuse"},
		{loadTexture(textureSpecularPath), "texture_specular"},
	};
	std::vector<Texture> texturesTest3 = {
		{loadTexture(textureColorPath), "texture_diffuse"},
		{loadTexture(textureSpecularPath), "texture_specular"},
		{loadTexture(textureNormalPath), "texture_normal"},
	};

	std::vector<Texture> brickTexture = {
		{loadTexture(textureColorBrickPath), "texture_diffuse"},
		{loadTexture(textureColorBrickPath), "texture_specular"},
		{loadTexture(textureNormalBrickPath), "texture_normal"},
		{loadTexture(textureDisplacementBrickPath), "texture_displacement"}
	};

	std::vector<Texture> boxTexture = {
		{loadTexture(textureColorBoxPath), "texture_diffuse"},
		{loadTexture(textureSpecularBoxPath), "texture_specular"},
	};

	Mesh cubeGreenMesh(vertices_data, indices);
	Mesh cubeTest1Mesh(vertices_data, indices, texturesTest1);
	Mesh cubeTest2Mesh(vertices_data, indices, texturesTest2);
	Mesh cubeTest3Mesh(vertices_data, indices, texturesTest3);
	Mesh cubeMesh(vertices_data, indices, textures);
	Mesh brickMesh(vertices_data, indices, brickTexture);
	Mesh boxMesh(vertices_data, indices, boxTexture);

	cubes.push_back(Object(brickMesh, cubePositions[0], glm::vec3(0.0f)));

	cubes.push_back(Object(boxMesh, cubePositions[1], glm::vec3(0.0f)));

	cubes.push_back(Object(cubeGreenMesh, cubePositions[2]));
	cubes.push_back(Object(cubeTest1Mesh, cubePositions[3]));
	cubes.push_back(Object(cubeTest2Mesh, cubePositions[4]));
	cubes.push_back(Object(cubeTest3Mesh, cubePositions[5]));
	cubes.push_back(Object(cubeMesh, cubePositions[6]));

	// sphere buffer
	std::vector<Vertex> sphereVertices;
	std::vector<unsigned int> sphereIndices;
	std::vector<Texture> sphereTexture = {
		{loadTexture(textureColorBrickPath), "texture_diffuse"},
		{loadTexture(textureColorBrickPath), "texture_specular"},
		{loadTexture(textureNormalBrickPath), "texture_normal"}
	};
	SphereMesh s(3);
	ConvertSphereMesh(s, sphereVertices, sphereIndices);

	Mesh sphereMesh(sphereVertices, sphereIndices, sphereTexture, false);
	Mesh sphereGreenMesh(sphereVertices, sphereIndices, vector<Texture>(), false);
	spheres.push_back(Object(sphereMesh, glm::vec3(-2.0f, 0.0f, 0.0f)));
	spheres.push_back(Object(sphereGreenMesh, glm::vec3(-4.0f, 0.0f, 0.0f)));
	
	// lights position
	std::vector<glm::vec3> pointLightPositions;
	for (const auto& cubePos : cubePositions) {
		pointLightPositions.push_back(cubePos + glm::vec3(1.0f, 1.0f, 1.0f));
	}

	for (int i = 0; i < pointLightPositions.size(); i++) {
		pointLights.push_back(PointLight(cubeMesh, pointLightPositions[i], glm::vec3(0.0f), glm::vec3(0.2f)));
	}
	
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// inputs
		// ------
		processInput(window);

		// clear renderer
		// --------------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render
		// ------
		// set shader
		shaderProgram.use();
		shaderProgram.setVec3("viewPos", camera.Position);

		shaderProgram.setFloat("material.shininess", 32.0f);
		shaderProgram.setFloat("material.height_scale", 0.1f);

		// directional light setup
		dirLight.loadLight(shaderProgram);

		// point lights setup
		shaderProgram.setInt("nrPointLights", pointLights.size());

		for (int i = 0; i < pointLights.size(); i++) {
			pointLights[i].loadLight(shaderProgram, i);
		}

		// spot light setup
		spotLight.update(camera.Position, camera.Front);
		spotLight.loadLight(shaderProgram);
		

		glm::mat4 projection = glm::mat4(1.0f);
		projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		shaderProgram.setMat4("projection", projection);

		glm::mat4 view = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		shaderProgram.setMat4("view", view);

		// draw objects
		glm::mat4 model = glm::mat4(1.0f);

		for (unsigned int i = 0; i < cubes.size(); i++)
		{
			cubes[i].Draw(shaderProgram);
		}

		for (unsigned int i = 0; i < spheres.size(); i++) {
			spheres[i].Draw(shaderProgram);
		}

		// draw light
		lightShaderProgram.use();
		lightShaderProgram.setMat4("projection", projection);
		lightShaderProgram.setMat4("view", view);

		for (int i = 0; i < pointLights.size(); i++) {
			//lightShaderProgram.setVec4("lightColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
			pointLights[i].Draw(lightShaderProgram);
		};

		// check and call events and swap the buffers
		// ------------------------------------------
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	static bool keyWasPressed[10] = { false }; // Stare pentru tastele 1-0
	const double cooldown = 0.3; // Timp minim între apăsări
	static double lastToggleTime[10] = { 0.0 }; // Timpul ultimei apăsări pentru fiecare tastă

	static bool lWasPressed = false;
	static double lLastToggleTime = 0.0;
	static bool oWasPressed = false;
	static double oLastToggleTime = 0.0;

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

	// Funcționalitate existentă pentru tastele L și O
	int lState = glfwGetKey(window, GLFW_KEY_L);
	int oState = glfwGetKey(window, GLFW_KEY_O);
	double currentTime = glfwGetTime();

	if (lState == GLFW_PRESS && !lWasPressed && (currentTime - lLastToggleTime) > cooldown) {
		spotLight.isOn = spotLight.isOn == 1 ? 0 : 1;
		lLastToggleTime = currentTime;
		lWasPressed = true;
	}

	if (lState == GLFW_RELEASE) {
		lWasPressed = false;
	}

	if (oState == GLFW_PRESS && !oWasPressed && (currentTime - oLastToggleTime) > cooldown) {
		dirLight.isOn = dirLight.isOn == 1 ? 0 : 1;
		oLastToggleTime = currentTime;
		oWasPressed = true;
	}

	if (oState == GLFW_RELEASE) {
		oWasPressed = false;
	}

	// Adăugare funcționalitate pentru tastele 1-0
	for (int i = 0; i < 10; i++) {
		int key = GLFW_KEY_0 + (i + 1) % 10; // Tasta curentă (1-9, 0)
		currentTime = glfwGetTime();

		if (glfwGetKey(window, key) == GLFW_PRESS && !keyWasPressed[i] && (currentTime - lastToggleTime[i]) > cooldown) {
			if (i < pointLights.size()) {
				pointLights[i].isOn = !pointLights[i].isOn; // Inversează starea luminii
				pointLights[i].specular = pointLights[i].isOn ? glm::vec3(1.0f) : glm::vec3(0.0f); // Setează specular la alb dacă e activă, altfel la negru
			}
			lastToggleTime[i] = currentTime;
			keyWasPressed[i] = true;
		}

		if (glfwGetKey(window, key) == GLFW_RELEASE) {
			keyWasPressed[i] = false;
		}
	}
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; 

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
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

// helper: clamping și unwrap seam pentru UV
static glm::vec2 ComputeSphericalUV(const glm::vec3& pos) {
	// 1) clamp Y pentru asin()
	float y = glm::clamp(pos.y, -1.0f, 1.0f);

	// 2) calcul θ și φ
	float theta = std::atan2(pos.z, pos.x);                 // [-π, π]
	float phi = std::asin(y);                             // [-π/2, π/2]

	// 3) mapare în [0,1]
	float u = theta / (2.0f * glm::pi<float>()) + 0.5f;      // [-.5,.5]→[0,1]
	float v = 0.5f - phi / glm::pi<float>();                // [0,1]

	// 4) wrap u (în caz că e aproape de 0 sau 1)
	if (u < 0.0f) u += 1.0f;
	if (u > 1.0f) u -= 1.0f;

	return { u, glm::clamp(v, 0.0f, 1.0f) };
}

void ConvertSphereMesh(
	const SphereMesh& mesh,
	std::vector<Vertex>& outVertices,
	std::vector<unsigned int>& outIndices)
{
	outVertices.clear();
	outIndices.clear();

	// 1) poziții, normale, UV inițiale
	outVertices.reserve(mesh.vertices.size());
	for (auto const& p : mesh.vertices) {
		Vertex v;
		v.Position = p;
		v.Normal = glm::normalize(p);
		v.TextCoord = ComputeSphericalUV(p);
		v.Tangent = glm::vec3(0.0f);
		v.Bitangent = glm::vec3(0.0f);
		outVertices.push_back(v);
	}

	// 2) indicii
	outIndices.reserve(mesh.triangles.size() * 3);
	for (auto const& tri : mesh.triangles) {
		outIndices.push_back((unsigned int)tri.x);
		outIndices.push_back((unsigned int)tri.y);
		outIndices.push_back((unsigned int)tri.z);
	}

	// 3) calculează tangente/bitangente pe triunghi
	auto fixSeam = [](glm::vec2& a, glm::vec2& b) {
		float du = b.x - a.x;
		if (du > 0.5f) b.x -= 1.0f;
		if (du < -0.5f) b.x += 1.0f;
		};

	for (size_t i = 0; i < outIndices.size(); i += 3) {
		Vertex& v0 = outVertices[outIndices[i + 0]];
		Vertex& v1 = outVertices[outIndices[i + 1]];
		Vertex& v2 = outVertices[outIndices[i + 2]];

		auto p0 = v0.Position, p1 = v1.Position, p2 = v2.Position;
		glm::vec2 uv0 = v0.TextCoord, uv1 = v1.TextCoord, uv2 = v2.TextCoord;

		// unwrap seam
		fixSeam(uv0, uv1);
		fixSeam(uv0, uv2);

		glm::vec3 dp1 = p1 - p0, dp2 = p2 - p0;
		glm::vec2 duv1 = uv1 - uv0, duv2 = uv2 - uv0;

		float denom = duv1.x * duv2.y - duv2.x * duv1.y;
		if (fabs(denom) < 1e-6f)
			continue;

		float r = 1.0f / denom;
		glm::vec3 tangent = r * (duv2.y * dp1 - duv1.y * dp2);
		glm::vec3 bitangent = r * (-duv2.x * dp1 + duv1.x * dp2);

		v0.Tangent += tangent;
		v1.Tangent += tangent;
		v2.Tangent += tangent;
		v0.Bitangent += bitangent;
		v1.Bitangent += bitangent;
		v2.Bitangent += bitangent;
	}

	// 4) ortonormalizează și fallback pentru zero‐length
	for (auto& v : outVertices) {
		glm::vec3 n = glm::normalize(v.Normal);
		glm::vec3 t = v.Tangent;

		if (glm::dot(t, t) < 1e-6f) {
			// alegem arbitrar o tangentă perpendiculară
			t = glm::normalize(glm::cross(n, glm::vec3(0.0f, 1.0f, 0.0f)));
		}
		else {
			// Gram‐Schmidt
			t = glm::normalize(t - n * glm::dot(n, t));
		}

		glm::vec3 b = glm::cross(n, t);
		float handedness = (glm::dot(b, v.Bitangent) < 0.0f) ? -1.0f : 1.0f;
		b = glm::normalize(b) * handedness;

		v.Tangent = t;
		v.Bitangent = b;
	}
}


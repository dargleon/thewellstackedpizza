/*
Author: Argüello León Dante Moisés
Date: Jun.8.2023 
License: MIT

This code allows you to load individual and composite models that are part of a representation of The Well Stacked Pizza pizzeria from GTA: San Andreas. 
It uses OpenGL and external libraries such as SOIL2, GLM, GLFW, GLEW. Some objects contain animations.
*/

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 720;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(-1.8f, 6.0f, 40.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(2.53f,3.0f, -10.3f),
	glm::vec3(2.53f,30.0f, 90.0f),
	glm::vec3(-10.1f, 2.6f, 0.0f), //Ventana Izquierda al Fondo
	glm::vec3(-10.1f, 2.6f, 8.0f), //Ventana Izquierda al centro
	glm::vec3(9.85f, 2.6f, 0.0f), //Ventana Derecha al Fondo
	glm::vec3(9.85f, 2.6f, 8.0f) //Ventana Derecha al centro
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};



glm::vec3 Light1 = glm::vec3(0);

//Variables de animacion
float movSilla = 0.0f;
float movCanasta = 0.0f;
float movHornoPizz = 0.0f;
float rotPuertaPrincipal = 0.0f;
float rotPuertaC1 = 0.0f;
float rotPuertaC2 = 0.0f;
float rotPizzas = 0.0f;
float rotMicroondas = 0.0f;
float rotMesa = 0.0f;
float tiempo;
float speed;

bool sillaAnim = false;
bool puertaAnim = false;
bool puertaC1Anim = false;
bool puertaC2Anim = false;
bool pizzasAnim = false;
bool aceiteAnim = false;
bool canastaAnim = false;
bool mesaAnim = false;
bool microondasAnim = false;
bool hornoPizzAnim = false;

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "The Well Stacked Pizza by Dante Arguello", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader SkyBoxShader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag");
	Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");
	
	//Model Piso((char*)"Models/Interior/piso.obj");
	Model Interior((char*)"Models/Interior/interiorpizzeria.obj");
	Model Fachada((char*)"Models/Fachada/fachada.obj");
	Model LogosFachada((char*)"Models/Fachada/logos.obj");
	Model BasesLamps((char*)"Models/Lamps/bases.obj");
	Model Lamps((char*)"Models/Lamps/lamps.obj");

	//Puertas
	Model PuertaPrin((char*)"Models/Puertas/puertaprincipal.obj");
	Model PuertaC1((char*)"Models/Puertas/puertacocina1.obj");
	Model VidrioPC1((char*)"Models/Puertas/vidriopcocina1.obj");
	Model PuertaC2((char*)"Models/Puertas/puertacocina2.obj");
	Model VidrioPC2((char*)"Models/Puertas/vidriopcocina2.obj");

	//Elementos del Comedor
	//Model Sillon((char*)"Models/Sillon/sillon.obj");
	Model Silla((char*)"Models/SillaV3/silla.obj"); //Silla individual
	//Model MesaChica((char*)"Models/MesaChicaV2/mesachica.obj"); //Mesa Chica individual
	//Model MesaLarga((char*)"Models/MesaLarga/mesaLarga.obj");	  //Mesa Larga individual

	//Elementos de la Cocina
	Model Freidora((char*)"Models/Freidora/freidora.obj");
	Model Aceite((char*)"Models/Freidora/aceite.obj");
	Model Canasta((char*)"Models/Freidora/canasta.obj");
	Model Parrilla((char*)"Models/Parrilla/parrilla.obj");
	Model ParrillaChica((char*)"Models/ParrillaChica/parrillachica.obj");
	Model VitStructure((char*)"Models/Vitrina/estructura.obj");
	Model VitGlass((char*)"Models/Vitrina/vidrio.obj");
	Model VitPizzas((char*)"Models/Vitrina/pizzas.obj");
	Model MesaPreparacion((char*)"Models/MesaDePreparacion/mesadepreparacion.obj");
	Model MesaConPuertas((char*)"Models/MesaConPuertas/mesaConPuertas.obj");
	Model PuertaMCP((char*)"Models/MesaConPuertas/puerta.obj");
	Model HornoStructure((char*)"Models/Horno/hornoStructure.obj");
	Model HornoInside((char*)"Models/Horno/hornoInside.obj");
	Model CajaRegistradora((char*)"Models/CajaRegistradora/CajaRegistradora.obj");
	Model MicroondasHorno((char*)"Models/Microondas/estructura.obj");
	Model MicroondasPuerta((char*)"Models/Microondas/puerta.obj");

	//Conjuntos de Objetos
	Model MesasYSillas((char*)"Models/MesasYSillas/mesasYSillas.obj"); //Conjunto de sillas y mesas chicas
	Model MesasYSillones((char*)"Models/MesasLargasYSillones/mesasLYSillones.obj"); //Conjunto de sillones y mesas largas
	
	GLfloat skyboxVertices[] = {
		// Positions
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

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//SkyBox
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Load textures
	vector<const GLchar*> faces;
	faces.push_back("SkyBox/right.tga");
	faces.push_back("SkyBox/left.tga");
	faces.push_back("SkyBox/top.tga");
	faces.push_back("SkyBox/bottom.tga");
	faces.push_back("SkyBox/back.tga");
	faces.push_back("SkyBox/front.tga");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"),1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light

		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f); //Direccion de la Luz
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.25f, 0.25f, 0.25f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.4f, 0.4f, 0.4f);

		
		/*
		// Point light Animado

		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.01f, 0.01f, 0.01f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);
		*/

		// Point light 1 (Cocina)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.08f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.005f);
		 
		// Point light 2 (Sol)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 1.5f, 1.5f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.3f, 0.3f, 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.0f);

		// Point light 3 (Ventana izquierda al fondo)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.7f, 0.7f, 0.7f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.075f);

		// Point light 4 (Ventana izquierda al centro)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.7f, 0.7f, 0.7f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.075f);

		// Point light 5 (Ventana derecha al fondo)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), 0.7f, 0.7f, 0.7f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 0.075f);

		// Point light 6 (Ventana derecha al centro)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].position"), pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].ambient"), 0.5f, 0.5f, 0.5f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].diffuse"), 0.7f, 0.7f, 0.7f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].specular"), 0.2f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].quadratic"), 0.075f);

		// SpotLight en camara
		/*
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight[0].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].quadratic"), 0.44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight[0].outerCutOff"), glm::cos(glm::radians(15.0f)));
		*/

		// SpotLight de vitrina de pizzas No. 1

		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].position"), -2.2f, 2.27f, -5.9f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].direction"), 0.0f, -1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].ambient"), 0.1f, 0.1f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].diffuse"), 0.4f, 0.4f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[0].specular"), 0.3f, 0.3f, 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].quadratic"), 0.44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].cutOff"), glm::cos(glm::radians(35.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[0].outerCutOff"), glm::cos(glm::radians(70.0f)));


		// SpotLight de vitrina de pizzas No. 2

		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].position"), 6.7f, 2.27f, -5.9f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].direction"), 0.0f, -1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].ambient"), 0.1f, 0.1f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].diffuse"), 0.4f, 0.4f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[1].specular"), 0.3f, 0.3f, 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].linear"), 0.35f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].quadratic"), 0.44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].cutOff"), glm::cos(glm::radians(35.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[1].outerCutOff"), glm::cos(glm::radians(70.0f)));

		// SpotLight de cocina No. 1

		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].position"), 4.88f, 4.0f, -10.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].direction"), 0.0f, -1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].diffuse"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[2].specular"), 0.1f, 0.1f, 0.1f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].linear"), 0.08f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].quadratic"), 0.08f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].cutOff"), glm::cos(glm::radians(35.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[2].outerCutOff"), glm::cos(glm::radians(70.0f)));

		// SpotLight de cocina No. 2

		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[3].position"), 0.19f, 4.0f, -10.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[3].direction"), 0.0f, -1.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[3].ambient"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[3].diffuse"), 0.2f, 0.2f, 0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLights[3].specular"), 0.1f, 0.1f, 0.1f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[3].linear"), 0.08f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[3].quadratic"), 0.08f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[3].cutOff"), glm::cos(glm::radians(35.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLights[3].outerCutOff"), glm::cos(glm::radians(70.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Carga de modelo 
		view = camera.GetViewMatrix();
		
		//Interior de la Pizzeria
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Interior.Draw(lightingShader);

		//Fachada de la Pizzeria
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Fachada.Draw(lightingShader);

		//Logos de la Fachada
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		LogosFachada.Draw(lightingShader);

		//Se dibujan las bases de las lamparas
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		BasesLamps.Draw(lightingShader);

		//Se dibujan los cristales de las lamparas
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.60);
		Lamps.Draw(lightingShader);
		glDisable(GL_BLEND); //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0); //Para que no se pinte todo de un color


		//Se dibujan las puertas
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.32f, 0.0f, 11.5f));
		model = glm::rotate(model, glm::radians(rotPuertaPrincipal), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		PuertaPrin.Draw(lightingShader); //Puerta de Entrada

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-6.2f, 0.0f, -5.5f));
		model = glm::rotate(model, glm::radians(rotPuertaC1), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		PuertaC1.Draw(lightingShader); //Puerta que da al comedor

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.8f, 0.0f, -10.177f));
		model = glm::rotate(model, glm::radians(rotPuertaC2), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		PuertaC2.Draw(lightingShader); //Puerta que da a la cocina


		//Se dibuja el conjunto de mesas y sillas que van al centro de la pizzeria
		
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		MesasYSillas.Draw(lightingShader);
		
		//Se dibuja una silla individual (se anima)
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.1f + movSilla, 0.0f, 2.7f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Silla.Draw(lightingShader);

		
		//Se dibuja el conjunto de mesas y sillones que van a los costados de la pizzeria
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		MesasYSillones.Draw(lightingShader);
		

		/*
		* Modelos individuales, no se usan en la version final ya que estan en los conjuntos de arriba
		* 
		model = glm::translate(model, glm::vec3(6.3f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Sillon.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		MesaChica.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.3f, 0.0f, 1.3f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		MesaLarga.Draw(lightingShader);
		*/


		//Parrilla Larga
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-1.6f, 0.0f, -13.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Parrilla.Draw(lightingShader);

		//Parrilla Chica
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.06f, 0.0f, -13.2f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		ParrillaChica.Draw(lightingShader);

		//Freidora
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.2f, 0.0f, -13.3f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Freidora.Draw(lightingShader);

		//Canasta de la Freidora
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.42f, 0.98f + movCanasta, -13.08f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Canasta.Draw(lightingShader);

		//Mesa de Preparacion
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(6.9f, 0.0f, -11.1f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		MesaPreparacion.Draw(lightingShader);

		//Mesa con Puertas 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.9f, 0.0f, -13.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		MesaConPuertas.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.0f, 0.168f, -12.52f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		PuertaMCP.Draw(lightingShader);

		//Mesa con Puertas 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.1f, 0.0f, -13.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		MesaConPuertas.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.2f, 0.168f, -12.52f));
		model = glm::rotate(model, glm::radians(rotMesa), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		PuertaMCP.Draw(lightingShader);

		//Horno 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.9f, 0.0f, -13.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		HornoStructure.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.9f, 0.0f, -13.1f + movHornoPizz));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		HornoInside.Draw(lightingShader);

		//Horno 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.1f, 0.0f, -13.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		HornoStructure.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.1f, 0.0f, -13.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		HornoInside.Draw(lightingShader);

		//Caja Registradora 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-1.0f, 1.31f, -5.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		CajaRegistradora.Draw(lightingShader);

		//Caja Registradora 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.0f, 1.31f, -5.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		CajaRegistradora.Draw(lightingShader);

		//Caja Registradora 3
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(3.0f, 1.31f, -5.9f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		CajaRegistradora.Draw(lightingShader);

		//Horno de Microondas 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.1f, 1.5f, -13.2f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		MicroondasHorno.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(4.1f, 1.5f, -13.2f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		MicroondasPuerta.Draw(lightingShader);

		//Horno de Microondas 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.0f, 1.5f, -13.2f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		MicroondasHorno.Draw(lightingShader);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.0f, 1.5f, -13.2f));
		model = glm::rotate(model, glm::radians(rotMicroondas), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		MicroondasPuerta.Draw(lightingShader);

		//Estructura de la Vitrina de Pizzas No. 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.2f, 1.31f, -5.9f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		VitStructure.Draw(lightingShader);

		//Pizzas de la Vitrina No. 1
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.2f, 1.31f, -5.9));
		model = glm::rotate(model, glm::radians(rotPizzas), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		VitPizzas.Draw(lightingShader);

		//Vidrio de la Vitrina No. 1
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-2.2f, 1.31f, -5.9f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.20);
		VitGlass.Draw(lightingShader);
		glDisable(GL_BLEND); //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0); //Para que no se pinte todo de un color

		//Estructura de la Vitrina de Pizzas No. 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.0f, 1.31f, -5.9f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		VitStructure.Draw(lightingShader);

		//Pizzas de la Vitrina No. 2
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.0f, 1.31f, -5.9));
		model = glm::rotate(model, glm::radians(rotPizzas), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		VitPizzas.Draw(lightingShader);

		//Vidrio de la Vitrina No. 2
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(7.0f, 1.31f, -5.9f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.20);
		VitGlass.Draw(lightingShader);
		glDisable(GL_BLEND); //Desactiva el canal alfa 
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0); //Para que no se pinte todo de un color

		glBindVertexArray(0);

		//Se dibuja el Aceite, tiene animacion por Shader
		Anim.Use();
		tiempo = glfwGetTime() * speed;
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(Anim.Program, "model");
		viewLoc = glGetUniformLocation(Anim.Program, "view");
		projLoc = glGetUniformLocation(Anim.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.2f, 0.0f, -13.3f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);
		Aceite.Draw(Anim);
		glBindVertexArray(0);


		/* DESCOMENTAR ESTE BLOQUE PARA VER LA UBICACION DE LAS POINTLIGHTS
		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < sizeof(pointLightPositions) / sizeof(pointLightPositions[0]); i++)
		{
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);
		*/

		// Draw skybox as last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		SkyBoxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(SkyBoxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// skybox cube
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS); // Set depth function back to default

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteBuffers(1, &skyboxVBO);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

}

void animacion()
{

	if (puertaAnim) {
		if (rotPuertaPrincipal <= 90.0f)
		{
			rotPuertaPrincipal += 0.5f;
		}
	}
	else
	{
		if (rotPuertaPrincipal > 0.5f)
		{
			rotPuertaPrincipal -= 0.5f;
		}
	}

	if (puertaC1Anim) {
		if (rotPuertaC1 <= 90.0f)
		{
			rotPuertaC1 += 0.5f;
		}
	}
	else
	{
		if (rotPuertaC1 > 0.5f)
		{
			rotPuertaC1 -= 0.5f;
		}
	}

	if (puertaC2Anim) {
		if (rotPuertaC2 <= 90.0f)
		{
			rotPuertaC2 += 0.5f;
		}
	}
	else
	{
		if (rotPuertaC2 > 0.5f)
		{
			rotPuertaC2 -= 0.5f;
		}
	}

	if (sillaAnim) {
		if (movSilla <= 0.8f)
		{
			movSilla += 0.05f;
		}
	}
	else
	{
		if (movSilla > 0.1f)
		{
			movSilla -= 0.05f;
		}
	}
	
	if (pizzasAnim) {
		if (rotPizzas >= 360.0f)
		{
			rotPizzas = 0.0f;
		}
		rotPizzas += 0.3f;
	}

	if (aceiteAnim)
	{
		speed = 25.0f;
	}
	else {
		speed = 0.0f;
	}

	if (canastaAnim) {
		if (movCanasta <= 0.5f)
		{
			movCanasta += 0.025f;
		}
	}
	else
	{
		if (movCanasta > 0.0f)
		{
			movCanasta -= 0.025f;
		}
	}

	if (mesaAnim) {
		if (rotMesa >= -90.0f)
		{
			rotMesa -= 0.5f;
		}
	}
	else
	{
		if (rotMesa < 0.5f)
		{
			rotMesa += 0.5f;
		}
	}

	if (microondasAnim) {
		if (rotMicroondas >= -90.0f)
		{
			rotMicroondas -= 0.5f;
		}
	}
	else
	{
		if (rotMicroondas < 0.5f)
		{
			rotMicroondas += 0.5f;
		}
	}

	if (hornoPizzAnim) {
		if (movHornoPizz <= 0.45f)
		{
			movHornoPizz += 0.05f;
		}
	}
	else
	{
		if (movHornoPizz > 0.1f)
		{
			movHornoPizz -= 0.05f;
		}
	}


}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_1]) //Abre o cierra la puerta principal al presionar 2
	{
		puertaAnim = !(puertaAnim);
	}

	if (keys[GLFW_KEY_2]) //Abre o cierra la puerta C1 al presionar 2
	{
		puertaC1Anim = !(puertaC1Anim);
	}
	if (keys[GLFW_KEY_3]) //Abre o cierra la puerta C2 al presionar 3
	{
		puertaC2Anim = !(puertaC2Anim);
	}

	if (keys[GLFW_KEY_4]) //Cambia la posicion de la silla al presionar 4
	{
		sillaAnim = !(sillaAnim);
	}

	if (keys[GLFW_KEY_5]) //Activa o desactiva la animacion de las pizzas C2 al presionar 5
	{
		pizzasAnim = !(pizzasAnim);
	}

	if (keys[GLFW_KEY_6]) //Activa o desactiva la animacion del aceite al presionar 6
	{
		aceiteAnim = !(aceiteAnim);
	}

	if (keys[GLFW_KEY_7])
	{
		canastaAnim = !(canastaAnim);
	}

	if (keys[GLFW_KEY_8])
	{
		mesaAnim = !(mesaAnim);
	}

	if (keys[GLFW_KEY_9])
	{
		microondasAnim = !(microondasAnim);
	}

	if (keys[GLFW_KEY_P])
	{
		hornoPizzAnim = !(hornoPizzAnim);
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <cmath>


const unsigned int WIDTH = 800;
const unsigned int HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
	"out vec4 FragColour;\n"
	"void main()\n"
	"{\n"
	"	FragColour = vec4(0.8f, 0.4f, 0.0f, 1.0f);\n"
	"}\0";

int main();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

class Wall
{
public:
	float width;
	float height;
	float x1, x2;
	float y1, y2;
};

class UnitVector
{
public:
	float x;
	float y;
	float normal;
};

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	GLFWwindow* WINDOW = glfwCreateWindow(WIDTH, HEIGHT, "TestWindow", NULL, NULL);
	if (WINDOW == NULL)
	{
		std::cout << "Window failed to initialize" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(WINDOW);
	glfwSetFramebufferSizeCallback(WINDOW, framebuffer_size_callback);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//MAKING VERTEX SHADER
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);			//SET SHADER SOURCE CODE
	glCompileShader(vertexShader);
	int success;							//CHECKING FOR COMPILE ERRORS
	char infolog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION::ERROR" << infolog << std::endl;
	}

	//MAKING FRAGMENT SHADER
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);		//SET SHADER SOURCE CODE
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);		//CHECKING FOR COMPILE ERRORS
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::ERROR" << infolog << std::endl;
	}

	//MAKING SHADER PROGRAM
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);		//ATTACHING SHADERS
	glAttachShader(shaderProgram, fragmentShader);		//ATTACHING SHADERS
	glLinkProgram(shaderProgram);						//LINKING SHADERS
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);	//CHECKING FOR LINK ERROR
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infolog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING::FAILED" << infolog << std::endl;
	}

	//DELETING SHADERS AFTER LINKING
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	Wall wall;
	wall.width = 10;
	wall.height = 2;
	wall.x1 = -5;
	wall.x2 = 5;
	wall.y1 = 4;
	wall.y2 = 4;

	//Take UnitVector and project it forward in steps until it hits a part of the wall, do this for every pixel in the camera's view.
	//SET THE LEFT AND RIGHT UNITVECTORS TO A 45DEG FOV
	UnitVector camera_left;
	camera_left.normal = sqrt(wall.x1 * wall.x1 + wall.y1 * wall.y1);
	camera_left.x = wall.x1/camera_left.normal;
	camera_left.y = wall.y1/camera_left.normal;
	camera_left.normal = 1.0f;
	UnitVector camera_right;
	camera_right.normal = sqrt(wall.x2 * wall.x2 + wall.y2 * wall.y2);
	camera_right.x = wall.x2/camera_right.normal;
	camera_right.y = wall.y2/camera_right.normal;
	camera_right.normal = 1.0f;

	//VERTICES OF WALL
	float vertices[] {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.5f,  0.5f, 0.0f,
		-0.5f,	0.5f, 0.0f
	};

	unsigned int indices[] {
		0,1,3,
		1,2,3
	};

	//CREATING VERTEX BUFFER OBJECT
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	double currentTime;
	double lastTime = glfwGetTime();
	double deltaT;
	while (!glfwWindowShouldClose(WINDOW))
	{
		currentTime = glfwGetTime();
		deltaT = currentTime - lastTime;
		std::cout << deltaT << std::endl;
		lastTime = currentTime;
		processInput(WINDOW);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		if (glfwGetKey(WINDOW, GLFW_KEY_A) == GLFW_PRESS) {
			vertices[0] -= deltaT * 1.0f;
			vertices[3] -= deltaT * 1.0f;
			vertices[6] -= deltaT * 1.0f;
			vertices[9] -= deltaT * 1.0f;
		}
		if (glfwGetKey(WINDOW, GLFW_KEY_D) == GLFW_PRESS) {
			vertices[0] += deltaT * 1.0f;
			vertices[3] += deltaT * 1.0f;
			vertices[6] += deltaT * 1.0f;
			vertices[9] += deltaT * 1.0f;
		}
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(WINDOW);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
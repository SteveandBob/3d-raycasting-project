#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <iostream>

int main();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

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

	//VERTICES OF SHAPE
	float vertices[]{
		-0.5f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
		 0.5f, -1.0f, 0.0f
	};

	//CREATING VERTEX BUFFER OBJECT
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(WINDOW))
	{
		processInput(WINDOW);
		
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwSwapBuffers(WINDOW);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
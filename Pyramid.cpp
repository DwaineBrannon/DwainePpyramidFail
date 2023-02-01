#include <GL/glew.h>
#include <GLFW/glfw3.h>

// GLM libraries
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>


	// Vertex shader source code
const char  *vertexShaderSource =
"#version 330 core\n"
"layout(location = 0) in vec4 vPosition;"
"layout(location = 1) in vec4 aColor;"
"out vec4 oColor;"
"uniform mat4 model;\n"
"uniform mat4 view;\n"
"uniform mat4 projection;\n"
"void main()\n"
"{\n"
"gl_Position = projection * view * model * vec4(vPosition, 1.0);\n"
"oColor = aColor;\n"
"}\n";

// Fragment shader source code
const char *fragmentShaderSource =
"#version 330 core\n"
"in vec4 oColor;"
"out vec4 fragColor;"
"void main()\n"
"{\n"
"fragColor = vec4(oColor, 1.0f);\n"
"}\n";

//camera variables
glm::vec3 cameraPos = glm::vec3(-1.0f, 0.0f, 3.0f);
glm::vec3 targetPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 upVector = glm::vec3(0.0f, 1.0f, 0.0f);


int width, height;
const float PI = 3.14159f;
const float toRadians = PI / 180.0f;

// Draw Primitive(s)
void draw()
{
	GLenum mode = GL_TRIANGLES;
	GLsizei indices = 16;
	glDrawElements(mode, indices, GL_UNSIGNED_BYTE, nullptr);


}

// Create and Compile Shaders
static GLuint CompileShader(const std::string& source, GLuint shaderType)
{
	// Create Shader object
	GLuint shaderID = glCreateShader(shaderType);
	const char* src = source.c_str();

	// Attach source code to Shader object
	glShaderSource(shaderID, 1, &src, nullptr);

	// Compile Shader
	glCompileShader(shaderID);

	// Return ID of Compiled shader
	return shaderID;

}

// Create Program Object
static GLuint CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	// Compile vertex shader
	GLuint vertexShaderComp = CompileShader(vertexShader, GL_VERTEX_SHADER);

	// Compile fragment shader
	GLuint fragmentShaderComp = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

	// Create program object
	GLuint shaderProgram = glCreateProgram();

	// Attach vertex and fragment shaders to program object
	glAttachShader(shaderProgram, vertexShaderComp);
	glAttachShader(shaderProgram, fragmentShaderComp);

	// Link shaders to create executable
	glLinkProgram(shaderProgram);

	// Delete compiled vertex and fragment shaders
	glDeleteShader(vertexShaderComp);
	glDeleteShader(fragmentShaderComp);

	// Return Shader Program
	return shaderProgram;

}


int main(void)
{	
	width = 640; height = 480;

	GLFWwindow* window;
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Make the window's context current */
	

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(width, height, "Dwaine's Pyramid", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	GLfloat vertices[] = {

			0.0f, 1.0f, 0.0f, // top
			1.0f, 0.0f, 0.0f, // Red

			-1.0f, -1.0f,1.0f, //front left
			 0.0f, 1.0f, 0.0f,

			1.0f, -1.0f, 1.0f,  // front-right vertex
			0.0f, 0.0f, 1.0f,

			1.0f, -1.0f, -1.0f,    // back-right vertex
			1.0f, 0.0f, 0.0f,

			-1.0f, -1.0f, -1.0f,    // back-left vertex
			 0.0f, 1.0f, 0.0f,

		};

		// Define element indices
		GLubyte indices[] = {
			 0, 1, 2,    // front triangle
			 0, 2, 3,    // right triangle
			 0, 3, 4,    // back triangle
			 0, 4, 1,    // left triangle
			 1, 2, 3, 4  // bottom quad
		};

	// Initialize GLEW
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			std::cout << "Error!" << std::endl;
			return -1;
		}

	// Enable depth test & accept fragment if it is closer to the camera than the former one
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	GLuint VBO, EBO, VAO;

	glGenBuffers(1, &VBO); // Create VBO
	glGenBuffers(1, &EBO); // Create EBO

	glGenVertexArrays(1, &VAO); // Create VOA
	glBindVertexArray(VAO);

	// VBO and EBO Placed in User-Defined VAO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // Select VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // Select EBO


	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Load vertex attributes
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // Load indices 

	// Specify attribute location and layout to GPU
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	

	// Color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	

	glBindVertexArray(0); // Unbind VOA or close off (Must call VOA explicitly in loop)
	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind VBO
	// Creating Shader Program
	GLuint shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

	// Use Shader Program exe once
	glUseProgram(shaderProgram);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		// Resize window and graphics simultaneously
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f); // Default pale blue from LearnOpenGL
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //enable depth 

		// Use Shader Program exe and select VAO before drawing 
		glUseProgram(shaderProgram); // Call Shader per-frame when updating attributes
		glBindVertexArray(VAO); // User-defined VAO must be called before draw. 

		// position values
		const float left = 0.0f;
		const float right = 800.0f;
		const float bottom = 0.0f;
		const float top = 600.0f;
		const float near = 0.1f;
		const float far = 100.0f;

		// Identity Matrix Declaration
		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::mat4 viewMatrix = glm::lookAt(cameraPos, targetPos, upVector);
		glm::mat4 projectionMatrix = glm::ortho(left, right, bottom, top, near, far);

		// Setting up Transformations
		modelMatrix = glm::scale(modelMatrix, glm::vec3(1.0f, 1.0f, 1.0f));
		modelMatrix = glm::rotate(modelMatrix, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 0.0f));


		//Set up transform that moves camera 
		viewMatrix = glm::translate(viewMatrix, glm::vec3(1.0f, 0.0f, -3.0f));
		viewMatrix = glm::rotate(viewMatrix, 45.0f * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		
		// specifying type fo projection
											// FOV, Width, Height near plane, far plane
		

		// Select uniform variable and shader
		GLuint modelLoc = glGetUniformLocation(shaderProgram, "model"); 
		GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
		GLuint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

		// Pass transform to shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
		
		
	


		// Draw primitive(s)
			draw();

		// Unbind Shader exe and VOA after drawing per frame
		glBindVertexArray(0); //Incase different VAO wii be used after
		glUseProgram(0); // Incase different shader will be used after

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	//Clear GPU resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);


	glfwTerminate();
	return 0;
}

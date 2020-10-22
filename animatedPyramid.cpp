/*
 * practiceActivity9_JonathanHandy.cpp
 *
 *  Created on: Oct 19, 2020
 *      Author: 1573734_snhu
 */

// Header inclusions
#include <iostream>
#include <GL/glew.h>
#include <GL/freeglut.h>

// GLM math header inclusions
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std; // Standard namespace

#define WINDOW_TITLE "Modern OpenGL" // Window title macro

// Shader program macro
#ifndef GLSL
#define GLSL(Version, Source) "#version " #Version "\n" #Source
#endif

// Variable declarations for shader, window size initialization, buffer and array objects
GLint shaderProgram, WindowWidth = 800, WindowHeight = 600;
GLuint VBO, VAO;

// Function prototypes
void UResizeWindow(int, int);
void URenderGraphics(void);
void UCreateShader(void);
void UCreateBuffers(void);

// Vertex shader source code
const GLchar * vertexShaderSource = GLSL(330,

	layout (location = 0) in vec3 position; // VAP position 0 for vertex position data
	layout (location = 1) in vec3 color; // VAP position 1 for color

	out vec3 mobileColor; // Transfer color data to the fragment shader

	// Uniform / global variables for the transform matrices
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main() {

		gl_Position = projection * view * model * vec4(position, 1.0f); // Transforms vertices into clip coordinates
		mobileColor = color; // References incoming color data
	}
);

// Fragment shader source code
const GLchar * fragmentShaderSource = GLSL(330,
	in vec3 mobileColor; // Holds incoming color data from vertex shader

	out vec4 gpuColor; // Variable to pass color data to GPU

	void main() {

		gpuColor = vec4(mobileColor, 1.0); // Send color data to GPU for rendering
	}
);

// Main program
int main(int argc, char* argv[]) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(WindowWidth, WindowHeight);
	glutCreateWindow(WINDOW_TITLE);

	glutReshapeFunc(UResizeWindow);

	glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK) {
			cout << "Failed to initialize GLEW" << endl;
			return -1;
		}

	UCreateShader();
	UCreateBuffers();

	glUseProgram(shaderProgram);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Sets background to black

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glutDisplayFunc(URenderGraphics);

	glutMainLoop();

	// Destroys buffer objects once used
	glDeleteVertexArrays(1, &VAO);
	glDeleteVertexArrays(1, &VBO);

	return 0;

}

// Resize window
void UResizeWindow(int w, int h) {
	WindowWidth = w;
	WindowHeight = h;
	glViewport(0, 0, WindowWidth, WindowHeight);
}

// Render graphics
void URenderGraphics(void) {

	glEnable(GL_DEPTH_TEST); // Enable z-depth
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clears the screen
	glBindVertexArray(VAO); // Activate the vertex array object before drawing

	// Transforms the object
	glm::mat4 model;
	model = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f)); // Place the object at the center of the viewport

	// Rotate cube every second by 0.0005f degrees in xyz
	model = glm::rotate(model, glutGet(GLUT_ELAPSED_TIME) * -0.0005f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f)); // Increase the object size by a scale of 2

	// Transforms the camera
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f)); // Moves the world -5 units in z

	// Creates a perspective projection
	glm::mat4 projection;
	projection = glm::perspective(45.0f, (GLfloat)WindowWidth / (GLfloat)WindowHeight, 0.1f, 100.0f);

	// Retrieves and passes transform matrices to shader program
	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glutPostRedisplay();

	// Draws the triangles
	glDrawArrays(GL_TRIANGLES, 0, 18);

	glBindVertexArray(0); // Deactivate the vertex array object
	glutSwapBuffers();

}

// Creates the shader program
void UCreateShader()
{

	//vertex shader
	GLint vertexShader = glCreateShader(GL_VERTEX_SHADER); //creates the vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //attaches the vertex shader to the source code
	glCompileShader(vertexShader); //compiles the vertex shader

	//fragment shader
	GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //creates the fragment shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //attaches the frag shader to the source code
	glCompileShader(fragmentShader); //compiles the fragment shader

	//shader program
	shaderProgram = glCreateProgram(); //creates the shader program and returns an id
	glAttachShader(shaderProgram, vertexShader); //attach vertex shader to the shader program
	glAttachShader(shaderProgram, fragmentShader); //attach frag shader to the source code
	glLinkProgram(shaderProgram); //link veretex and frag shaders to shader program

	//delete the vertex and fragment shaders once linked
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

}

// Creates the buffer and array objects
void UCreateBuffers() {

	// Position and texture coordinate data for 18 triangles
	GLfloat vertices[] = {
			// Position     	 	 	// Color
			// Side 1
			-0.5f, -0.5f, -0.5f,		1.0f, 0.0f, 0.0f,
			0.5f, -0.5f, -0.5f,			1.0f, 0.0f, 0.0f,
			0.0f,  0.5f,  0.0f,			1.0f, 0.0f, 0.0f,

			// Side 2
			0.0f,  0.5f,  0.0f,			0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,		0.0f, 1.0f, 0.0f,

			// Side 3
			-0.5f, -0.5f,  0.5f,		0.0f, 0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,			0.0f, 0.0f, 1.0f,
			0.0f,  0.5f,  0.0f,			0.0f, 0.0f, 1.0f,

			// Side 4
			0.0f,  0.5f,  0.0f,			1.0f, 0.0f, 1.0f,
			0.5f, -0.5f,  0.5f,			1.0f, 0.0f, 1.0f,
			0.5f, -0.5f, -0.5f,			1.0f, 0.0f, 1.0f,

			// Bottom
			0.5f, -0.5f, -0.5f,			1.0f, 1.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,		1.0f, 1.0f, 0.0f,
			0.5f, -0.5f, -0.5f,			1.0f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,		1.0f, 1.0f, 0.0f,
			0.5f, -0.5f,  0.5f,			1.0f, 1.0f, 0.0f,


	};


	// Generate buffer IDs
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);


	// Activate the vertex array object before binding and setting any VBOs and vertex attrib pointers
	glBindVertexArray(VAO);

	// Activate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Copy vertices to VBO


	// Set attribute pointer 0 to hold position data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // Enables vertex attribute

	// Set attribute pointer 1 to hold color data
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1); // Enables vertex attribute

	glBindVertexArray(0); // Deactivates the VAO which is good practice

}

































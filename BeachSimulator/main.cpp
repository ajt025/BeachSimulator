#include "main.h"

void errorCallback(int error, const char* description)
{
	// Print error.
	std::cerr << description << std::endl;
}

void setupCallbacks(GLFWwindow* window)
{
	// Set the error callback.
	glfwSetErrorCallback(errorCallback);
	// Set the key callback.
	glfwSetKeyCallback(window, Window::keyCallback);
	// Set the window resize callback.
	glfwSetWindowSizeCallback(window, Window::resizeCallback);
}

void setupOpenglSettings()
{
	// Enable depth buffering.
	glEnable(GL_DEPTH_TEST);
	// Related to shaders and z value comparisons for the depth buffer.
	glDepthFunc(GL_LEQUAL);
	// Set polygon drawing mode to fill front and back of each polygon.
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	// Set clear color to black.
	glClearColor(0.0, 0.0, 0.0, 0.0);
}

void printVersions()
{
	// Get info of GPU and supported OpenGL version.
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
		<< std::endl;

	//If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
	std::cout << "Supported GLSL version is: " <<
		glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

int main(void)
{
	// Create the GLFW window.
	GLFWwindow* window = Window::createWindow(640, 480);
	if (!window) exit(EXIT_FAILURE);

	// Print OpenGL and GLSL versions.
	printVersions();
	// Setup callbacks.
	setupCallbacks(window);
	// Setup OpenGL settings.
	setupOpenglSettings();
	// Initialize the shader program; exit if initialization fails.
	if (!Window::initializeProgram()) exit(EXIT_FAILURE);
	// Initialize objects/pointers for rendering; exit if initialization fails.
	if (!Window::initializeObjects()) exit(EXIT_FAILURE);

	// Loop while GLFW window should stay open.
	while (!glfwWindowShouldClose(window))
	{
		// Main render display callback. Rendering of objects is done here.
		Window::displayCallback(window);

		// Idle callback. Updating objects, etc. can be done here.
		Window::idleCallback();
	}

	Window::cleanUp();
	// Destroy the window.
	glfwDestroyWindow(window);
	// Terminate GLFW.
	glfwTerminate();

	exit(EXIT_SUCCESS);
}

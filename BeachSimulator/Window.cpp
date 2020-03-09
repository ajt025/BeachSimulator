#include "Window.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#endif

/* 
 * Declare your variables below. Unnamed namespace is used here to avoid 
 * declaring global or static variables.
 */
namespace
{
	int width, height;
	std::string windowTitle("Beach Simulator");

    // Cursor position
    glm::vec2 lastPos;
    GLboolean firstMouse = true;

	Object* currentObj; // The object currently displaying.
    Cube* cube; // Left in for camera testing

	glm::vec3 eye(0.0f, 0.0f, 0.0f); // Camera position.
	glm::vec3 center(0.0f, 0.0f, -1.0f); // The point we are looking at.
	glm::vec3 up(0.0f, 1.0f, 0.0f); // The up direction of the camera.
	float fovy = 60.0f;
	float near = 1.0f;
	float far = 1000.0f;
	glm::mat4 view = glm::lookAt(eye, center, up); // View matrix, defined by eye, center and up.
	glm::mat4 projection; // Projection matrix.

	GLuint program; // The shader program id.
    GLuint skyboxProgram; // Skybox shader program id

    // Normal Shader Locs
	GLuint projectionLoc; // Location of projection in shader.
	GLuint viewLoc; // Location of view in shader.
	GLuint modelLoc; // Location of model in shader.
	GLuint colorLoc; // Location of color in shader.

    // Skybox PV Locs
    GLuint projectionSkyLoc; // Location of projection in shader.
    GLuint viewSkyLoc; // Location of view in shader.

    // Skybox buffers
    GLuint skyboxVAO, skyboxVBO;

    // Skybox texture
    GLuint cubemapTexture;

// ***      WINDOW CONSTANTS     *** //

    // faceBoxes strings
    const std::vector<std::string> boxFaces = {
        "skybox/right.jpg",
        "skybox/left.jpg",
        "skybox/top.jpg",
        "skybox/bottom.jpg",
        "skybox/front.jpg",
        "skybox/back.jpg"
    };

    // Predefined skybox vertices
    const float skyboxVertices[] = {
        // positions
        -1000.0f,  1000.0f, -1000.0f,
        -1000.0f, -1000.0f, -1000.0f,
         1000.0f, -1000.0f, -1000.0f,
         1000.0f, -1000.0f, -1000.0f,
         1000.0f,  1000.0f, -1000.0f,
        -1000.0f,  1000.0f, -1000.0f,

        -1000.0f, -1000.0f,  1000.0f,
        -1000.0f, -1000.0f, -1000.0f,
        -1000.0f,  1000.0f, -1000.0f,
        -1000.0f,  1000.0f, -1000.0f,
        -1000.0f,  1000.0f,  1000.0f,
        -1000.0f, -1000.0f,  1000.0f,

         1000.0f, -1000.0f, -1000.0f,
         1000.0f, -1000.0f,  1000.0f,
         1000.0f,  1000.0f,  1000.0f,
         1000.0f,  1000.0f,  1000.0f,
         1000.0f,  1000.0f, -1000.0f,
         1000.0f, -1000.0f, -1000.0f,

        -1000.0f, -1000.0f,  1000.0f,
        -1000.0f,  1000.0f,  1000.0f,
         1000.0f,  1000.0f,  1000.0f,
         1000.0f,  1000.0f,  1000.0f,
         1000.0f, -1000.0f,  1000.0f,
        -1000.0f, -1000.0f,  1000.0f,

        -1000.0f,  1000.0f, -1000.0f,
         1000.0f,  1000.0f, -1000.0f,
         1000.0f,  1000.0f,  1000.0f,
         1000.0f,  1000.0f,  1000.0f,
        -1000.0f,  1000.0f,  1000.0f,
        -1000.0f,  1000.0f, -1000.0f,

        -1000.0f, -1000.0f, -1000.0f,
        -1000.0f, -1000.0f,  1000.0f,
         1000.0f, -1000.0f, -1000.0f,
         1000.0f, -1000.0f, -1000.0f,
        -1000.0f, -1000.0f,  1000.0f,
         1000.0f, -1000.0f,  1000.0f
    };

const GLfloat m_ROTSCALE = 0.002f;
const GLfloat m_MOVEMENTSCALE = 0.005f;
};

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
    
    // Create skybox shader program with a vertex shader and a fragment shader.
    skyboxProgram = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
    
	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}
    
    if (!skyboxProgram)
    {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	colorLoc = glGetUniformLocation(program, "color");

    glUseProgram(skyboxProgram);
    // Get the locations of uniform variables.
    projectionSkyLoc = glGetUniformLocation(skyboxProgram, "projectionSky");
    viewSkyLoc = glGetUniformLocation(skyboxProgram, "viewSky");
    
	return true;
}

bool Window::initializeObjects()
{
    cube = new Cube(1.0f);
    
    currentObj = cube;
    
    glUseProgram(skyboxProgram);
    
    // skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    // Skybox textures
    cubemapTexture = loadCubemap(boxFaces);

	return true;
}

void Window::cleanUp()
{
    delete cube;
    
	// Delete the shader program.
	glDeleteProgram(program);
    glDeleteProgram(skyboxProgram);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height);
#endif
	width = w;
	height = h;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	projection = glm::perspective(glm::radians(fovy),
		(float)width / (float)height, near, far);
}

void Window::idleCallback()
{}

void Window::displayCallback(GLFWwindow* window)
{
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(program);
    
    // Specify the values of the uniform variables we are going to use.
    glm::mat4 model = currentObj->getModel();
    glm::vec3 color = currentObj->getColor();
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    glUniform3fv(colorLoc, 1, glm::value_ptr(color));

    // Render the object.
    currentObj->draw();
    
    // Skybox Shader Drawing
    glDepthFunc(GL_LEQUAL);
    glUseProgram(skyboxProgram);

    glUniformMatrix4fv(projectionSkyLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewSkyLoc, 1, GL_FALSE, glm::value_ptr(view));

    glBindVertexArray(skyboxVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // no padding on bytes

    glBindVertexArray(0);
    glDepthFunc(GL_LESS);

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	 // Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		default:
			break;
		}
	}
}

void Window::cursor_position_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos) {
    if (firstMouse) {
        lastPos = glm::vec2(xpos, ypos);
        firstMouse = false;
    }
    
    glm::vec2 currPos = glm::vec2(xpos, ypos);
    // Get the delta of cursor pos's and create direction vector
    glm::vec2 direction = currPos - lastPos;
    
    glm::vec3 toRotateAround = glm::cross(center, up);
    glm::mat4 xRotator = glm::rotate(-direction.x * m_ROTSCALE, up);
    glm::mat4 yRotator = glm::rotate(-direction.y * m_ROTSCALE, toRotateAround);
    
    // Create two rotators conditionally
    // xyRotator created and applied for bounds checking on center
    // If outside y bounds, only apply the x rotator and update cameraview mat to be as so
    glm::mat4 xyRotator = xRotator * yRotator;
    
    glm::vec3 tempCenterXY = glm::mat3(xyRotator) * center;
    glm::vec3 tempCenterX = glm::mat3(xRotator) * center;
        
    if (tempCenterXY.y > -0.90f && tempCenterXY.y < 0.90f) {
        view = glm::lookAt(eye, eye + tempCenterXY, up); // rotate on X and Y in normal bounds
        center = tempCenterXY;
    } else {
        view = glm::lookAt(eye, eye + tempCenterX, up); // out of y bounds, only rotate on X
        center = tempCenterX; // update center
    }
    
    // Update last pos for cursor delta
    lastPos = currPos;
}

// Helpers

void Window::move(Direction direction) {
    switch (direction) {
    case FORWARD:
        eye += m_MOVEMENTSCALE * center;
        view = glm::lookAt(eye, eye + center, up);
        break;
    case BACKWARD:
        eye -= m_MOVEMENTSCALE * center;
        view = glm::lookAt(eye, eye + center, up);
        break;
    case LEFT:
        eye += glm::normalize(glm::cross(eye, up)) * m_MOVEMENTSCALE;
        view = glm::lookAt(eye, eye + center, up);
        break;
    case RIGHT:
        eye -= glm::normalize(glm::cross(eye, up)) * m_MOVEMENTSCALE;
        view = glm::lookAt(eye, eye + center, up);
        break;
    default:
        break;
    }
}

// Not using texture.cpp, OpenGL version of cubemap loading
GLuint Window::loadCubemap(std::vector<std::string> boxFaces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (GLuint i = 0; i < boxFaces.size(); i++)
    {
        unsigned char *data = stbi_load(boxFaces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << boxFaces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    
    return textureID;
}

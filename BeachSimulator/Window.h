#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <memory>

#include "Object.h"
#include "Cube.h"
#include "shader.h"
#include "PointCloud.h"

enum Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};



const GLfloat m_TERRAIN_SCALE = 32.0f;

class Window
{
public:

    struct Particle {
        glm::vec3 Position, Velocity;
        glm::vec4 Color;
        GLfloat Life;

        Particle()
            : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
    };

	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void cursor_position_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
    
    // helpers
    static void move(Direction direction);
    static void regenerateTerrain();
    static GLuint loadCubemap(std::vector<std::string> boxFaces);
    static GLuint FirstUnusedParticle();
    static void RespawnParticle(Particle& particle, glm::vec3 offset);
    static void drawParticles();
    static void drawOcean();
    static void drawPalmTree();

};

#endif

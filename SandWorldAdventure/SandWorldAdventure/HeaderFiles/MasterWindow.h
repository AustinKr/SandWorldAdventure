#pragma once
#include <string>

#include <GL/glew.h>
// If none of the inclusion macros are defined, the standard OpenGL GL / gl.h header(OpenGL / gl.h on macOS) is included when glfw3.h is
#include <GLFW/glfw3.h>

class MasterWindow
{
private:
	void InitializeWindow();
	// TODO: USe this function or delete it
	void InitializeShaders();

public:
	const char* p_WindowTitle = "Sand World Adventure GL";
	GLFWwindow* p_glfwWindow;

	// Initializes glfw and glew and creates a window.
	MasterWindow();
};
#include <iostream>
#include <glad/glad.h> // loads the OpenGL function pointers
#include <GLFW/glfw3.h> // handles window creation and input and openGL context creation

int main()
{
    std::cout << "DoomWeek engine starting...\n";
    /* --  initialize GLFW - must be the very first GLFW call,
     * if this fails, something is seriously wrong with the system -- */
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }
/* -- tell GLFW what kind of OpenGL context we want
 * 3.3 Core Profile - modern OpenGL, no legacy deprecated features
 * 4x MSAA - smooths out jagged edges on geometry, free quality boost -- */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    /* -- create the window -- 1920X1080 , windowed mode,
     * first nullptr = no fullscreen monitor,
     * second nullptr = no shared OpenGL context -- */

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "DOOMWEEK", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
// make this window's OpenGL context active on this thread, must be called before GLAD loads
    glfwMakeContextCurrent(window);
/* -- loads all the OpenGL function pointers through GLAD, after this every glFunction like glClear, glDrawArrays is ready -- */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
// game loop - runs until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // set background color
        glClearColor(0.1f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //show the frame we drew and check for input events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //clean up GLFW resources before exiting
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
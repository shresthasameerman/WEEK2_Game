#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "core/Shader.h"

// screen dimensions
const unsigned int SCR_WIDTH  = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH  / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// mouse callback - called by GLFW every time the mouse moves
void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;
    camera.processMouseMovement(xOffset, yOffset);
}

// scroll callback - called by GLFW every time the scroll wheel moves
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    camera.processMouseScroll(yOffset);
}
const char* VERT_PATH = "assets/shaders/world.vert";
const char* FRAG_PATH = "assets/shaders/world.frag";

int main()
{

    std::cout << "DoomWeek engine starting...\n";

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "DOOMWEEK", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // capture mouse, register callbacks
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetScrollCallback(window, scrollCallback);

    Shader worldShader(VERT_PATH, FRAG_PATH);

    //a simple coloured cube - 6 faces,2 triangles per face, 3 vertices per triangle = 36 vertices
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.2f, // back face
         0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
         0.5f,  0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
         0.5f,  0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
        -0.5f,  0.5f, -0.5f, 0.8f, 0.2f, 0.2f,
        -0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.2f,

        -0.5f, -0.5f, 0.5f, 0.2f, 0.8f, 0.2f, // front face
         0.5f, -0.5f, 0.5f, 0.2f, 0.8f, 0.2f,
         0.5f,  0.5f, 0.5f, 0.2f, 0.8f, 0.2f,
         0.5f,  0.5f, 0.5f, 0.2f, 0.8f, 0.2f,
        -0.5f,  0.5f, 0.5f, 0.2f, 0.8f, 0.2f,
        -0.5f, -0.5f, 0.5f, 0.2f, 0.8f, 0.2f,


        -0.5f,  0.5f,  0.5f, 0.2f, 0.2f, 0.8f, // left face
        -0.5f,  0.5f, -0.5f, 0.2f, 0.2f, 0.8f,
        -0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.8f,
        -0.5f, -0.5f, -0.5f, 0.2f, 0.2f, 0.8f,
        -0.5f, -0.5f,  0.5f, 0.2f, 0.2f, 0.8f,
        -0.5f,  0.5f,  0.5f, 0.2f, 0.2f, 0.8f,

        0.5f, 0.5f,  0.5f, 0.8f, 0.8f, 0.2f, // right face
        0.5f, 0.5f, -0.5f, 0.8f, 0.8f, 0.2f,
        0.5f, -0.5f, -0.5f, 0.8f, 0.8f, 0.2f,
        0.5f, -0.5f, -0.5f, 0.8f, 0.8f, 0.2f,
        0.5f, -0.5f,  0.5f, 0.8f, 0.8f, 0.2f,
        0.5f,  0.5f,  0.5f, 0.8f, 0.8f, 0.2f,

        -0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.8f, // bottom face
         0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.8f,
         0.5f, -0.5f,  0.5f, 0.8f, 0.2f, 0.8f,
         0.5f, -0.5f,  0.5f, 0.8f, 0.2f, 0.8f,
        -0.5f, -0.5f,  0.5f, 0.8f, 0.2f, 0.8f,
        -0.5f, -0.5f, -0.5f, 0.8f, 0.2f, 0.8f,

        0.5f, 0.5f, -0.5f, 0.2f, 0.8f, 0.8f, // top face
        -0.5f, 0.5f, -0.5f, 0.2f, 0.8f, 0.8f,
        -0.5f, 0.5f,  0.5f, 0.2f, 0.8f, 0.8f,
        -0.5f, 0.5f,  0.5f, 0.2f, 0.8f, 0.8f,
         0.5f, 0.5f,  0.5f, 0.2f, 0.8f, 0.8f,
         0.5f, 0.5f, -0.5f, 0.2f, 0.8f, 0.8f,
    };
    //create vertex array object and vertex buffer object
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //bind the VAO and VBO, upload the vertex data, and set the vertex attribute pointers
    glBindVertexArray(VAO);

    //upload vertex data to the GPU
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //tell OpenGL how to read the position (location = 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //tell OpenGL how to read the color (location = 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    // game loop
    while (!glfwWindowShouldClose(window)) {
        // delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // movement input
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera.processKeyboard(Camera_Movement::FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera.processKeyboard(Camera_Movement::BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.processKeyboard(Camera_Movement::LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.processKeyboard(Camera_Movement::RIGHT, deltaTime);

        glClearColor(0.1f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //activate shader
        worldShader.use();

        //build matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view  = camera.getViewMatrix();
        glm::mat4 projection = glm::perspective(
            glm::radians(camera.fov),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f,
            100.0f
        );
        //set matrices to shader
        worldShader.setMat4("uModel", model);
        worldShader.setMat4("uView", view);
        worldShader.setMat4("uProjection", projection);

        //render the cube
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
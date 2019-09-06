#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>

#include "shader.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_HEIGHT = 800;
const unsigned int SCR_WIDTH = 600;
unsigned int POLYGON_MODE = GL_FILL;

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    //*MacOS specific to force CORE profile only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW window creation
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return -1;
    }

    // Create our triangle vertexes mapped in -1, 1 space in device coordinates
    float vertices[] = {
      -0.5f, -0.5f, 0.0f,
      0.5f, -0.5f, 0.0f,
      -0.5f,  0.5f, 0.0f,
      0.5f,  -0.5f, 0.0f,
      0.5f, 0.5f, 0.0f,
      -0.5f, 0.5f, 0.0f
    };

    float colors[] = {
      1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f,
      0.0f,  1.0f, 0.0f
    };


    //Create, Bind and Write data to our points data buffer
    unsigned int points_VBO;
    glGenBuffers(1, &points_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(0, GL_ARRAY_BUFFER);//unbind

    //Create, Bind and Write data to our colors data buffer
    unsigned int colors_VBO;
    glGenBuffers(1, &colors_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glBindBuffer(0, GL_ARRAY_BUFFER);//unbind

    //Create our VAO object bind to it and setup object configuration for drawing
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //setup points_VBO to index 0 in our shader
    glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //setup colors_VBO to index 1 in our shader
    glBindBuffer(GL_ARRAY_BUFFER, colors_VBO);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //Enable previously created shader attributes (stored in newer versions of OpenGL)
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Unbind
    glBindVertexArray(0);


    Shader redShaderProgram("/home/tallys/git/learnopengl/src/shaders/points.vert", "/home/tallys/git/learnopengl/src/shaders/dynamic_color.frag");

    Shader orangeShaderProgram("/home/tallys/git/learnopengl/src/shaders/points.vert", "/home/tallys/git/learnopengl/src/shaders/static_color.frag");


    float timeValue = 0;

    // #######################################################################
    // RENDER LOOP
    // #######################################################################
    float alpha = 1.0;
    while(!glfwWindowShouldClose(window))
    {
      timeValue = glfwGetTime();
      timeValue = sin(timeValue);

      //clear openGL buffer (can be COLOR, STENCIL and DEPTH) filling them with the given
      // glClearColor
      glClearColor(0.2f, 0.3f, 0.9f, alpha);
      glClear(GL_COLOR_BUFFER_BIT);

      //input
      processInput(window);

      orangeShaderProgram.use();
      glUniform1f(0, timeValue);
      glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

      // Draw elements from Element Buffer Object (use indices to avoid duplicated data)
      //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glDrawArrays(GL_TRIANGLES, 3, 6);
      redShaderProgram.use();
      glUniform1f(0, timeValue);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      // glBindVertexArray(0); // no need to unbind it every time

      // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    // Delete all GLFW resources
    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window) {

  if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
    std::cout << "ESC key pressed, exiting... bye bye!" << std::endl;
  }

  if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
     glPolygonMode(GL_FRONT_AND_BACK, POLYGON_MODE);
     if(POLYGON_MODE == GL_FILL) {
       POLYGON_MODE = GL_LINE;
     } else {
       POLYGON_MODE = GL_FILL;
     }
  }
}

// Resize glViewport each time the user resize the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

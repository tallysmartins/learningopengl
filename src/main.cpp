#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_HEIGHT = 800;
const unsigned int SCR_WIDTH = 600;

const char* vertexShaderSource = "vec3 aPos; void main() { gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0); }";

int main()
{
    //Initialize the GLFW library before any glfw call, returns GL_TRUE or GL_FALSE
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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
      0.0f,  0.5f, 0.0f
    };  

    // Allocate a buffer in the GPU and save its id in VBO
    unsigned int VBO;
    glGenBuffers(1, &VBO);

    // Create vertexShader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    // Set and compile vertexShader defined in string vertexShaderSource
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check if shader compiled successfully
    int  success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    // Output log
    if(!success)
    {
      glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    // render loop
    // ------------
    float alpha = 1.0;
    while(!glfwWindowShouldClose(window))
    {
      //clear openGL buffer (can be COLOR, STENCIL and DEPTH) filling them with the given
      // glClearColor
      glClearColor(0.2f, 0.3f, 0.3f, alpha);
      glClear(GL_COLOR_BUFFER_BIT);

      // See this buffer as an Vertex Array
      glBindBuffer(GL_ARRAY_BUFFER, VBO);

      // Write our vertices to the buffer
      glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

      glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices));
      glBindBuffer(GL_ARRAY_BUFFER, 0);


      //input
      processInput(window);

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
}

// Resize glViewport each time the user resize the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

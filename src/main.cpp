#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cmath>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_HEIGHT = 800;
const unsigned int SCR_WIDTH = 600;
unsigned int POLYGON_MODE = GL_FILL;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "out vec3 colour;"
    "uniform float sinVal = 1.0;"
    "void main()\n"
    "{\n"
    "   colour = aColor*sinVal;"
    "   gl_Position = vec4(aPos.x + (0.4 * sinVal), aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *redFragmentShaderSource = "#version 330 core\n"
    "in vec3 colour;"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(colour, 1.0f);\n"
    "}\n\0";

const char *orangeFragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

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

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;


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

    // Try to insert this later
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Unbind
    glBindVertexArray(0);

    // ########################################################
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

    // ########################################################
    // Create fragmentShader
    unsigned int redFragmentShader, orangeFragmentShader;
    redFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    orangeFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // upload code to compile
    glShaderSource(redFragmentShader, 1, &redFragmentShaderSource, NULL);
    glCompileShader(redFragmentShader);

    glGetShaderiv(redFragmentShader, GL_COMPILE_STATUS, &success);
    // Output log
    if(!success)
    {
      glGetShaderInfoLog(redFragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // upload code to compile
    glShaderSource(orangeFragmentShader, 1, &orangeFragmentShaderSource, NULL);
    glCompileShader(orangeFragmentShader);

    glGetShaderiv(orangeFragmentShader, GL_COMPILE_STATUS, &success);
    // Output log
    if(!success)
    {
      glGetShaderInfoLog(orangeFragmentShader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // ########################################################
    // Create a shader Program and attach all shaders that we have in a single program object
    unsigned int redShaderProgram, orangeShaderProgram;
    redShaderProgram = glCreateProgram();
    orangeShaderProgram = glCreateProgram();

    glAttachShader(redShaderProgram, vertexShader);
    glAttachShader(redShaderProgram, redFragmentShader);
    glLinkProgram(redShaderProgram);


    // Check for errors when linking shaders in the shaderProgram
    glGetProgramiv(redShaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
      glGetProgramInfoLog(redShaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glAttachShader(orangeShaderProgram, vertexShader);
    glAttachShader(orangeShaderProgram, orangeFragmentShader);
    glLinkProgram(orangeShaderProgram);

    // Check for errors when linking shaders in the shaderProgram
    glGetProgramiv(orangeShaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
      glGetProgramInfoLog(orangeShaderProgram, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(redFragmentShader);
    glDeleteShader(orangeFragmentShader);



    float timeValue = 0;
    float t = 0;
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

      int sinValLocation = glGetUniformLocation(orangeShaderProgram, "sinVal");
      glUseProgram(orangeShaderProgram);
      glUniform1f(sinValLocation, timeValue);
      glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

      // Draw elements from Element Buffer Object (use indices to avoid duplicated data)
      //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      glDrawArrays(GL_TRIANGLES, 3, 6);
      glUseProgram(redShaderProgram);
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

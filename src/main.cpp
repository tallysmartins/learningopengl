#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shapefil.h>

#include <iostream>
#include <cmath>
#include <vector>

#include "shader.hpp"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_HEIGHT = 512;
const unsigned int SCR_WIDTH = 512;
unsigned int POLYGON_MODE = GL_FILL;

GLenum glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
            case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
            case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
            case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

std::vector<int> shapeCounts;

std::vector<float> loadMap() {

  std::vector<float> points;

  int nEntities, pnShapeType;
  double padfMinBound[4], padfMaxBound[4];
  SHPHandle myHandler = SHPOpen("/home/tallys/git/od-analysis/datasets/od1987/raw/Mapas/Shape/Zonas1987_region", "rb");
	if(myHandler == NULL) {
		std::cout << __FILE__ << ":" << __LINE__ <<" [E]:Nao foi possivel abrir shapefile. Abortando execução!\n" ;
		exit(-1);
	}

  SHPGetInfo(myHandler, &nEntities, &pnShapeType, padfMinBound, padfMaxBound);
  shapeCounts.assign(nEntities, 0);
  std::cout << nEntities << std::endl;
  std::cout << pnShapeType << std::endl;

  float xMin = padfMinBound[0];
  float yMin = padfMinBound[1];
  float xMax = padfMaxBound[0];
  float yMax = padfMaxBound[1];
  printf("xMin: %f, yMin: %f\n", xMin, yMin);
  printf("xMax: %f, yMax: %f\n", xMax, yMax);
  std::cout << "READING SHAPEFILE"  << std::endl;

  for(int T=0; T<nEntities; T++){
  //for(int T=0; T<2; T++){
    SHPObject *obj = SHPReadObject(myHandler, T);
    shapeCounts[T] = obj->nVertices;

    std::cout << "Reading Shape " << T << std::endl;
    std::cout << "nSHPType " << obj->nSHPType << std::endl;
    std::cout << "nShapeId " << obj->nShapeId << std::endl;
    std::cout << "nParts " << obj->nParts << std::endl;
    std::cout << "*panPartStart " << *obj->panPartStart << std::endl;
    std::cout << "nVertices " << obj->nVertices << std::endl;
    float x, y, z = 0;
    float rangeX = xMax - xMin;
    float rangeY = yMax - yMin;
    float border = 0.0;
    float size = 2;
    float scale=1.0;

    // Get the scale based on max range X or Y axes
    // If 
    if (rangeX>rangeY)
      scale  = (1-border)*size/rangeX;
    else
      scale  = (1-border)*size/rangeY;

    float xTranslation = -scale/2;//Our points fall into 0,1 quadrant, we need to 
    float yTranslation = -scale/2;//make a translation in both axes to center in -1,1

    for(int i=0; i < obj->nVertices; i++) {
      // Vertex points to be draw are made of 3 float elements (X, Y, Z)

      //Calculate normalized vertexes for axes X and Y
			// Normalized points fall in range [0,1]
      x = (obj->padfX[i] - xMin)/(xMax - xMin); 
      y = (obj->padfY[i] - yMin)/(yMax - yMin);
      fprintf(stderr, "%f, %f,\n", x, y);

			//Scale points and make a translation to center points in between [-1,1]
      x = x*scale+xTranslation;
      y = y*scale+yTranslation;

      //Add X, Y, Z coordinates to points data array
      points.push_back(x);
      points.push_back(y);
      points.push_back(z); // z = 0;


    }
  }

  return points;
}

int main()
{
    std::vector<float> points = loadMap();
    int N = points.size();

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    //*MacOS specific to force CORE profile only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // GLFW window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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

    //Create, Bind and Write data to our points data buffer
    unsigned int points_VBO;
    glGenBuffers(1, &points_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
    glBufferData(GL_ARRAY_BUFFER, N*sizeof(float), points.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);//unbind

    //Create our VAO object bind to it and setup object configuration for drawing
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //setup points_VBO to index 0 in our shader
    glBindBuffer(GL_ARRAY_BUFFER, points_VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //Enable previously created shader attributes (stored in newer versions of OpenGL)
    glEnableVertexAttribArray(0);

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
      glCheckError();

      //clear openGL buffer (can be COLOR, STENCIL and DEPTH) filling them with the given
      // glClearColor
      glClearColor(0.0f, 0.0f, 0.1f, alpha);
      glClear(GL_COLOR_BUFFER_BIT);

      //input
      processInput(window);

      orangeShaderProgram.use();
      int loc = glGetUniformLocation(orangeShaderProgram.ID, "c");
      std::cout << "loc = " << loc << std::endl;
      orangeShaderProgram.setFloat("c", 1);
      glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

      // Draw elements from Element Buffer Object (use indices to avoid duplicated data)
      //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

      int last = 0;
      for(int j=0; j<shapeCounts.size(); j++) {
        glDrawArrays(GL_TRIANGLE_FAN, last, shapeCounts[j]);
        last += shapeCounts[j];
      }
      //glDrawArrays(GL_TRIANGLE_FAN, 0, N/3);

      last = 0;
      orangeShaderProgram.setFloat("c", 0);
      glLineWidth(1.2);
      for(int j=0; j<shapeCounts.size(); j++) {
        glDrawArrays(GL_LINE_LOOP, last, shapeCounts[j]);
        last += shapeCounts[j];
      }

      glCheckError();
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

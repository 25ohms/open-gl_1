#include <GL/glew.h> // THIS MUST COME FIRST
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


int main(void)
{
  glewExperimental = GL_TRUE;
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
      return -1;

  /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);*/
  /*glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);*/
  /*glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(840, 480, "ohms", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK) {
    std::cout << "Error!" << std::endl; 
  } else {
    std::cout << "GLEW works!" << std::endl;
  }

  // checking versions
  std::cout << glGetString(GL_VERSION) << std::endl;
  std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
  
  /* checking to see if extensions are available*/
  GLCall(std::string ext = reinterpret_cast<const char*>(glGetString(GL_EXTENSIONS)));
  /*std::cout << ext << std::endl;*/

  /* Defining a buffer */

  {
    float positions[] = {
       -0.5f, -0.5f,  // 0
        0.5f, -0.5f,  // 1
        0.5f,  0.5f,  // 2
       -0.5f,  0.5f   // 3
    };

    unsigned int indices[] = {
      0, 1, 2,
      2, 3, 0
    };

    /*unsigned int vao;*/
    /*GLCall(glGenVertexArraysAPPLE(1, &vao));*/
    /*GLCall(glBindVertexArrayAPPLE(vao));*/

    /* tentative */
    VertexArray va;
    VertexBuffer vb(positions, 4 * 2 * sizeof(float)); // NOT TENTATIVE

    VertexBufferLayout layout;
    layout.push<float>(2);
    va.addBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    Shader shader("res/shaders/Basic.shader");
    shader.bind();
    shader.setUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

    /* assume we unbind everything */
    va.unBind();
    vb.unBind();
    ib.unBind();
    shader.unBind();
    
    float r = 0.0f;
    float increment = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      GLCall(glClear(GL_COLOR_BUFFER_BIT));

      /* Rebind everything */
      shader.bind();
      shader.setUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

      va.bind();
      ib.bind();
      /* Once everything is rebound, we can draw */

      GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

      if (r > 1.0f)
        increment = -0.05f;
      else if (r < 0.0f)
        increment = 0.05f;

      r += increment;

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }
  }
  // delete buffers
  glfwTerminate();
  return 0;
}

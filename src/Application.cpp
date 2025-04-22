#include <GL/glew.h> // THIS MUST COME FIRST
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <sstream>


struct shaderProgramSource
{
    std::string vertexSource;
    std::string fragmentSource;
};


static shaderProgramSource parseShader(const std::string& filePath)
{
    std::ifstream stream(filePath);

    enum class shaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    shaderType type = shaderType::NONE;
    while(getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = shaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = shaderType::FRAGMENT;
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}


static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    
    unsigned int id = glCreateShader(type);

    /* source must exist outside of the scope of this method*/
    const char* src = source.c_str(); // pointer to the beginning of our data
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); 
    if (result == GL_FALSE)
    {
      int length;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
      char* message = (char*)alloca(length * sizeof(char)); // stack allocated
      glGetShaderInfoLog(id, length, &length, message);
      std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<  std::endl;
      std::cout << message << std::endl;
      glDeleteShader(id);
      return 0;
    }


    return id;
}

/* method defined as int so that we can create a UID*/
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) // to avoid linking with other cpp files
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);


    /* shaders are attached to the program*/
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}



int main(void)
{
    glewExperimental = GL_TRUE;
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
      std::cout << "Error!" << std::endl; 
    } else {
      std::cout << "GLEW works!" << std::endl;
    }

    // checking versions
    std::cout << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    /* Defining a buffer */

    float positions[6] = {
       -0.5f, -0.5f,
        0.0f,  0.5f,
        0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer); // we need to bind the buffer before specifying anything else
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); // specifies to C++ the structure of vertices

    shaderProgramSource source = parseShader("res/shaders/Basic.shader");

    /*std::cout << "VERTEX" << std::endl;*/
    /*std::cout << source.vertexSource << std::endl;*/
    /*std::cout << "FRAGMENT" << std::endl;*/
    /*std::cout << source.fragmentSource << std::endl;*/

    unsigned int shader = CreateShader(source.vertexSource, source.fragmentSource);
    glUseProgram(shader); // 3. Lookup and enable attribute
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}

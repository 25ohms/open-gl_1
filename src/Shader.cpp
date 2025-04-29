#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string &filepath) : m_FilePath(filepath), m_RendererID(0) { 
  shaderProgramSource source = parseShader(filepath);
  m_RendererID = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader(){ 
  GLCall(glDeleteProgram(m_RendererID));
}

void Shader::bind() const{ 
  GLCall(glUseProgram(m_RendererID));
}

void Shader::unBind() const{ 
  GLCall(glUseProgram(0));
}

// Set uniforms
void Shader::setUniform4f(const std::string &name, float v0, float v1, float v2, float v3){ 
  GLCall(glUniform4f(getUniformLocation(name), v0, v1, v2, v3));
}

int Shader::getUniformLocation(const std::string &name){
  if(m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) // optimizing via caching
    return m_UniformLocationCache[name];

  GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
  if (location == -1)
    std::cout << "Warning: Uniform " << name << " doesn't exist" << std::endl;
  m_UniformLocationCache[name] = location;
  return location;
}

shaderProgramSource Shader::parseShader(const std::string& filePath) {
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


unsigned int Shader::compileShader(unsigned int type, const std::string& source) {    
  unsigned int id = glCreateShader(type);

  /* source must exist outside of the scope of this method*/
  const char* src = source.c_str(); // pointer to the beginning of our data
  GLCall(glShaderSource(id, 1, &src, nullptr));
  GLCall(glCompileShader(id));

  int result;
  GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE)
  {
    int length;
    GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    char* message = (char*)alloca(length * sizeof(char)); // stack allocated
    GLCall(glGetShaderInfoLog(id, length, &length, message));
    std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") <<  std::endl;
    std::cout << message << std::endl;
    GLCall(glDeleteShader(id));
    return 0;
  }

  return id;
}

/* method defined as int so that we can create a UID*/
unsigned int Shader::createShader(const std::string& vertexShader, const std::string& fragmentShader) {
  GLCall(unsigned int program = glCreateProgram());
  unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

  GLCall(glAttachShader(program, vs));
  GLCall(glAttachShader(program, fs));
  GLCall(glLinkProgram(program));
  GLCall(glValidateProgram(program));


  /* shaders are attached to the program*/
  GLCall(glDeleteShader(vs));
  GLCall(glDeleteShader(fs));

  return program;
}

#pragma once
#include <string>
#include <unordered_map>

struct shaderProgramSource
{
  std::string vertexSource;
  std::string fragmentSource;
};

class Shader {
  private:
    std::string m_FilePath; // debugging purposes
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;

    shaderProgramSource parseShader(const std::string& filePath);
    unsigned int compileShader(unsigned int type, const std::string& source);
    unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);

    // caching for uniforms
    int getUniformLocation(const std::string &name);
  public:
    Shader(const std::string &filepath);
    ~Shader();

    void bind() const;
    void unBind() const;

    // Set uniforms
    void setUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
};

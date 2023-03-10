#pragma once
#include "../Object.h"
#include "Shader.h"

#include <glm/glm.hpp>

class ShaderProgram : public Object
{
public:
    using Location = GLint;

public:
    ShaderProgram();
    virtual ~ShaderProgram();

    bool Build(const Shader& vertexShader, const Shader& fragmentShader);
    void Use() const;
    bool IsLinked() const;

    void Bind() const override;
    Location GetUniformLocation(const char* name);

public:
    void SetUnifrom(const char* name, int value);
    void SetUnifrom(const char* name, bool value);
    void SetUnifrom(const char* name, float value);
    void SetUnifrom(const char* name, glm::vec2 vector);
    void SetUnifrom(const char* name, glm::vec3 vector);
    void SetUnifrom(const char* name, glm::vec4 vector);
    void SetUnifrom(const char* name, glm::mat4 matrix);
    void SetUnifrom(ShaderProgram::Location location, glm::vec3 vector);
    void SetUnifrom(ShaderProgram::Location location, int value);
    void SetUnifrom(ShaderProgram::Location location, bool value);
    void SetUnifrom(ShaderProgram::Location location, float value);

private:
    void AttachShader(const Shader& shader);
    bool Link();
   
    

private:
#ifndef NDEBUG
    inline bool IsUsed() const { return s_usedHandle == GetHandle(); }
    static Handle s_usedHandle;
#endif


};


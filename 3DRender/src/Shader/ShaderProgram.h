#pragma once
#include "../Object.h"
#include "Shader.h"

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

private:
    void AttachShader(const Shader& shader);
    bool Link();
   
    

private:
#ifndef NDEBUG
    inline bool IsUsed() const { return s_usedHandle == GetHandle(); }
    static Handle s_usedHandle;
#endif


};


#pragma once

#include <glm/fwd.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include "wrappers/glWrapper.hh"
#include "wrappers/glmWrapper.hh"

class program {
public:
    struct ShaderSource {
        std::string src;
        GLenum type;
        std::string name;
    };
public:
    program();
    ~program();

    static program* make_program(const std::vector<ShaderSource>& shaders);
    static program* make_program_path(const std::vector<ShaderSource>& shaders_path);

    const char* getlog() const { return log_.c_str(); };
    bool isready() const { return ready_; }
    void use() const;

    GLuint get_program() const { return program_; }

    void setUniformMat4(const char *name, const glm::mat4& val,
                        bool throwIfMissing=false) const;

    void setUniformVec3(const char *name, const glm::vec3 &val,
                        bool throwIfMissing=false) const;

    void setUniformVec4(const char *name, const glm::vec4 &val,
                        bool throwIfMissing=false) const;

    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

protected:
    program(GLuint program, const std::string& log="", bool ready=false)
        : program_(program), log_(log), ready_(ready)
    {}

private:
    GLuint program_;

    std::string log_;
    bool ready_;
};

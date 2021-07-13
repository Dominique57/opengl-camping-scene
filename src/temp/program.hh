#pragma once

#include <glm/fwd.hpp>
#include <string>
#include <vector>
#include <stdexcept>
#include "wrappers/glWrapper.hh"
#include "wrappers/glmWrapper.hh"

class program {
public:
    program();
    ~program();
    static program* make_program(
            const std::string& vertex_shader_src,
            const std::string& fragment_shader_src);
    static program* make_program_path(
            const std::string& vertex_path,
            const std::string& fragment_path);

    const char* getlog() const { return log_.c_str(); };
    bool isready() const { return ready_; }
    void use() const;

    GLuint get_program() const { return program_; }

    void setUniformMat4(const std::string& name, const glm::mat4& val,
            bool throwIfMissing=false) const;

protected:
    program(GLuint program, std::string log, bool ready)
        : program_(program), log_(log), ready_(ready)
    {}

private:
    GLuint program_;

    std::string log_;
    bool ready_ = false;
};

#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <iomanip>

inline std::ostream& operator<<(std::ostream &out, const glm::mat4 &mat) {
    for (auto i = 0U; i < 4; ++i) {
        for (auto j = 0U; j < 4; ++j) {
            out << std::setw(3) << mat[i][j];
        }
        out << '\n';
    }
    return out;
}

inline std::ostream& operator<<(std::ostream &out, const glm::vec3 &mat) {
    for (auto i = 0U; i < 3; ++i) {
        out << std::setw(3) << mat[i];
    }
    return out;
}

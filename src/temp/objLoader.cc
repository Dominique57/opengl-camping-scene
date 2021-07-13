#include "objLoader.hh"
#include <array>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>

static inline void throwError(const char *message, unsigned lineNumber,
        unsigned char* lineCode=nullptr) {
    auto errMsg = std::string("Line n") + std::to_string(lineNumber) + " ";
    if (lineCode)
        ((errMsg += "Line code `") += *lineCode) += "` ";

    throw std::invalid_argument(errMsg + message);
}

static void parseLine(const std::string &line, ObjLoader::ObjData &res,
        unsigned lineNumber) {
    std::stringstream lineStream(line);
    unsigned char lineCode;
    lineStream >> lineCode;

    if (lineCode == 'v') {
        std::array<GLfloat, 3> vertices;
        lineStream >> vertices[0] >> vertices[1] >> vertices[2];
        res.vertices.insert(res.vertices.end(), vertices.begin(),
                vertices.end());
    } else if (lineCode == 'f') {
        std::array<unsigned, 3> faces;
        lineStream >> faces[0] >> faces[1] >> faces[2];
        res.faces.insert(res.faces.end(), faces.begin(),
                faces.end());
    } else if (lineCode == '#') {
        return;
    } else {
        throwError("Unrecognized line code", lineNumber, &lineCode);
    }

    if (lineStream.fail())
        throwError("Error while handling line", lineNumber, &lineCode);
    if (!lineStream.eof())
        throwError("Line did not end correctly", lineNumber, &lineCode);
}

ObjLoader::ObjData ObjLoader::loadObj(const std::string &path) {
    auto res = ObjData{};

    std::ifstream file(path);
    std::string line;
    unsigned lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber += 1;
        if (line.empty())
            continue;

        parseLine(line, res, lineNumber);
    }

    return res;
}

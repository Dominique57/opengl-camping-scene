#include "objLoader.hh"
#include <array>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>

static inline void throwError(const char *message, unsigned lineNumber,
        const char* lineCode=nullptr) {
    auto errMsg = std::string("Line n") + std::to_string(lineNumber) + " ";
    if (lineCode)
        errMsg += (std::string("Line code `") + lineCode + "`: ");

    throw std::invalid_argument(errMsg + message);
}

template<typename T, unsigned N>
void addNElements(std::stringstream &lineStream, std::vector<T> &store) {
    std::array<T, N> elements;
    for (auto i = 0U; i < N; ++i)
        lineStream >> elements[i];
    store.insert(store.end(), elements.begin(), elements.end());
}

static void parseLine(const std::string &line, ObjLoader::ObjData &res,
        unsigned lineNumber) {
    std::stringstream lineStream(line);
    std::string lineCode;
    lineStream >> lineCode;

    if (lineCode == "v") {
        addNElements<GLfloat, 3>(lineStream, res.vertices);
    } else if (lineCode == "f") {
        addNElements<ObjLoader::VertexDataIndex, 3>(lineStream, res.faces);
    } else if (lineCode == "vn") {
        addNElements<GLfloat, 3>(lineStream, res.normals);
    } else if (lineCode == "#") {
        return;
    } else {
        throwError("Unrecognized line code", lineNumber, lineCode.c_str());
    }

    if (lineStream.fail())
        throwError("Error while handling line", lineNumber, lineCode.c_str());
    if (!lineStream.eof())
        throwError("Line did not end correctly", lineNumber, lineCode.c_str());
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

std::istream& operator>>(std::istream& in, ObjLoader::VertexDataIndex& vertexDataIndex) {
    std::string elt;
    in >> elt;
    std::stringstream eltStream(elt);
    {
        const std::istream &in(in); // make inStream const and immutable in this context
        eltStream >> vertexDataIndex.verticeIndex;
        // check if slash
        if (eltStream.peek() == '/' || eltStream.peek() == '\'') {
            eltStream.get();
            // check if there is an index following
            if ('0' <= eltStream.peek() && eltStream.peek() <= '9')
                eltStream >> vertexDataIndex.textureIndex;

            // check if there is another slash
            if (eltStream.peek() == '/' || eltStream.peek() == '\'') {
                eltStream.get();
                // check if there is an index following
                if ('0' <= eltStream.peek() && eltStream.peek() <= '9')
                    eltStream >> vertexDataIndex.normalIndex;
            }
        }
    }
    return in;
}

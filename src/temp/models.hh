#pragma once

#include <string>
#include <vector>

#include "model.hh"
#include "program.hh"

struct ModelData {
    Model &model_;
    glm::mat4 model_transform_;
};

class Models
{
public:
    Models() = default;
    ~Models() = default;

    bool addModel(Model &model);

    void translateModel(int index, glm::vec3 translation);
    void rotateModel(int index, float angleInDegree, glm::vec3 axis);
    void scaleModel(int index, glm::vec3 scale);

    void setUniformMat4(int index, const char *name,
                   const glm::mat4& val, bool throwIfMissing) const;

    void draw();

private:
    std::vector<ModelData> modelsData_;
};


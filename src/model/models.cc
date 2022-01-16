//
// Created by lea on 15/07/2021.
//

#include "models.hh"

int Models::addModel(Model &model)
{
    modelsData_.push_back(ModelData{model, glm::mat4(1.0f)});
    return modelsData_.size() - 1;
}

void Models::translateModel(int index, glm::vec3 translation)
{
    modelsData_.at(index).model_transform_ = glm::translate(modelsData_.at(index).model_transform_,
                                                            translation);
}

void Models::rotateModel(int index, float angleInDegree, glm::vec3 axis)
{
    modelsData_.at(index).model_transform_ = glm::rotate(modelsData_.at(index).model_transform_,
                                                         glm::radians(angleInDegree),
                                                         axis);
}

void Models::scaleModel(int index, glm::vec3 scale)
{
    modelsData_.at(index).model_transform_ = glm::scale(modelsData_.at(index).model_transform_,
                                                        scale);
}

void Models::draw() {
    modelsData_[0].model_.getProgram().use();
    for (auto modelData : modelsData_) {
        modelData.model_.getProgram().setUniformMat4("model", modelData.model_transform_, true);
        modelData.model_.draw();
    }
}

void Models::setUniformMat4(int index, const char *name,
                            const glm::mat4 &val,
                            bool throwIfMissing) const
{
    modelsData_.at(index).model_.getProgram().setUniformMat4(name, val, throwIfMissing);
}

void Models::setUniformMat4(const char *name, const glm::mat4 &val,
                            bool throwIfMissing) const
{
    for (auto &model : modelsData_)
        model.model_.getProgram().setUniformMat4(name, val ,throwIfMissing);
}

//
// Created by lea on 15/07/2021.
//

#include "models.hh"

bool Models::addModel(Model &model,
                      const std::string &vertex_path,
                      const std::string &frag_path)
{
    auto* program = program::make_program_path(vertex_path, frag_path);
    if (!program->isready()) {
        std::cerr << "Failed to build shader :\n" << program->getlog() << '\n';
        delete program;
        return false;
    }
    modelsData_.push_back(ModelData{model, program, glm::mat4(1.0f)});
    return true;
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

void Models::draw()
{
    for (auto modelData : modelsData_) {
        modelData.program_->setUniformMat4("model", modelData.model_transform_, true);
        modelData.program_->use();
        modelData.model_.draw(modelData.program_);
    }
}

void Models::setUniformMat4(int index,
                            const char *name,
                            const glm::mat4 &val,
                            bool throwIfMissing) const
{
    modelsData_.at(index).program_->setUniformMat4(name, val, throwIfMissing);
}

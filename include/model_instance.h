#ifndef GIR1_MODEL_INSTANCE_H
#define GIR1_MODEL_INSTANCE_H

#include <model_asset.h>
#include <glm/glm.hpp>


struct ModelInstance {
    ModelAsset *asset{nullptr};
    glm::mat4 transform{};
};

#endif //GIR1_MODEL_INSTANCE_H

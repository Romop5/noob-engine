#include <engine/engine.h>
#include <iostream>
#include <utils/logger.h>

int main() {

    LOG_INFO("Starting viewer\n");
    auto tree = std::make_shared<SceneNode>();

    auto node = std::make_shared<SceneNode>();
    tree->addChild(node);

    LOG_INFO("Json of tree: %s %s nein\n", "wtf",
             tree->to_json().dump(1).c_str());

    
}

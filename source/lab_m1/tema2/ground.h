#pragma once

#ifndef __GROUND_H__
#define __GROUND_H__

#include "components/simple_scene.h"

namespace m1
{
    class GroundClass
    {
    public:
        GroundClass();
        ~GroundClass();

        void Init(WindowObject* window, std::unordered_map<std::string, Mesh*>& meshes, int size);

    public:

    private:
        int size;
        glm::vec3 terrainColor = glm::vec3(0.6f, 0, 0);
    };
}   // namespace m1

#endif
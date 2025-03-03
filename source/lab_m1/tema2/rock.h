#pragma once

#ifndef __ROCK_H__
#define __ROCK_H__

#include "components/simple_scene.h"
#include "AABB.h"

namespace m1
{
    class RockClass
    {
    public:
        RockClass();
        ~RockClass();

        void Init(WindowObject* window, std::unordered_map<std::string, Mesh*>& meshes, glm::mat4 locationMatrix);

        void UpdateLocation(WindowObject* window, float deltaTime, glm::mat4 locationMatrix);

    public:

        std::vector<std::pair<std::string, glm::mat4>> meshList;
        
        std::vector<VertexFormat> vertices1;
        std::vector<VertexFormat> vertices2;

        BoxClass* bodyBox = new BoxClass();
        BoxClass* topBox = new BoxClass();

    private:

        float bodySize;

        glm::vec3 bodyColor = glm::vec3(0.7, 0.7, 0.7);
    };
}   // namespace m1

#endif
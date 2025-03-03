#pragma once

#ifndef __TEMA2_H__
#define __TEMA2_H__

#include "components/simple_scene.h"
#include "camera.h"
#include "drone.h"
#include "rock.h"
#include "package.h"
#include "checkpoint.h"
#include "arrow.h"
#include "AABB.h"
#include <algorithm>
#include <cmath>
#include "lab_m1/tema2/ground.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        bool InitShadowMap();

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderDepthMesh(Mesh* mesh, const glm::mat4& modelMatrix);
        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, bool map);
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, bool map);

        void ColisionCheck();
        bool Intersect(BoxClass* b, DroneClass* d);
        glm::vec3 getRandomLoc();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        cam::Camera* camera;
        cam::Camera* miniMap;
        cam::Camera* shadowCamera;
        glm::mat4 projectionMatrix;
        glm::mat4 projectionMatrixMap;
        glm::mat4 projectionMatrixShadow;

        float fov;
        float latime;
        float inaltime;

        DroneClass *drone = new DroneClass();
        ArrowClass* droneArrow = new ArrowClass();
        GroundClass* terrain = new GroundClass();
        PackageClass* package = new PackageClass();
        CheckClass* checkpoint = new CheckClass();
        ArrowClass* arrow = new ArrowClass();

        std::vector<RockClass> rocks;

        //random loc
        std::vector<glm::vec3> possibleLoc;
        glm::vec3 packageLoc;
        glm::vec3 checkLoc;

        //fog
        //glm::vec3 fogColor = glm::vec3(0.67, 0.41, 0.37);
        glm::vec3 fogColor = glm::vec3(1, 1, 1);

        glm::vec2 fogPos = glm::vec2(0,40);

        //shadow
        GLuint frameBufferShadow;
        GLuint depthMap;
        float depthMapHeight;
        float depthMapWidth;

        //score
        int score;
    };
}   // namespace m1
#endif
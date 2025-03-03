#include "lab_m1/tema2/tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    camera = new cam::Camera();
    camera->Set(glm::vec3(0, 0, 2), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    camera->RotateThirdPerson_OX(-glm::pi<float>()/4);

    miniMap = new cam::Camera();
    miniMap->Set(glm::vec3(0, 100, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, -1));
    miniMap->distanceToTarget = 100;

    shadowCamera = new cam::Camera();
    shadowCamera->Set(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    shadowCamera->distanceToTarget = 100;
    shadowCamera->RotateThirdPerson_OX(-35.264 * glm::pi<float>() / 180);
    shadowCamera->RotateThirdPerson_OY(-glm::pi<float>() / 4);

    glm::ivec2 resolution = window->GetResolution();
    depthMapWidth = resolution.x*2;
    depthMapHeight = resolution.y*2;

    fov = 90;
    inaltime = 60;
    latime = 80;

    score = 0;

    projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, 0.01f, 200.0f);
    projectionMatrixMap = glm::ortho(-latime / 2, latime / 2, -inaltime / 2, inaltime / 2, 0.01f, 200.0f);
    projectionMatrixShadow = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 0.01f, 200.0f);

    cout << InitShadowMap() << endl;

    //obj
    drone->Init(window, meshes);
    droneArrow->Init(window,meshes,glm::vec3(0,0,0));
    droneArrow->bodySize = 1;
    terrain->Init(window, meshes, 100);
    arrow->Init(window, meshes, glm::vec3(-30,90,-20));

    //randomize obstacle location
    for (float i = -90; i <= 90; i += 10) {
        for (float j = -90; j <= 90; j += 10) {
            if (i != 0 && j != 0) {
                possibleLoc.push_back(glm::vec3(i, 0, j));
            }
        }
    }

    packageLoc = getRandomLoc();
    package->Init(window, meshes, packageLoc);

    for (int i = 0; i < 100; i++) {
        rocks.push_back(RockClass());
        
        glm::vec3 loc = getRandomLoc();
        glm::mat4 locMat = glm::mat4(1);
        locMat = glm::translate(locMat, loc);
        
        rocks[i].Init(window, meshes, locMat);
    }

    //shaders init
    {
        Shader* shader = new Shader("GroundShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "GroundVertex.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "GroundFragment.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;

        Shader* shader2 = new Shader("FogShader");
        shader2->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FogVertex.glsl"), GL_VERTEX_SHADER);
        shader2->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FogFragment.glsl"), GL_FRAGMENT_SHADER);
        shader2->CreateAndLink();
        shaders[shader2->GetName()] = shader2;

        Shader* shader3 = new Shader("ShadowShader");
        shader3->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "ShadowVertex.glsl"), GL_VERTEX_SHADER);
        shader3->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "ShadowFragment.glsl"), GL_FRAGMENT_SHADER);
        shader3->CreateAndLink();
        shaders[shader3->GetName()] = shader3;

        Shader* shader4 = new Shader("MapShader");
        shader4->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "MapVertex.glsl"), GL_VERTEX_SHADER);
        shader4->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "MapFragment.glsl"), GL_FRAGMENT_SHADER);
        shader4->CreateAndLink();
        shaders[shader4->GetName()] = shader4;

    }

}

bool Tema2::InitShadowMap() {
    //init frame buffer
    glGenFramebuffers(1, &frameBufferShadow);
    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferShadow);

    //init texture
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);

    glm::ivec2 resolution = window->GetResolution();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, depthMapWidth, depthMapHeight, 0,
        GL_DEPTH_COMPONENT, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    //init framebuffer
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthMap, 0);

    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        return false;
    }
    return true;
}

glm::vec3 Tema2::getRandomLoc() {

    int i = rand() % possibleLoc.size();
    glm::vec3 loc = possibleLoc[i];
    possibleLoc.erase(possibleLoc.begin() + i);

    return loc;
}


void Tema2::ColisionCheck() {
    drone->collision = false;
    drone->pCollision = false;
    for (int i = 0; i < rocks.size(); i++) {
        if (Intersect(rocks[i].bodyBox, drone)) {
            drone->collision = true;
            break;
        }
        if (Intersect(rocks[i].topBox, drone)) {
            drone->collision = true;
            break;
        }
    }
    if (!drone->hasP) {
        drone->pCollision = Intersect(package->box, drone);
    }

    if (drone->hasP) {
        glm::vec2 drone_plane_pos = glm::vec2(drone->orientation->position.x, drone->orientation->position.z);
        glm::vec2 check_plane_pos = glm::vec2(checkpoint->orientation->position.x, checkpoint->orientation->position.z);

        if (abs(drone_plane_pos.x - check_plane_pos.x) < checkpoint->bodySize &&
            abs(drone_plane_pos.y - check_plane_pos.y) < checkpoint->bodySize) {
            //reset package delivery
            //destroy checkpoint
            drone->hasP = false;
            delete checkpoint;
            possibleLoc.push_back(checkLoc);
            checkpoint = new CheckClass();

            //reinit package
            possibleLoc.push_back(packageLoc);
            packageLoc = getRandomLoc();
            package->Init(window, meshes, packageLoc);
            score++;
            cout << "score:"<< score << endl;
        }
    }
}

bool Tema2::Intersect(BoxClass* b, DroneClass* d) {
    float x = max(b->minX, min(d->orientation->position.x, b->maxX));
    float y = max(b->minY, min(d->orientation->position.y, b->maxY));
    float z = max(b->minZ, min(d->orientation->position.z, b->maxZ));

    float distance = sqrt(
        (x - d->orientation->position.x) * (x - d->orientation->position.x) +
        (y - d->orientation->position.y) * (y - d->orientation->position.y) +
        (z - d->orientation->position.z) * (z - d->orientation->position.z)
    );

    return distance < d->radius;
}

void Tema2::FrameStart()
{
    glClearColor(fogColor.x, fogColor.y, fogColor.z, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);

    ColisionCheck();
}

void Tema2::Update(float deltaTimeSeconds)
{   
    glm::vec3 new_forward = drone->orientation->forward -
        glm::dot(drone->orientation->forward, glm::vec3(0, 1, 0)) * glm::vec3(0, 1, 0);

    //shadow pass
    {
        shadowCamera->GetSetCameraPosition(drone->orientation->position);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBufferShadow);
        glm::ivec2 resolution = window->GetResolution();
        glViewport(0, 0, depthMapWidth, depthMapHeight);
        glClear(GL_DEPTH_BUFFER_BIT);

        for (auto& pair : drone->meshList) {
            RenderDepthMesh(meshes[pair.first], pair.second);
        }

        for (int i = 0; i < rocks.size(); i++) {
            for (auto& pair : rocks[i].meshList) {
                RenderDepthMesh(meshes[pair.first], pair.second);
            }
        }
        RenderDepthMesh(meshes["package"], package->pMatrix);
        if (checkpoint) {
            RenderDepthMesh(meshes["checkpoint"], checkpoint->cMatrix);
        }
        RenderDepthMesh(meshes["terrain"], glm::mat4(1));
    }

    //normal pass
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glm::ivec2 resolution = window->GetResolution();
        glViewport(0, 0, resolution.x, resolution.y);
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        drone->UpdateLocation(window, deltaTimeSeconds, modelMatrix);

        if (drone->hasP) {
            package->orientation->SetPackagePosition(drone->orientation->position);
            package->UpdateLocation(window, deltaTimeSeconds);

        }
       
        camera->GetSetCameraPosition(drone->orientation->position);
        /*
        camera->RotateThirdPerson_OX(glm::pi<float>() / 4);
        camera->forward = glm::normalize(new_forward);
        camera->right = glm::cross(droneArrow->orientation->forward, camera->up);
        camera->right = glm::normalize(camera->right);
        camera->RotateThirdPerson_OX(-glm::pi<float>() / 4);
        */
        //objects render
        for (auto& pair : drone->meshList) {
            RenderSimpleMesh(meshes[pair.first], shaders["FogShader"], pair.second, false);
        }

        for (int i = 0; i < rocks.size(); i++) {
            for (auto& pair : rocks[i].meshList) {
                RenderSimpleMesh(meshes[pair.first], shaders["FogShader"], pair.second, false);
            }
        }
        RenderSimpleMesh(meshes["package"], shaders["FogShader"], package->pMatrix, false);
        if (checkpoint) {
            RenderSimpleMesh(meshes["checkpoint"], shaders["FogShader"], checkpoint->cMatrix, false);
        }
        RenderSimpleMesh(meshes["terrain"], shaders["GroundShader"], glm::mat4(1), false);
    }

    // minmap pass
    {
        glClear(GL_DEPTH_BUFFER_BIT);
        //minimap calculations
        miniMap->GetSetCameraPosition(camera->position);
        glm::vec3 new_up =camera->forward -
                            glm::dot(camera->forward, glm::vec3(0,1,0)) * glm::vec3(0, 1, 0);
        miniMap->up = glm::normalize(new_up);
        miniMap->right = glm::cross(miniMap->up, miniMap->forward);
        miniMap->right = glm::normalize(miniMap->right);

        arrow->orientation->position = glm::vec3(drone->orientation->position.x, 90, drone->orientation->position.z)
            + miniMap->up*15.0f + miniMap->right * 25.0f;
            //+ glm::vec3(-30, 90, -20);
        
        droneArrow->orientation->position = drone->orientation->position;
        droneArrow->orientation->forward = glm::normalize(new_forward);
        droneArrow->orientation->right = glm::cross(droneArrow->orientation->forward, droneArrow->orientation->up);
        droneArrow->orientation->right = glm::normalize(droneArrow->orientation->right);
        droneArrow->UpdateLocation(deltaTimeSeconds);

        if (drone->hasP) {
            arrow->PointTo(checkpoint->orientation->position);
        }
        else {
            arrow->PointTo(package->orientation->position);
        }
        arrow->UpdateLocation(deltaTimeSeconds);

        glm::ivec2 resolution = window->GetResolution();
        glViewport(0, 0, resolution.x/4, resolution.y/4);

        RenderMesh(meshes["arrow"], shaders["VertexColor"], droneArrow->aMatrix , true);
        RenderMesh(meshes["arrow"], shaders["VertexColor"], arrow->aMatrix, true);

        for (int i = 0; i < rocks.size(); i++) {
            for (auto& pair : rocks[i].meshList) {
                RenderMesh(meshes[pair.first], shaders["VertexColor"], pair.second, true);
            }
        }
        if (!drone->hasP) {
            RenderMesh(meshes["package"], shaders["VertexColor"], package->pMatrix, true);
        }
        if (checkpoint) {
            RenderMesh(meshes["checkpoint"], shaders["VertexColor"], checkpoint->cMatrix, true);
        }

        RenderMesh(meshes["terrain"], shaders["GroundShader"], glm::mat4(1), true);
    }
}

void Tema2::FrameEnd()
{

    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderDepthMesh(Mesh* mesh, const glm::mat4& modelMatrix) {
    Shader* shader = shaders["ShadowShader"];

    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    int location = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    location = glGetUniformLocation(shader->program, "View");
    glm::mat4 viewMatrix = shadowCamera->GetViewMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    location = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrixShadow));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, bool map)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    glUseProgram(shader->program);

    int location = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    location = glGetUniformLocation(shader->program, "View");
    glm::mat4 viewMatrix;
    if (map) {
        viewMatrix = miniMap->GetViewMatrix();

    }
    else {
        viewMatrix = camera->GetViewMatrix();
    }
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    location = glGetUniformLocation(shader->program, "Projection");

    if (map) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrixMap));
    }
    else {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    }

    int loc = glGetUniformLocation(shader->program, "shadowMatrix");
    glm::mat4 shadowMatrix = projectionMatrixShadow * shadowCamera->GetViewMatrix() * modelMatrix;
    //shadowMatrix = glm::translate(shadowMatrix, glm::vec3(0.5,0.5,0.5));
    //shadowMatrix = glm::scale(shadowMatrix, glm::vec3(0.5,0.5,0.5));

    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(shadowMatrix));

    loc = glGetUniformLocation(shader->program, "fog_position");
    glUniform2f(loc, fogPos.x, fogPos.y);

    loc = glGetUniformLocation(shader->program, "fog_color");
    glUniform3f(loc, fogColor.x, fogColor.y, fogColor.z);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glUniform1i(glGetUniformLocation(shader->program, "shadow"), 0);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, bool map)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();

    if (map) {
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(miniMap->GetViewMatrix()));
    }
    else {
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    }

    if(map){
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrixMap));
    }
    else {
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    }
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int loc = glGetUniformLocation(shader->program, "shadowMatrix");
    glm::mat4 shadowMatrix = projectionMatrixShadow * shadowCamera->GetViewMatrix() * modelMatrix;
    //shadowMatrix = glm::translate(shadowMatrix, glm::vec3(0.5, 0.5, 0.5));
    //shadowMatrix = glm::scale(shadowMatrix, glm::vec3(0.5, 0.5, 0.5));

    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(shadowMatrix));

    loc = glGetUniformLocation(shader->program, "fog_position");
    glUniform2f(loc, fogPos.x, fogPos.y);

    loc = glGetUniformLocation(shader->program, "fog_color");
    glUniform3f(loc, fogColor.x, fogColor.y, fogColor.z);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glUniform1i(glGetUniformLocation(shader->program, "shadow"), 0);

    mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods)
{   
    drone->InputUpdate(window, deltaTime);
    
}


void Tema2::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_X && drone->pCollision 
        && drone->orientation->position.y > package->orientation->position.y) {
        drone->hasP = true;
        checkLoc = getRandomLoc();
        glm::vec3 loc = glm::vec3(checkLoc.x, checkLoc.y+1, checkLoc.z);
        checkpoint->Init(window, meshes, loc);
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    drone->OnKeyRelease(key, mods);
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
        camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}

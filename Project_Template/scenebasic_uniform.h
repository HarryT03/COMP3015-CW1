#ifndef SCENEBASIC_UNIFORM_H
#define SCENEBASIC_UNIFORM_H

#include "helper/scene.h"

#include <glad/glad.h>
#include "helper/glslprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glm/glm.hpp"
#include "helper/plane.h"
#include "helper/objmesh.h"
#include "helper/skybox.h"


class SceneBasic_Uniform : public Scene
{
private:

    Plane plane;
    SkyBox sky;
    std::unique_ptr<ObjMesh> mesh;
    float tPrev;
    float angle;
    GLuint brick, cement;
    GLSLProgram prog;
    GLSLProgram progSkyBox;
    void setMatrices();
    void setSkyBoxMatrices();
    void compile();



public:
    SceneBasic_Uniform();

    void initScene();
    void update(float t);
    void render();
    void resize(int, int);
    
};

#endif // SCENEBASIC_UNIFORM_H

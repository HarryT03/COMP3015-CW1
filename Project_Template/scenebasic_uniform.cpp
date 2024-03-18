#include "scenebasic_uniform.h"
#include <cstdio>
#include <cstdlib>
#include <string>
using std::string;

#include <sstream>
#include <iostream>
using std::cerr;
using std::endl;

#include "helper/glutils.h"
#include "helper/texture.h"

using glm::vec3;
using glm::vec4;
using glm::mat3;
using glm::mat4;

SceneBasic_Uniform::SceneBasic_Uniform() : plane(50.0f, 50.0f, 1, 1), sky(100.0f), tPrev(0) {
    
    mesh = ObjMesh::load("media/streetlight.obj", true);
}

void SceneBasic_Uniform::initScene()
{
    compile();
    glEnable(GL_DEPTH_TEST);
    model = mat4(1.0f);

    vec3 cameraPos = vec3(4.0f, 5.0f, 5.0f);
    vec3 targetPos = vec3(0.0f, 1.00f, 0.0f);
    vec3 upDir = vec3(0.0f, 10.0f, 0.0f);
    view = glm::lookAt(cameraPos, targetPos, upDir);

    projection = mat4(1.0f);
    angle = 0.0f;

    prog.setUniform("Light.La", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Light.L", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Light.Position", view * vec4(5.0f, 5.0f, 5.0f, 1.0f));

    prog.setUniform("Spot.L", vec3(0.9f));
    prog.setUniform("Spot.La", vec3(0.5f));
    prog.setUniform("Spot.Exponent", 50.0f);
    prog.setUniform("Spot.Cutoff", glm::radians(15.0f));

    prog.setUniform("Fog.MaxDist", 100.0f);
    prog.setUniform("Fog.MinDist", 1.0f);
    prog.setUniform("Fog.Color", vec3(0.5f));


    GLuint cement = Texture::loadTexture("media/texture/cement.jpg");
    GLuint brick = Texture::loadTexture("media/texture/brick1.jpg");
    GLuint skybox = Texture::loadHdrCubeMap("media/texture/cube/pisa-hdr/pisa");

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, cement);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, brick);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox);
}



void SceneBasic_Uniform::compile()
{
	try {
		prog.compileShader("shader/basic_uniform.vert");
		prog.compileShader("shader/basic_uniform.frag");
		prog.link();
        prog.use();

        progSkyBox.compileShader("shader/skybox.vert");
        progSkyBox.compileShader("shader/skybox.frag");
        progSkyBox.link();

	} catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}

void SceneBasic_Uniform::update( float t )
{
    float deltaT = t - tPrev;
    if (tPrev == 0.0f) deltaT = 0.0f;
    tPrev = t;
    angle += 0.1f * deltaT;
    if (angle > glm::two_pi<float>()) angle -= glm::two_pi<float>();
}

void SceneBasic_Uniform::render()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    vec4 lightPos = vec4(10.0f * cos(angle), 10.0f, 10.0f*sin(angle), 1.0f);
    mat3 normalMatrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
    
    //SPOTLIGHT
    prog.use();
    prog.setUniform("Spot.Position", vec3(view*lightPos));
    prog.setUniform("Spot.Direction", normalMatrix*vec3(-lightPos));
    
    //MESH
    prog.setUniform("Material.Kd", 0.4f, 0.4f, 0.4f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Shininess", 100.0f);
    prog.setUniform("Material.Choice", 0);

    model = mat4(1.0f);
    model = glm::scale(model, vec3(0.1f));
    model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vec3(0.0f, 0.0f, 0.0f));
    setMatrices();
    mesh->render(); 

    //PLANE
    prog.setUniform("Material.Kd", 0.1f, 0.1f, 0.1f);
    prog.setUniform("Material.Ka", 0.5f, 0.5f, 0.5f);
    prog.setUniform("Material.Ks", 0.9f, 0.9f, 0.9f);
    prog.setUniform("Material.Shininess", 180.0f);
    prog.setUniform("Material.Choice", 2);

    model = mat4(1.0f);
    model = glm::translate(model, vec3(0.0f, -0.45f, 0.0f));
    setMatrices();
    plane.render();

    //SKYBOX
    progSkyBox.use();
    model = mat4(1.0f);
    setSkyBoxMatrices();
    sky.render();

}

void SceneBasic_Uniform::resize(int w, int h)
{
    glViewport(0, 0, w, h);
    width = w;
    height = h;
    projection = glm::perspective(glm::radians(70.0f), (float)w / h, 0.3f, 100.0f);
}

void SceneBasic_Uniform::setMatrices() {
    mat4 mv = view * model;
    prog.setUniform("ModelViewMatrix", mv);
    prog.setUniform("NormalMatrix", glm::mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));
    prog.setUniform("MVP", projection * mv);
}

void SceneBasic_Uniform::setSkyBoxMatrices() {
    mat4 mv = view * model;
    progSkyBox.setUniform("MVP", projection * mv);
}
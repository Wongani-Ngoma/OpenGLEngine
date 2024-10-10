#include "../../Core/Model.h"
#include "../../Game/Camera/Camera.h"
#include "../../Backend/BackEnd.h"

class Axes {
public:
    // lil hack, we use texture coords as colors
    Axes() {

    }
    void setup() {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        vertices.push_back(Vertex(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), glm::vec2(0.0, 0.0)));
        vertices.push_back(Vertex(glm::vec3(1.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
        vertices.push_back(Vertex(glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0), glm::vec2(0.0, 0.0)));
        vertices.push_back(Vertex(glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, 0.0, 1.0), glm::vec2(0.0, 0.0)));

        indices = {0,1,0,2,0,3};

        _mesh = Mesh(vertices, indices);
    }

    ~Axes() {
        _mesh.cleanup();
    }

    void render(Shader& shader, Camera camera) {

        shader.activate();

        glm::mat4 model(1.0);
        glm::mat4 view(1.0);

        model = glm::translate(model, camera._cameraPos +
            glm::vec3(camera._cameraFront) * 0.2f);
        view = camera.getViewMatrix();

        model = glm::scale(model, size);

        shader.setMat4("model", model);
        shader.setMat4("view", view);

        // set viewport to the top right corner of the screen and disble depth test
        glViewport(
            BackEnd::getScreenWidth() - BackEnd::getScreenWidth() / 10, BackEnd::getScreenHeight() - BackEnd::getScreenHeight() / 10,
            BackEnd::getScreenWidth()  / 10, BackEnd::getScreenHeight()  / 10
        );

        _mesh.setDrawMode(GL_LINES);
        _mesh.render(shader);

        // restore the viewport
        glViewport(0, 0, BackEnd::getScreenWidth(), BackEnd::getScreenHeight());
    }


private:
    Mesh _mesh;
    glm::vec3 size = glm::vec3(1.0);
};
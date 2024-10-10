#include "Room.h"

Room::Room() {
}

Room::Room(const char* wallPath, glm::vec3 pos) {
    Transformation t;

    t.position = pos + glm::vec3(roomLength / 2.0, 0.0, 0.0);
    t.localRotation = glm::vec3(0.0, glm::radians(0.0f), 0.0);
    walls[0] = StaticGameObject(wallPath, t);

    t.position = pos - glm::vec3(roomLength / 2.0, 0.0, 0.0);
    t.localRotation = glm::vec3(0.0, glm::radians(180.0f), 0.0);
    walls[1] = StaticGameObject(wallPath, t);

    t.position = pos + glm::vec3(0.0, 0.0, roomLength / 2.0);
    t.localRotation = glm::vec3(0.0, glm::radians(90.0f), 0.0);
    walls[2] = StaticGameObject(wallPath, t);

    t.position = pos - glm::vec3(0.0, 0.0, roomLength / 2.0);
    t.localRotation = glm::vec3(0.0, glm::radians(- 90.0f), 0.0);
    walls[3] = StaticGameObject(wallPath, t);

    t.position = pos - glm::vec3(0.0, roomLength / 2.0, 0.0);
    t.localRotation = glm::vec3(0.0, 0.0, glm::radians(- 90.0f));
    walls[4] = StaticGameObject(wallPath, t);
}

void Room::update() {
    for (int i = 0; i < 4; i++) {
        walls[i].update();
    }
}

void Room::render(Shader& shader) {
    for (int i = 0; i < 4; i++) {
        walls[i].render(shader);
    }
}

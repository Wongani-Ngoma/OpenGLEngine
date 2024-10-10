#pragma once

#ifndef SCENE_H
#define SCENE_H

#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <fstream>

#include <vector>
#include "../Physics/StaticGameObject.h"
#include "../Physics/DynamicGameObject.h"

#include "../MapLoader/JsonObjectLoader.h"

namespace Scene {
    void init(std::vector<StaticGameObject>* staticGameObjects, std::vector<DynamicGameObject>* dynamicGameObjects);
    void loadScene(const char* mapFile);
    void saveEntireScene(std::string filename);
    void saveObjectToMapFile(std::ofstream* outFile, StaticGameObject object);
    void saveObjectToMapFile(std::ofstream* outFile, DynamicGameObject object);
    void loadGameObject(const Value& value);
};

#endif
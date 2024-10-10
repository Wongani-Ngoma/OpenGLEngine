#pragma once
#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <istream>

#include "../JsonHelper.hpp"
#include "../Util.hpp"

#include "../Physics/DynamicGameObject.h"
#include "../Physics/StaticGameObject.h"

using namespace rapidjson;

namespace JsonObjectLoader {
    DynamicGameObject loadObject(std::string path);
};

#endif MODEL_LOADER_H
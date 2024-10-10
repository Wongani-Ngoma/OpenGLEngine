#include "Scene.h"

namespace Scene {
    std::vector<StaticGameObject>* _staticGameObjects;
    std::vector<DynamicGameObject>* _dynamicGameObjects;

}

void Scene::init(std::vector<StaticGameObject>*staticGameObjects, std::vector<DynamicGameObject>*dynamicGameObjects) {
    _staticGameObjects = staticGameObjects;
    _dynamicGameObjects = dynamicGameObjects;
}

void Scene::loadScene(const char* mapFile) {
    Document document;

    std::string json = Util::getFileContents(mapFile);
    rapidjson::ParseResult result = document.Parse(json.c_str());

    // Check for parse errors 
    if (document.HasParseError()) {
        std::cerr << "Error parsing JSON: " << document.GetParseError() << "\n";

        // Print the problematic part of the JSON
        size_t start = result.Offset() > 20 ? result.Offset() - 20 : 0;
        size_t length = 40;
        if (start + length > json.length()) {
            length = json.length() - start;
        }
        std::cerr << "Context: ..." << json.substr(start, length) << "..." << std::endl;


        return;
    }


    // Iterate over static objects
    if (document.HasMember("staticObjects") && document["staticObjects"].IsObject()) {
        const auto& staticObjects = document["staticObjects"];
        for (auto it = staticObjects.MemberBegin(); it != staticObjects.MemberEnd(); ++it) {
            loadGameObject(it->value);
        }
    }

    // Iterate over dynamic objects
    if (document.HasMember("dynamicObjects") && document["dynamicObjects"].IsObject()) {
        const auto& dynamicObjects = document["dynamicObjects"];
        for (auto it = dynamicObjects.MemberBegin(); it != dynamicObjects.MemberEnd(); ++it) {
            loadGameObject(it->value);
        }
    }
    
}

void Scene::loadGameObject(const Value& value) {

    std::string thePath = value["path"].GetString();
    glm::vec3 pos = JsonGlmHelper::toVec3(value["position"]);
    glm::vec3 scale = JsonGlmHelper::toVec3(value["scale"]);
    glm::vec3 rot = JsonGlmHelper::toVec3(value["rotation"]);

    if (value["type"] == "static") {
        StaticGameObject obj = StaticGameObject(thePath, Transformation(pos, rot));
        _staticGameObjects->push_back(obj);
    }
    else if (value["type"] == "dynamic") {
        DynamicGameObject obj = DynamicGameObject(thePath, Transformation(pos, rot));
        _dynamicGameObjects->push_back(obj);
    }

}


void Scene::saveEntireScene(std::string filename) {

    std::ofstream outFile(filename);
    if (outFile.is_open()) {

        outFile << "{\n";

        outFile << "\n\"staticObjects\": {\n";
        for (int i = 0; i < (*_staticGameObjects).size(); i++) {
            saveObjectToMapFile(&outFile, (*_staticGameObjects)[i]);
            if(i < ((*_staticGameObjects).size() - 1)) outFile << ",";
        }
        outFile << "},";

        outFile << "\"dynamicObjects\": {\n";
        for (int i = 0; i < (*_dynamicGameObjects).size(); i++) {
            saveObjectToMapFile(&outFile, (*_dynamicGameObjects)[i]);
            if (i < ((*_dynamicGameObjects).size() - 1)) outFile << ",";

        }
        outFile << "}";


        outFile << "}";
        outFile.close();
    }
    else {
        throw std::runtime_error("Unable to open file for writing: " + filename);
    }

    
}

void Scene::saveObjectToMapFile(std::ofstream* outFile, StaticGameObject object) {
    (*outFile) << "\"" + object.getName() + "\" :";
    rapidjson::Document document;
    document.SetObject();

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

    // Add string values
    document.AddMember("id", object.getID(), allocator);
    document.AddMember("path", rapidjson::Value(object.getModel()->_directory.c_str(), allocator).Move(), allocator);
    document.AddMember("type", rapidjson::Value("static", allocator).Move(), allocator);

    // Add vec3 values
    auto addVec3 = [&document, &allocator](const char* name, const glm::vec3& vec) {
        rapidjson::Value vecArray(rapidjson::kArrayType);
        vecArray.PushBack(vec.x, allocator);
        vecArray.PushBack(vec.y, allocator);
        vecArray.PushBack(vec.z, allocator);
        document.AddMember(rapidjson::Value(name, allocator).Move(), vecArray, allocator);
        };

    addVec3("position", object.getModel()->_transformation.position);
    addVec3("rotation", Util::vec3FromQuat(object.getModel()->_transformation.localRotation));
    addVec3("scale", object.getModel()->_transformation._scale);

    // Write to file
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

   (*outFile) << buffer.GetString();
}

void Scene::saveObjectToMapFile(std::ofstream* outFile, DynamicGameObject object) {
    rapidjson::Document document;
    document.SetObject();

    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    (*outFile) << "\"" + object.getName() + "\" :";

    // Add string values
    document.AddMember("id", object.getID(), allocator);
    document.AddMember("path", rapidjson::Value(object.getModel()->_directory.c_str(), allocator).Move(), allocator);
    document.AddMember("type", rapidjson::Value("dynamic", allocator).Move(), allocator);

    // Add vec3 values
    auto addVec3 = [&document, &allocator](const char* name, const glm::vec3& vec) {
        rapidjson::Value vecArray(rapidjson::kArrayType);
        vecArray.PushBack(vec.x, allocator);
        vecArray.PushBack(vec.y, allocator);
        vecArray.PushBack(vec.z, allocator);
        document.AddMember(rapidjson::Value(name, allocator).Move(), vecArray, allocator);
        };

    addVec3("position", object.getModel()->_transformation.position);
    addVec3("rotation", Util::vec3FromQuat(object.getModel()->_transformation.localRotation));
    addVec3("scale", object.getModel()->_transformation._scale);

    // Write to file
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    (*outFile) << buffer.GetString();
    (*outFile) << "\n";
}
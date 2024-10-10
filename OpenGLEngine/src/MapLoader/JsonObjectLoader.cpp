#include "JsonObjectLoader.h"

DynamicGameObject JsonObjectLoader::loadObject(std::string path) {
    std::string file = Util::getFileContents(path.c_str());
    Document document;

    //document.Parse(file.c_str());
    document.Parse(file.c_str());

    // Check for parse errors 
    if (document.HasParseError()) { 
        std::cerr << "Error parsing JSON: " << document.GetParseError() << "\n";
        return DynamicGameObject();

    } 

    std::string thePath = document["path"].GetString();
    glm::vec3 pos = JsonGlmHelper::toVec3(document["pos"]);
    glm::vec3 scale = JsonGlmHelper::toVec3(document["scale"]);
    glm::vec3 rot = JsonGlmHelper::toVec3(document["rotation"]);

    DynamicGameObject obj = DynamicGameObject(thePath, Transformation(pos, scale, rot));

    return obj;
}
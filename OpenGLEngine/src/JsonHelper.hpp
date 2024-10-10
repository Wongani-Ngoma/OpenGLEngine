#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <string>

class JsonGlmHelper {

public:
    // Convert a RapidJSON value to glm::vec3
    static inline glm::vec3 toVec3(const rapidjson::Value& value) {
        if (value.IsArray() && value.Size() == 3) {
            return glm::vec3(
                value[0].GetFloat(),
                value[1].GetFloat(),
                value[2].GetFloat()
            );
        } 
    }

    // Get a glm::vec3 from a RapidJSON object using a key
    static inline glm::vec3 getVec3(const rapidjson::Value& object, const char* key) {
        if (object.HasMember(key)) {
            return toVec3(object[key]);
        } else {
            throw std::out_of_range("Key not found in JSON object");
        }
    }

};
#endif JSON_HELPER_H


/*
#include <rapidjson/document.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <string>

namespace json_glm_helper {

// Convert a RapidJSON value to glm::vec3
glm::vec3 to_vec3(const rapidjson::Value& value) {
if (value.IsArray() && value.Size() == 3) {
return glm::vec3(
value[0].GetFloat(),
value[1].GetFloat(),
value[2].GetFloat()
);
} else if (value.IsObject() && value.HasMember("x") && value.HasMember("y") && value.HasMember("z")) {
return glm::vec3(
value["x"].GetFloat(),
value["y"].GetFloat(),
value["z"].GetFloat()
);
} else {
throw std::invalid_argument("JSON value cannot be converted to glm::vec3");
}
}

// Convert a RapidJSON value to glm::vec3, with a default value if conversion fails
glm::vec3 to_vec3(const rapidjson::Value& value, const glm::vec3& default_value) {
try {
return to_vec3(value);
} catch (const std::exception&) {
return default_value;
}
}

// Get a glm::vec3 from a RapidJSON object using a key
glm::vec3 get_vec3(const rapidjson::Value& object, const char* key) {
if (object.HasMember(key)) {
return to_vec3(object[key]);
} else {
throw std::out_of_range("Key not found in JSON object");
}
}

// Get a glm::vec3 from a RapidJSON object using a key, with a default value if the key is not found or conversion fails
glm::vec3 get_vec3(const rapidjson::Value& object, const char* key, const glm::vec3& default_value) {
if (object.HasMember(key)) {
return to_vec3(object[key], default_value);
} else {
return default_value;
}
}

} // namespace json_glm_helper
*/
#ifndef Util_HPP
#define Util_HPP

#include <fstream>
#include <PxPhysicsAPI.h>

#include "Common.h"
#include "Core/Model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iterator>

using namespace physx;

namespace Util {
    inline glm::vec3 vec3FromQuat(glm::quat q) {
        return glm::vec3(q.x, q.y, q.z);
    }


    inline PxTransform PxTransFromModelTrans(Transformation modelTrans) {
        PxTransform PxTrans(PxVec3(0.0));
        PxTrans.p = PxVec3(modelTrans.position.x, modelTrans.position.y, modelTrans.position.z);
        PxTrans.q = PxQuat(modelTrans.localRotation.x, modelTrans.localRotation.y, modelTrans.localRotation.z, modelTrans.localRotation.w);
        return PxTrans;
    }

    inline glm::vec3 GlmVec3FromPxVec3(PxVec3 v) {
        return glm::vec3(v.x, v.y, v.z);
    }

    inline glm::quat GlmQuatFromPxQuat(PxQuat q) {
        return glm::quat(q.w, q.x, q.y, q.z);
    }

    inline PxVec3 PxVec3FromGlmVec3(glm::vec3 v) {

        return PxVec3(v.x, v.y, v.z);
    }

    inline std::string getFileContents(const char* filename) {
        std::ifstream file;
        std::stringstream buff;
        std::string ret = "";

        file.open(filename);
        if (file.is_open()) {
            buff << file.rdbuf();
            ret = buff.str();
            file.close();
        }
        else {
            std::cout << "ERROR: Could not open file: " << filename << "\n";
        }

        return ret;
    }

    // shamelessly copied from Lord Tokyospliff, too retarted to figure it out meself
    inline glm::vec3 GetMouseRay(glm::mat4 projection, glm::mat4 view, int windowWidth, int windowHeight, int mouseX, int mouseY) {
        float x = (2.0f * mouseX) / (float)windowWidth - 1.0f;
        float y = 1.0f - (2.0f * mouseY) / (float)windowHeight;
        float z = 1.0f;
        glm::vec3 ray_nds = glm::vec3(x, y, z);
        glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, ray_nds.z, 1.0f);
        glm::vec4 ray_eye = glm::inverse(projection) * ray_clip;
        ray_eye = glm::vec4(ray_eye.x, ray_eye.y, ray_eye.z, 0.0f);
        glm::vec4 inv_ray_wor = (inverse(view) * ray_eye);
        glm::vec3 ray_wor = glm::vec3(inv_ray_wor.x, inv_ray_wor.y, inv_ray_wor.z);
        ray_wor = normalize(ray_wor);
        return ray_wor;
    }

    inline void writeToFile(const char* filePath, const char* contents) {
        std::ofstream file(filePath);
        if (file.is_open()) {
            file << contents;
        } else {
            std::cout << "Could not open file:" << filePath << " for writing\n";
        }
    }

}

#endif // !Util_HPP
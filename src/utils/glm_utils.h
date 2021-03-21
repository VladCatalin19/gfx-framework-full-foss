#pragma once

#include <ostream>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"


inline glm::vec3 NormalizedRGB(unsigned char r, unsigned char g, unsigned char b) {
    return glm::vec3(r / 255.f, g / 255.f, b / 255.f);
}


/**
 *  overload ostream operator << for easy printing glm classes
 */
inline std::ostream &operator<< (std::ostream &out, const glm::ivec2 &vec) {
    out << "[" << vec.x << " " << vec.y << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::vec2 &vec) {
    out << "[" << vec.x << " " << vec.y << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::ivec3 &vec) {
    out << "[" << vec.x << " " << vec.y << " " << vec.z << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::vec3 &vec) {
    out << "[" << vec.x << " " << vec.y << " " << vec.z << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::ivec4 &vec) {
    out << "[" << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::vec4 &vec) {
    out << "[" << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << "]";
    return out;
}

inline std::ostream &operator<< (std::ostream &out, const glm::quat &rot) {
    out << "[" << rot.x << " " << rot.y << " " << rot.z << " " << rot.w << "]";
    return out;
}

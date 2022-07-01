#ifndef GFXC_TRANSFORM_INTERNALS_H
#define GFXC_TRANSFORM_INTERNALS_H

#include "transform.h"

namespace gfxc
{
    struct TransformInternals
    {
        glm::mat4               m_worldModel;

        // Rotations
        glm::quat               m_worldRotation;
        glm::quat               m_relativeRotation;
        glm::quat               m_invWorldRotation;

        // Positions
        glm::vec3               m_worldPosition;
        glm::vec3               m_localPosition;

        // Scale relative to the object, not influenced by rotations
        glm::vec3               m_localScalingFactor;

        // Speeds
        float                   m_rotationSpeed;
        float                   m_translationSpeed;
        float                   m_scalingSpeed;

        bool                    m_isInMotion;
        bool                    m_isModelOutdated;
        bool                    m_updateHierarchy;

        Transform *             m_parentNode;
        std::list<Transform *>  m_childNodes;

        TransformInternals(const Transform &transform);
        ~TransformInternals() = default;
        TransformInternals(const TransformInternals &) = default;
        TransformInternals(TransformInternals &&) = default;
        TransformInternals& operator=(const TransformInternals &) = default;
        TransformInternals& operator=(TransformInternals &&) = default;

        bool operator==(const TransformInternals &other);
        bool operator!=(const TransformInternals &other);

        void CopyFrom(const Transform &transform);
    };
}

#endif // GFXC_TRANSFORM_INTERNALS_H

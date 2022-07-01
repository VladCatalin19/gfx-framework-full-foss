#include "transform_internals.h"

namespace gfxc
{
    TransformInternals::TransformInternals(const Transform &transform)
    {
        CopyFrom(transform);
    }

    bool TransformInternals::operator==(const TransformInternals &other)
    {
        return (m_worldModel == other.m_worldModel)
                and (m_worldRotation == other.m_worldRotation)
                and (m_relativeRotation == other.m_relativeRotation)
                and (m_invWorldRotation == other.m_invWorldRotation)
                and (m_worldPosition == other.m_worldPosition)
                and (m_localPosition == other.m_localPosition)
                and (m_localScalingFactor == other.m_localScalingFactor)
                and (m_rotationSpeed == other.m_rotationSpeed)
                and (m_translationSpeed == other.m_translationSpeed)
                and (m_scalingSpeed == other.m_scalingSpeed)
                and (m_isInMotion == other.m_isInMotion)
                and (m_isModelOutdated == other.m_isModelOutdated)
                and (m_updateHierarchy == other.m_updateHierarchy)
                and (m_parentNode == other.m_parentNode)
                and (m_childNodes == other.m_childNodes);
    }

    bool TransformInternals::operator!=(const TransformInternals &other)
    {
        return !(*this == other);
    }

    void TransformInternals::CopyFrom(const Transform &transform)
    {
        m_worldModel = transform.m_worldModel;

        // Rotations
        m_worldRotation = transform.m_worldRotation;
        m_relativeRotation = transform.m_relativeRotation;
        m_invWorldRotation = transform.m_invWorldRotation;

        // Positions
        m_worldPosition = transform.m_worldPosition;
        m_localPosition = transform.m_localPosition;

        // Scale relative to the object, not influenced by rotations
        m_localScalingFactor = transform.m_localScalingFactor;

        // Speeds
        m_rotationSpeed = transform.m_rotationSpeed;
        m_translationSpeed = transform.m_translationSpeed;
        m_scalingSpeed = transform.m_scalingSpeed;

        m_isInMotion = transform.m_isInMotion;
        m_isModelOutdated = transform.m_isModelOutdated;
        m_updateHierarchy = transform.m_updateHierarchy;

        m_parentNode = transform.m_parentNode;
        m_childNodes = transform.m_childNodes;
    }
}

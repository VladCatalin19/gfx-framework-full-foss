#include "glm/ext/quaternion_common.hpp"
#include "glm/gtx/quaternion.hpp"
#include "transform.h"

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

namespace gfxc
{
    Transform::Transform()
        : m_worldRotation(0.0F, 0.0F, 0.0F, 0.0F),
          m_relativeRotation(0.0F, 0.0F, 0.0F, 0.0F),
          m_invWorldRotation(0.0F, 0.0F, 0.0F, 0.0F),
          m_worldPosition(0.0F),
          m_localPosition(0.0F),
          m_localScalingFactor(glm::vec3(1.0F)),
          m_rotationSpeed(1.0F),
          m_translationSpeed(1.0F),
          m_scalingSpeed(0.02F)
    {
        Init();
    }

    Transform::Transform(const Transform &transform)
        : m_worldRotation(transform.m_worldRotation),
          m_invWorldRotation(glm::inverse(transform.m_worldRotation)),
          m_worldPosition(transform.m_worldPosition),
          m_localPosition(transform.m_localPosition),
          m_localScalingFactor(transform.m_localScalingFactor),
          m_rotationSpeed(transform.m_rotationSpeed),
          m_translationSpeed(transform.m_translationSpeed),
          m_scalingSpeed(transform.m_scalingSpeed)
    {
        Init();
    }
    
    Transform::~Transform()
    {

    }
    

    // ****************************
    // Transform motion state

    void Transform::SetHierarchyUpdate(bool value)
    {
        m_updateHierarchy = value;
    }
    
    void Transform::ClearMotionState()
    {
        m_isInMotion = false;

        for (Transform *child : m_childNodes)
        {
            child->ClearMotionState();
        }
    }
    
    bool Transform::GetMotionState() const
    {
        return m_isInMotion;
    }
    

    // ****************************
    // Get transform properties

    glm::vec3 Transform::GetLocalPosition() const
    {
        return m_localPosition;
    }
    
    glm::vec3 Transform::GetWorldPosition() const
    {
        return m_worldPosition;
    }
    
    glm::quat Transform::GetWorldRotation() const
    {
        return m_worldRotation;
    }
    
    glm::quat Transform::GetRelativeRotation() const
    {
        return m_relativeRotation;
    }
    
    glm::vec3 Transform::GetRotationEulerRad() const
    {
        return glm::eulerAngles(m_worldRotation);
    }
    
    glm::vec3 Transform::GetRotationEuler360() const
    {
        return DEGREES(glm::eulerAngles(m_worldRotation));
    }
    

    glm::vec3 Transform::GetLocalOYVector() const
    {
        return m_worldRotation * glm::vec3_up;
    }
    
    glm::vec3 Transform::GetLocalOXVector() const
    {
        return m_worldRotation * glm::vec3_right;
    }
    
    glm::vec3 Transform::GetLocalOZVector() const
    {
        return m_worldRotation * glm::vec3_forward;
    }
    

    glm::vec3 Transform::GetScale() const
    {
        return m_localScalingFactor;
    }
    
    const glm::mat4& Transform::GetModel()
    {
        if (m_isModelOutdated)
        {
            ComputeWorldModel();
        }
        return m_worldModel;
    }
    

    float Transform::GetMoveSpeed() const
    {
        return m_translationSpeed;
    }
    
    float Transform::GetScaleSpeed() const
    {
        return m_scalingSpeed;
    }
    
    float Transform::GetRotationSpeed() const
    {
        return m_rotationSpeed;
    }
    

    // ****************************
    // Continuous transform methods

    void Transform::Move(const glm::vec3 &offset)
    {
        SetWorldPosition(m_worldPosition + offset);

        UpdateWorldModel();
    }
    
    void Transform::Move(const glm::vec3 &dir, float deltaTime)
    {
        SetWorldPosition(m_worldPosition + m_translationSpeed * deltaTime * glm::normalize(dir));

        UpdateWorldModel();
    }
    
    void Transform::Scale(float deltaTime)
    {
        m_localScalingFactor += glm::vec3(m_scalingSpeed * deltaTime);

        UpdateWorldModel();
    }
    

    // Rotations
    void Transform::RotateWorldOX(float deltaTime)
    {
        const glm::vec3 angles = deltaTime * m_rotationSpeed * glm::vec3_right;
        const glm::quat worldRotation = glm::quat( RADIANS(angles) ) * m_worldRotation;

        SetWorldRotation(worldRotation);
    }
    
    void Transform::RotateWorldOY(float deltaTime)
    {
        const glm::vec3 angles = deltaTime * m_rotationSpeed * glm::vec3_up;
        const glm::quat worldRotation = glm::quat( RADIANS(angles) ) * m_worldRotation;

        SetWorldRotation(worldRotation);
    }
    
    void Transform::RotateWorldOZ(float deltaTime)
    {
        const glm::vec3 angles = deltaTime * m_rotationSpeed * glm::vec3_forward;
        const glm::quat worldRotation = glm::quat( RADIANS(angles) ) * m_worldRotation;

        SetWorldRotation(worldRotation);
    }
    
    void Transform::RotateLocalOX(float deltaTime)
    {
        const glm::vec3 angles = deltaTime * m_rotationSpeed * glm::vec3_right;
        const glm::quat worldRotation = m_worldRotation * glm::quat( RADIANS(angles) );

        SetWorldRotation(worldRotation);
    }
    
    void Transform::RotateLocalOY(float deltaTime)
    {
        const glm::vec3 angles = deltaTime * m_rotationSpeed * glm::vec3_up;
        const glm::quat worldRotation = m_worldRotation * glm::quat( RADIANS(angles) );

        SetWorldRotation(worldRotation);
    }
    
    void Transform::RotateLocalOZ(float deltaTime)
    {
        const glm::vec3 angles = deltaTime * m_rotationSpeed * glm::vec3_forward;
        const glm::quat worldRotation = m_worldRotation * glm::quat( RADIANS(angles) );

        SetWorldRotation(worldRotation);
    }
    

    // Positions
    void Transform::SetLocalPosition(glm::vec3 position)
    {
        m_localPosition = position;

        UpdateWorldPosition();
        UpdateChildrenPosition();
        UpdateModelPosition();
    }
    
    void Transform::SetWorldPosition(glm::vec3 position)
    {
        m_worldPosition = position;

        UpdateLocalPosition();
        UpdateChildrenPosition();
        UpdateModelPosition();
    }
    

    // Rotations
    void Transform::SetWorldRotation(glm::quat rotationQ)
    {
        m_worldRotation = rotationQ;
        m_invWorldRotation = glm::inverse(rotationQ);

        UpdateChildrenRotation();
        UpdateRelativeRotation();
        UpdateWorldModel();
    }
    
    void Transform::SetWorldRotation(const glm::vec3 &eulerAngles360)
    {
        SetWorldRotation(glm::quat( RADIANS(eulerAngles360) ));
    }
    
    void Transform::SetWorldRotationAndScale(const glm::quat &rotationQ, glm::vec3 scale)
    {
        m_localScalingFactor = scale;

        SetWorldRotation(rotationQ);
    }
    

    void Transform::SetReleativeRotation(const glm::vec3 &eulerAngles360)
    {
        SetReleativeRotation(glm::quat( RADIANS(eulerAngles360) ));
    }
    
    void Transform::SetReleativeRotation(const glm::quat &localRotationQ)
    {
        const glm::quat worldRotation = (m_parentNode == nullptr)
                                        ? localRotationQ
                                        : m_parentNode->m_worldRotation * localRotationQ;

        SetWorldRotation(worldRotation);
    }
    

    // Scales
    void Transform::SetScale(glm::vec3 scale)
    {
        m_localScalingFactor = scale;

        UpdateWorldModel();
    }
    

    // ****************************
    // Transform properties

    void Transform::SetMoveSpeed(float unitsPerSecond)
    {
        m_translationSpeed = unitsPerSecond;
    }
    
    void Transform::SetScaleSpeed(float unitsPerSecond)
    {
        m_scalingSpeed = unitsPerSecond;
    }
    
    void Transform::SetRotationSpeed(float degreesPerSecond)
    {
        m_rotationSpeed = degreesPerSecond;
    }
    

    // ****************************
    // Transform operations
    float Transform::DistanceTo(Transform *transform)
    {
        return glm::length(transform->GetWorldPosition() - m_worldPosition);
    }
    
    float Transform::DistanceTo(const glm::vec3 &position)
    {
        return glm::length(position - m_worldPosition);
    }
    
    float Transform::Distance2To(Transform *transform)
    {
        return glm::length2(transform->GetWorldPosition() - m_worldPosition);
    }
    
    float Transform::Distance2To(const glm::vec3 &position)
    {
        return glm::length2(position - m_worldPosition);
    }
    

    // ****************************
    // Hierarchy chain

    void Transform::AddChild(Transform *transform)
    {
        m_childNodes.push_back(transform);

        transform->m_parentNode = this;
        // A very idiotic way of updating the child's internal fields...
        transform->SetWorldPosition(transform->m_worldPosition);
        transform->SetWorldRotation(transform->m_worldRotation);
    }
    
    void Transform::RemoveChild(Transform *transform)
    {
        (void)(m_childNodes.remove(transform));

        transform->m_parentNode = nullptr;
        transform->SetWorldPosition(transform->m_worldPosition);
        transform->SetWorldRotation(transform->m_worldRotation);
    }
    

    void Transform::Init()
    {
        m_relativeRotation = glm::quat();
        m_parentNode = nullptr;
        m_isInMotion = false;
        m_isModelOutdated = true;
        m_updateHierarchy = true;

        UpdateWorldModel();
    }
    

    // ****************************
    // Should only be called by the owner

    void Transform::ComputeWorldModel()
    {
        m_worldModel = glm::mat4(1.0F);
        // Not sure if translate was called like this or something like this was used
        // m_worldModel[3] = glm::vec4(0.0F, 0.0F, 0.0F, 1.0F);
        m_worldModel = glm::translate(m_worldModel, glm::vec3(0.0F));
        m_worldModel *= glm::toMat4(m_worldRotation);
        m_worldModel = glm::scale(m_worldModel, m_localScalingFactor);

        UpdateModelPosition();

        m_isModelOutdated = false;
    }
    
    void Transform::UpdateWorldModel()
    {
        m_isInMotion = true;
        m_isModelOutdated = true;
    }
    
    void Transform::UpdateWorldPosition()
    {
        const glm::vec3 pos = (m_parentNode == nullptr)
                              ? m_localPosition
                              : m_parentNode->m_worldPosition + m_parentNode->m_worldRotation * m_localPosition;
        m_worldPosition = pos;
    }
    
    void Transform::UpdateLocalPosition()
    {
        const glm::vec3 pos = (m_parentNode == nullptr)
                              ? m_worldPosition
                              : m_parentNode->m_invWorldRotation * (m_worldPosition - m_parentNode->m_worldPosition);
        m_localPosition = pos;
    }
    
    void Transform::UpdateRelativeRotation()
    {
        const glm::quat inv_rot = (m_parentNode == nullptr)
                                  ? glm::quat()
                                  : m_parentNode->m_invWorldRotation;
        m_relativeRotation = inv_rot * m_worldRotation;
    }
    

    void Transform::UpdateWorldInfo()
    {
        // No null checking for the parent is done here... This will break if the
        // function is called for the root transform.
        m_worldPosition = m_parentNode->m_worldRotation * m_localPosition;
        m_worldRotation = m_parentNode->m_worldRotation * m_relativeRotation;
        m_invWorldRotation = glm::inverse(m_worldRotation);

        UpdateChildrenRotation();
        UpdateWorldModel();
    }
    
    void Transform::UpdateChildrenPosition()
    {
        if (m_updateHierarchy)
        {
            for (Transform *child : m_childNodes)
            {
                child->SetLocalPosition(child->m_localPosition);
            }
        }
    }
    
    void Transform::UpdateChildrenRotation()
    {
        if (m_updateHierarchy)
        {
            for (Transform *child : m_childNodes)
            {
                child->UpdateWorldInfo();
            }
        }
    }
    

    void Transform::UpdateModelPosition()
    {
        // Beats me why it is done like this. Maybe it is an inlined glm function.
        m_worldModel[3][0] = m_worldPosition.x;
        m_worldModel[3][1] = m_worldPosition.y;
        m_worldModel[3][2] = m_worldPosition.z;

        m_isInMotion = true;
    }
}

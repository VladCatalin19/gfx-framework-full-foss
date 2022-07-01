#include "transform_wrapper.h"
#include "components/transform.h"
#include "transform_internals.h"

#include "utils/glm_utils.h"
#include "utils/math_utils.h"

#include <ios>
#include <iostream>

static std::ostream& operator<<(std::ostream &os, const std::list<gfxc::Transform*> &children)
{
    auto remainingChildren = children.size();
    os << "[";
    for (gfxc::Transform *child : children)
    {
        os << child;

        --remainingChildren;
        if (remainingChildren > 0U)
        {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

static void PrintInternalState(const gfxc::TransformInternals &internals, const unsigned indent)
{
    std::string indentStr(indent, ' ');

    std::cout << indentStr << "Model matrix: " << internals.m_worldModel[0] << "\n"
              << indentStr << "              " << internals.m_worldModel[1] << "\n"
              << indentStr << "              " << internals.m_worldModel[2] << "\n"
              << indentStr << "              " << internals.m_worldModel[3] << "\n"
              << "\n"
              << indentStr << "World rotation: " << internals.m_worldRotation << " " << glm::eulerAngles(internals.m_worldRotation) << "\n"
              << indentStr << "Relative rotation: " << internals.m_relativeRotation << " " << glm::eulerAngles(internals.m_relativeRotation) << "\n"
              << indentStr << "Inverse world rotation: " << internals.m_invWorldRotation << " " << glm::eulerAngles(internals.m_invWorldRotation) << "\n"
              << "\n"
              << indentStr << "World position: " << internals.m_worldPosition << "\n"
              << indentStr << "Local position: " << internals.m_localPosition << "\n"
              << "\n"
              << indentStr << "Local scale: " << internals.m_localScalingFactor << "\n"
              << "\n"
              << indentStr << "Rotation speed: " << internals.m_rotationSpeed << "\n"
              << indentStr << "Translation speed: " << internals.m_translationSpeed << "\n"
              << indentStr << "Scaling speed: " << internals.m_scalingSpeed << "\n"
              << "\n"
              << std::boolalpha
              << indentStr << "Is in motion: " << internals.m_isInMotion << "\n"
              << indentStr << "Is model outdated: " << internals.m_isModelOutdated << "\n"
              << indentStr << "Update hierarchy: " << internals.m_updateHierarchy << "\n"
              << std::noboolalpha
              << "\n"
              << indentStr << "Parent: " << internals.m_parentNode << "\n"
              << indentStr << "Children: " << internals.m_childNodes << std::endl;
}

static void PrintDifferences(const gfxc::TransformInternals &i1, const gfxc::TransformInternals &i2, const unsigned indent)
{
    std::string indentStr(indent, ' ');
    indentStr += "-->";

    if (i1.m_worldModel != i2.m_worldModel)
    {
        std::cout << indentStr << "Model matrix has changed\n"
                  << indentStr << "    Before: " << i1.m_worldModel[0] << "\n"
                  << indentStr << "            " << i1.m_worldModel[1] << "\n"
                  << indentStr << "            " << i1.m_worldModel[2] << "\n"
                  << indentStr << "            " << i1.m_worldModel[3] << "\n"
                  << indentStr << "    After: " << i2.m_worldModel[0] << "\n"
                  << indentStr << "           " << i2.m_worldModel[1] << "\n"
                  << indentStr << "           " << i2.m_worldModel[2] << "\n"
                  << indentStr << "           " << i2.m_worldModel[3] << "\n";
    }

    if (i1.m_worldRotation != i2.m_worldRotation)
    {
        std::cout << indentStr << "World rotation has changed\n"
                  << indentStr << "    Before: " << i1.m_worldRotation << " " << glm::eulerAngles(i1.m_worldRotation) << "\n"
                  << indentStr << "    After: " << i2.m_worldRotation << " " << glm::eulerAngles(i2.m_worldRotation) << "\n";
    }
    if (i1.m_relativeRotation != i2.m_relativeRotation)
    {
        std::cout << indentStr << "Relative rotation has changed\n"
                  << indentStr << "    Before: " << i1.m_relativeRotation << " " << glm::eulerAngles(i1.m_relativeRotation) << "\n"
                  << indentStr << "    After: " << i2.m_relativeRotation << " " << glm::eulerAngles(i2.m_relativeRotation) << "\n";
    }
    if (i1.m_invWorldRotation != i2.m_invWorldRotation)
    {
        std::cout << indentStr << "Inverse world rotation has changed\n"
                  << indentStr << "    Before: " << i1.m_invWorldRotation << " " << glm::eulerAngles(i1.m_invWorldRotation) << "\n"
                  << indentStr << "    After: " << i2.m_invWorldRotation << " " << glm::eulerAngles(i2.m_invWorldRotation) << "\n";
    }

    if (i1.m_worldPosition != i2.m_worldPosition)
    {
        std::cout << indentStr << "World position has changed\n"
                  << indentStr << "    Before: " << i1.m_worldPosition << "\n"
                  << indentStr << "    After: " << i2.m_worldPosition << "\n";
    }
    if (i1.m_localPosition != i2.m_localPosition)
    {
        std::cout << indentStr << "Local position has changed\n"
                  << indentStr << "    Before: " << i1.m_localPosition << "\n"
                  << indentStr << "    After: " << i2.m_localPosition << "\n";
    }

    if (i1.m_localScalingFactor != i2.m_localScalingFactor)
    {
        std::cout << indentStr << "Local scaling factor has changed\n"
                  << indentStr << "    Before: " << i1.m_localScalingFactor << "\n"
                  << indentStr << "    After: " << i2.m_localScalingFactor << "\n";
    }

    if (i1.m_rotationSpeed != i2.m_rotationSpeed)
    {
        std::cout << indentStr << "Rotation speed has changed\n"
                  << indentStr << "    Before: " << i1.m_rotationSpeed << "\n"
                  << indentStr << "    After: " << i2.m_rotationSpeed << "\n";
    }
    if (i1.m_translationSpeed != i2.m_translationSpeed)
    {
        std::cout << indentStr << "Translation speed has changed\n"
                  << indentStr << "    Before: " << i1.m_translationSpeed << "\n"
                  << indentStr << "    After: " << i2.m_translationSpeed << "\n";
    }
    if (i1.m_scalingSpeed != i2.m_scalingSpeed)
    {
        std::cout << indentStr << "Scaling speed has changed\n"
                  << indentStr << "    Before: " << i1.m_scalingSpeed << "\n"
                  << indentStr << "    After: " << i2.m_scalingSpeed << "\n";
    }

    std::cout << std::boolalpha;
    if (i1.m_isInMotion != i2.m_isInMotion)
    {
        std::cout << indentStr << "Is in motion has changed\n"
                  << indentStr << "    Before: " << i1.m_isInMotion << "\n"
                  << indentStr << "    After: " << i2.m_isInMotion << "\n";
    }
    if (i1.m_isModelOutdated != i2.m_isModelOutdated)
    {
        std::cout << indentStr << "Is model outdated has changed\n"
                  << indentStr << "    Before: " << i1.m_isModelOutdated << "\n"
                  << indentStr << "    After: " << i2.m_isModelOutdated << "\n";
    }
    if (i1.m_updateHierarchy != i2.m_updateHierarchy)
    {
        std::cout << indentStr << "Update hierarchy has changed\n"
                  << indentStr << "    Before: " << i1.m_updateHierarchy << "\n"
                  << indentStr << "    After: " << i2.m_updateHierarchy << "\n";
    }
    std::cout << std::noboolalpha;

    if (i1.m_parentNode != i2.m_parentNode)
    {
        std::cout << indentStr << "Parent has changed\n"
                  << indentStr << "    Before: " << i1.m_parentNode << "\n"
                  << indentStr << "    After: " << i2.m_parentNode << "\n";
    }

    if (i1.m_childNodes != i2.m_childNodes)
    {
        std::cout << indentStr << "Child nodes have changed:\n"
                  << indentStr << "    Before: " << i1.m_childNodes << "\n"
                  << indentStr << "    After: " << i2.m_childNodes << "\n";
    }
    std::flush(std::cout);
}

namespace gfxc
{
    TransformWrapper::TransformWrapper()
        : Transform(),
          m_printIndent(0U),
          m_printIndentIncrement(4U)
    {
        std::cout << "Constructor called" << std::endl;
        PrintInternalState(*this, 0U);
    }

    TransformWrapper::~TransformWrapper()
    {
        std::cout << "Destructor called" << std::endl;
    }
    

    TransformWrapper::TransformWrapper(const TransformWrapper &other)
        : Transform(other)
    {
        std::cout << "Copy constructor called" << std::endl;
        PrintInternalState(*this, 0U);
    }
    
    
    glm::vec3 TransformWrapper::GetLocalPosition() const
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;
        return Transform::GetLocalPosition();
    }
    
    glm::vec3 TransformWrapper::GetWorldPosition() const
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;
        return Transform::GetWorldPosition();
    }
    
    glm::quat TransformWrapper::GetWorldRotation() const
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;
        return Transform::GetWorldRotation();
    }
    
    glm::quat TransformWrapper::GetRelativeRotation() const
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;
        return Transform::GetRelativeRotation();
    }
    
    glm::vec3 TransformWrapper::GetRotationEulerRad() const
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;
        return Transform::GetRotationEulerRad();
    }
    
    glm::vec3 TransformWrapper::GetRotationEuler360() const
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;
        return Transform::GetRotationEuler360();
    }
    

    glm::vec3 TransformWrapper::GetLocalOYVector() const
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;
        return Transform::GetLocalOYVector();
    }
    
    glm::vec3 TransformWrapper::GetLocalOXVector() const
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;
        return Transform::GetLocalOXVector();
    }
    
    glm::vec3 TransformWrapper::GetLocalOZVector() const
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;
        return Transform::GetLocalOZVector();
    }
    

    glm::vec3 TransformWrapper::GetScale() const
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;
        return Transform::GetScale();
    }
    
    const glm::mat4& TransformWrapper::GetModel()
    {
        //std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;
        
        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        (void)Transform::GetModel();
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
        return Transform::m_worldModel;
    }
    

    float TransformWrapper::GetMoveSpeed() const
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;
        return Transform::GetMoveSpeed();
    }
    
    float TransformWrapper::GetScaleSpeed() const
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;
        return Transform::GetScaleSpeed();
    }
    
    float TransformWrapper::GetRotationSpeed() const
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;
        return Transform::GetRotationSpeed();
    }
    

    // ****************************
    // Continuous transform methods

    void TransformWrapper::Move(const glm::vec3 &offset)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::Move(offset);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::Move(const glm::vec3 &dir, float deltaTime)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::Move(dir, deltaTime);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::Scale(float deltaTime)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::Scale(deltaTime);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    

    // Rotations
    void TransformWrapper::RotateWorldOX(float deltaTime)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::RotateWorldOX(deltaTime);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::RotateWorldOY(float deltaTime)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::RotateWorldOY(deltaTime);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::RotateWorldOZ(float deltaTime)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::RotateWorldOZ(deltaTime);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::RotateLocalOX(float deltaTime)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::RotateLocalOX(deltaTime);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::RotateLocalOY(float deltaTime)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::RotateLocalOY(deltaTime);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::RotateLocalOZ(float deltaTime)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::RotateLocalOZ(deltaTime);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    

    // Positions
    void TransformWrapper::SetLocalPosition(glm::vec3 position)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::SetLocalPosition(position);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::SetWorldPosition(glm::vec3 position)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::SetWorldPosition(position);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    

    // Rotations
    void TransformWrapper::SetWorldRotation(glm::quat rotationQ)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::SetWorldRotation(rotationQ);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::SetWorldRotation(const glm::vec3 &eulerAngles360)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::SetWorldRotation(eulerAngles360);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::SetWorldRotationAndScale(const glm::quat &rotationQ, glm::vec3 scale)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::SetWorldRotationAndScale(rotationQ, scale);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    

    void TransformWrapper::SetReleativeRotation(const glm::vec3 &eulerAngles360)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::SetReleativeRotation(eulerAngles360);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::SetReleativeRotation(const glm::quat &localRotationQ)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::SetReleativeRotation(localRotationQ);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    

    // Scales
    void TransformWrapper::SetScale(glm::vec3 scale)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::SetScale(scale);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    

    // ****************************
    // Transform properties

    void TransformWrapper::SetMoveSpeed(float unitsPerSecond)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::SetMoveSpeed(unitsPerSecond);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::SetScaleSpeed(float unitsPerSecond)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::SetScaleSpeed(unitsPerSecond);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::SetRotationSpeed(float degreesPerSecond)
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::SetRotationSpeed(degreesPerSecond);
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    

    void TransformWrapper::Init()
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::Init();
        m_printIndent -= m_printIndentIncrement;
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    

    // ****************************
    // Should only be called by the owner

    void TransformWrapper::ComputeWorldModel()
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::ComputeWorldModel();
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::UpdateWorldModel()
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::UpdateWorldModel();
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::UpdateWorldPosition()
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::UpdateWorldPosition();
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::UpdateLocalPosition()
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::UpdateLocalPosition();
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::UpdateRelativeRotation()
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::UpdateRelativeRotation();
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    

    void TransformWrapper::UpdateWorldInfo()
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::UpdateWorldInfo();
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::UpdateChildrenPosition()
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::UpdateChildrenPosition();
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    
    void TransformWrapper::UpdateChildrenRotation()
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::UpdateChildrenRotation();
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
    

#if false
    void TransformWrapper::UpdateModelPosition()
    {
        std::cout << std::string(m_printIndent, ' ') << "-----" << __func__ << "-----" << " called" << std::endl;

        TransformInternals prevState(*this);
        m_printIndent += m_printIndentIncrement;
        Transform::UpdateModelPosition();
        TransformInternals currState(*this);

        PrintDifferences(prevState, currState, m_printIndent);
    }
#endif
}

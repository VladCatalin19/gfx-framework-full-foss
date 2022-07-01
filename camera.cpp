#include "camera.h"
#include "glm/ext/matrix_transform.hpp"
#include "transform.h"

#include "utils/glm_utils.h"

#include <iostream>

namespace gfxc
{
    Camera::Camera()
        : m_transform(new Transform())
    {
        Init();
    }
    
    Camera::~Camera()
    {

    }
    

    void Camera::Init()
    {
        m_type = CameraType::FIRST_PERSON;
        m_zNear = 0.01F;
        m_zFar = 50.0F;
        m_FoVy = 40.0F;
        m_aspectRatio = 1.6F;
        m_minSpeed = 0.1F;
        m_maxSpeed = 100.0F;
        m_sensitivityOX = 0.002F;
        m_sensitivityOY = 0.002F;

        m_transform->SetMoveSpeed(20.0F);
        m_transform->SetRotationSpeed(50.0F);
        m_transform->SetScale(glm::vec3(0.1F, 0.1F, 0.1F));

        Update();
    }
    
    void Camera::Log() const
    {
        (void)(std::cout << "Camera =>" << std::endl
                         << "Rotation: " << m_transform->GetRotationEulerRad() << std::endl
                         << "Position: " << m_transform->GetWorldPosition() << std::endl
                         << "Forward : " << m_transform->GetLocalOZVector() << std::endl
                         << "Up      : " << m_transform->GetLocalOYVector() << std::endl
                         << "------------------------------------------------" << std::endl);
    }
    
    void Camera::Update()
    {
        if (m_transform->GetMotionState())
        {
            const glm::vec3 eye = m_transform->GetWorldPosition();
            const glm::vec3 center = eye - m_transform->GetLocalOZVector();
            const glm::vec3 up = m_transform->GetLocalOYVector();

            m_view = glm::lookAt(eye, center, up);
        }
    }
    

    const glm::mat4& Camera::GetViewMatrix() const
    {
        return m_view;
    }
    
    const glm::mat4& Camera::GetProjectionMatrix() const
    {
        return m_projection;
    }
    

    // Rotation
    void Camera::RotateOX(float deltaTime)
    {
        if (deltaTime != 0.0F)
        {
            m_transform->RotateLocalOX(deltaTime * m_sensitivityOX);
        }
    }
    
    void Camera::RotateOY(float deltaTime)
    {
        if (deltaTime != 0.0F)
        {
            m_transform->RotateWorldOY(deltaTime * m_sensitivityOY);
        }
    }
    
    void Camera::RotateOZ(float deltaTime)
    {
        // Function was empty in assembly
        (void)deltaTime;
    }
    

    // Speed
    void Camera::UpdateSpeed(float offset)
    {
        float speed = m_transform->GetMoveSpeed();
        if ( (m_minSpeed < (speed + offset)) and ((speed + offset) > m_maxSpeed) )
        {
            m_transform->SetMoveSpeed(speed + offset);
        }
    }
    

    // Translate Camera - local axis
    void Camera::SetPosition(const glm::vec3 &position)
    {
        m_transform->SetWorldPosition(position);
        Update();
    }
    
    void Camera::SetRotation(const glm::quat &worldRotation)
    {
        m_transform->SetWorldRotation(worldRotation);
        Update();
    }
    
    void Camera::SetPositionAndRotation(const glm::vec3 &position, const glm::quat &worldRotation)
    {
        m_transform->SetWorldPosition(position);
        m_transform->SetWorldRotation(worldRotation);
        Update();
    }
    

    // Translate Camera - world axis
    void Camera::MoveForward(float deltaTime)
    {
        m_transform->Move(glm::normalize(-m_transform->GetLocalOZVector()), deltaTime);
    }
    
    void Camera::MoveBackward(float deltaTime)
    {
        m_transform->Move(glm::normalize(m_transform->GetLocalOZVector()), deltaTime);
    }
    
    void Camera::MoveRight(float deltaTime)
    {
        m_transform->Move(m_transform->GetLocalOXVector(), deltaTime);
    }
    
    void Camera::MoveLeft(float deltaTime)
    {
        m_transform->Move(-m_transform->GetLocalOXVector(), deltaTime);
    }
    
    void Camera::MoveUp(float deltaTime)
    {
        m_transform->Move(glm::vec3_up, deltaTime);
    }
    
    void Camera::MoveDown(float deltaTime)
    {
        m_transform->Move(glm::vec3_down, deltaTime);
    }
    
    void Camera::MoveInDirection(glm::vec3 direction, float deltaTime)
    {
        m_transform->Move(direction, deltaTime);
    }
    

    void Camera::SetPerspective(float FoVy, float aspectRatio, float zNear, float zFar)
    {
        m_isPerspective = true;
        m_FoVy = FoVy;
        m_aspectRatio = aspectRatio;
        m_zNear = zNear;
        m_zFar = zFar;

        m_projection = glm::perspective(RADIANS(FoVy), aspectRatio, zNear, zFar);
    }
    
    void Camera::SetOrthographic(float width, float height, float zNear, float zFar)
    {
        m_isPerspective = false;
        m_ortographicWidth = width;
        m_zNear = zNear;
        m_zFar = zFar;
        m_aspectRatio = width / height;

        m_projection = glm::ortho(-width / 2, width / 2, -height / 2, height / 2, zNear, zFar);
    }
    
    void Camera::SetOrthographic(float left, float right, float bottom, float top, float zNear, float zFar)
    {
        m_isPerspective = false;
        m_ortographicWidth = right - left;
        m_zNear = zNear;
        m_zFar = zFar;
        m_aspectRatio = (right - left) / (top - bottom);

        m_projection = glm::ortho(left, right, bottom, top, zNear, zFar);
    }
    

    void Camera::SetProjection(const ProjectionInfo& projInfo)
    {
        if (projInfo.isPerspective)
        {
            SetPerspective(projInfo.FoVy, projInfo.aspectRatio, projInfo.zNear, projInfo.zFar);
        }
        else
        {
            SetOrthographic(projInfo.width, projInfo.height, projInfo.zNear, projInfo.zFar);
        }
    }
    
    ProjectionInfo Camera::GetProjectionInfo() const
    {
        ProjectionInfo info;
        info.FoVy = m_FoVy;
        info.zNear = m_zNear;
        info.zFar = m_zFar;
        info.aspectRatio = m_aspectRatio;
        info.width = m_ortographicWidth;
        info.height = m_ortographicWidth / m_aspectRatio;
        info.isPerspective = m_isPerspective;
        return info;
    }
    

    float Camera::GetFieldOfViewY() const
    {
        return m_FoVy;
    }
    
    float Camera::GetFieldOfViewX() const
    {
        return m_aspectRatio * m_FoVy;
    }
    

    // FPS Rotation Mode
    void Camera::UpdateRoll(float deltaAngle)
    {
        m_transform->RotateLocalOZ(deltaAngle);
    }
    
    void Camera::UpdateYaw(float deltaAngle)
    {
        m_transform->RotateWorldOY(deltaAngle);
    }
    
    void Camera::UpdatePitch(float deltaAngle)
    {
        m_transform->RotateLocalOX(deltaAngle);
    }
}

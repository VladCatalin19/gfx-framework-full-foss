#ifndef GFXC_TRANSFORM_WRAPPER_H
#define GFXC_TRANSFORM_WRAPPER_H

#include "transform.h"

namespace gfxc
{
    class TransformWrapper : public Transform
    {
    public:
        TransformWrapper();
        virtual ~TransformWrapper();

        TransformWrapper(const TransformWrapper &other);

        TransformWrapper(TransformWrapper &&other) = default;
        TransformWrapper& operator=(const TransformWrapper &other) = default;
        TransformWrapper& operator=(TransformWrapper &&other) = default;

        
        virtual glm::vec3 GetLocalPosition() const override;
        virtual glm::vec3 GetWorldPosition() const override;
        virtual glm::quat GetWorldRotation() const override;
        virtual glm::quat GetRelativeRotation() const override;
        virtual glm::vec3 GetRotationEulerRad() const override;
        virtual glm::vec3 GetRotationEuler360() const override;

        virtual glm::vec3 GetLocalOYVector() const override;
        virtual glm::vec3 GetLocalOXVector() const override;
        virtual glm::vec3 GetLocalOZVector() const override;

        virtual glm::vec3 GetScale() const override;
        virtual const glm::mat4& GetModel() override;

        virtual float GetMoveSpeed() const override;
        virtual float GetScaleSpeed() const override;
        virtual float GetRotationSpeed() const override;

        // ****************************
        // Continuous transform methods

        virtual void Move(const glm::vec3 &offset) override;
        virtual void Move(const glm::vec3 &dir, float deltaTime) override;
        virtual void Scale(float deltaTime) override;

        // Rotations
        virtual void RotateWorldOX(float deltaTime) override;
        virtual void RotateWorldOY(float deltaTime) override;
        virtual void RotateWorldOZ(float deltaTime) override;
        virtual void RotateLocalOX(float deltaTime) override;
        virtual void RotateLocalOY(float deltaTime) override;
        virtual void RotateLocalOZ(float deltaTime) override;

        // Positions
        virtual void SetLocalPosition(glm::vec3 position) override;
        virtual void SetWorldPosition(glm::vec3 position) override;

        // Rotations
        virtual void SetWorldRotation(glm::quat rotationQ) override;
        virtual void SetWorldRotation(const glm::vec3 &eulerAngles360) override;
        virtual void SetWorldRotationAndScale(const glm::quat &rotationQ, glm::vec3 scale) override;

        virtual void SetReleativeRotation(const glm::vec3 &eulerAngles360) override;
        virtual void SetReleativeRotation(const glm::quat &localRotationQ) override;

        // Scales
        virtual void SetScale(glm::vec3 scale) override;

        // ****************************
        // Transform properties

        virtual void SetMoveSpeed(float unitsPerSecond) override;
        virtual void SetScaleSpeed(float unitsPerSecond) override;
        virtual void SetRotationSpeed(float degreesPerSecond) override;

        virtual void Init() override;

        // ****************************
        // Should only be called by the owner

        virtual void ComputeWorldModel() override;
        virtual void UpdateWorldModel() override;
        virtual void UpdateWorldPosition() override;
        virtual void UpdateLocalPosition() override;
        virtual void UpdateRelativeRotation() override;

        virtual void UpdateWorldInfo() override;
        virtual void UpdateChildrenPosition() override;
        virtual void UpdateChildrenRotation() override;


        //virtual void UpdateModelPosition() override;

    private:
        unsigned m_printIndent;
        unsigned m_printIndentIncrement;
    };
}

#endif // GFXC_TRANSFORM_WRAPPER_H
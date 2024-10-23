

/*
 * Copyright (c) 2024. Combat Jongerenmarketing en -communicatie B.V
 * All rights reserved.
 */

#include "BloxxEngine/Camera.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>

namespace BloxxEngine
{
Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch)
    : Position(pos),
      Front(glm::vec3(0.0f, 0.0f, -1.0f)),
      WorldUp(up),
      Yaw(yaw),
      Pitch(pitch),
      MovementSpeed(6.f),
      MouseSensitivity(0.01f),
      ZoomFactor(45.0f)
{
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::Move(const Movement direction, const float deltaTime)
{
    const float velocity = MovementSpeed * deltaTime;

    switch (direction)
    {
    case Movement::Forward:
        Position += Front * velocity;
        break;
    case Movement::Backward:
        Position -= Front * velocity;
        break;
    case Movement::Left:
        Position -= Right * velocity;
        break;
    case Movement::Right:
        Position += Right * velocity;
        break;
    case Movement::Up:
        Position += Up * velocity;
        break;
    case Movement::Down:
        Position -= Up * velocity;
        break;
    }
}

void Camera::Rotate(float xOffset, float yOffset, bool constrainPitch)
{
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;

    Yaw += xOffset;
    Pitch += yOffset;

    // Constrain the pitch angle to prevent screen flip
    if (constrainPitch)
    {
        if (Pitch > 89.0)
            Pitch = 89.0;
        if (Pitch < -89.0)
            Pitch = -89.0;
    }

    UpdateCameraVectors();
}

void Camera::Zoom(float yOffset)
{
    ZoomFactor -= yOffset;

    if (ZoomFactor < 1.0f)
        ZoomFactor = 1.0f;
    if (ZoomFactor > 45.0f)
        ZoomFactor = 45.0f;
}


void Camera::UpdateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // Recalculate Right and Up vectors
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
} // namespace BloxxEngine
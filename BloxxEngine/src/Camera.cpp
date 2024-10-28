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
      MovementSpeed(5.f),
      MouseSensitivity(0.01f),
      ZoomFactor(45.0f)
{
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::Move(const bool forward, const bool backward,
                  const bool left, const bool right,
                  const bool up, const bool down,
                  const float deltaTime)
{
    glm::vec3 direction(0.0f);

    if (forward)
        direction += Front;
    if (backward)
        direction -= Front;
    if (left)
        direction -= Right;
    if (right)
        direction += Right;
    if (up)
        direction += Up;
    if (down)
        direction -= Up;

    if (glm::length(direction) > 0.0f)
        direction = glm::normalize(direction);

    float velocity = MovementSpeed * deltaTime;
    Position += direction * velocity;
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
#pragma once

#include "raylab/Node.h"

#include <blueprint/Pin.h>

namespace raylab
{
namespace node
{

class Camera : public Node
{
public:
    Camera(const std::string& title)
        : Node(title)
    {
        AddPin(std::make_shared<bp::Pin>(false, 0, bp::PIN_ANY_VAR, "Out", *this));

        Layout();
    }

    sm::vec3 pos;
    sm::vec3 target;

    RTTR_ENABLE(Node)

}; // Camera

class Pinhole : public Camera
{
public:
    Pinhole() : Camera("Pinhole") {}

    float dis = 0.0f;
    float zoom = 1.0f;

    RTTR_ENABLE(Camera)

}; // Pinhole

}
}
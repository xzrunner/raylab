#pragma once

#include "raylab/Node.h"

#include <blueprint/Pin.h>

namespace raylab
{
namespace node
{

class Light : public Node
{
public:
    Light(const std::string& title)
        : Node(title)
    {
        AddPin(std::make_shared<bp::Pin>(false, 0, bp::PIN_ANY_VAR, "Out", *this));

        Layout();
    }

    RTTR_ENABLE(Node)

}; // Light

class Ambient : public Light
{
public:
    Ambient() : Light("Ambient") {}

    float      radiant_scale = 1.0f;
    pt0::Color color = pt0::Color(1.0f, 1.0f, 1.0f);

    RTTR_ENABLE(Light)

}; // Ambient

class PointLight : public Light
{
public:
    PointLight() : Light("PointLight") {}

    float      radiant_scale = 1.0f;
    pt0::Color color = pt0::Color(1.0f, 1.0f, 1.0f);
    sm::vec3   location;

    RTTR_ENABLE(Light)

}; // PointLight

}
}
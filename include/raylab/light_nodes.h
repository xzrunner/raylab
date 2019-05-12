#pragma once

#include "raylab/Node.h"
#include "raylab/PinType.h"

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
        AddPin(std::make_shared<bp::Pin>(false, 0, PIN_LIGHT, "Out", *this));

        Layout();
    }

    bool shadows = true;

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

class Directional : public Light
{
public:
    Directional() : Light("Directional") {}

    float      radiant_scale = 1.0f;
    pt0::Color color = pt0::Color(1.0f, 1.0f, 1.0f);
    sm::vec3   dir;

    RTTR_ENABLE(Light)

}; // Directional

class PointLight : public Light
{
public:
    PointLight() : Light("PointLight") {}

    float      radiant_scale = 1.0f;
    pt0::Color color = pt0::Color(1.0f, 1.0f, 1.0f);
    sm::vec3   location;

    RTTR_ENABLE(Light)

}; // PointLight

class AreaLight : public Light
{
public:
    AreaLight()
        : Light("AreaLight")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_OBJECT, "Obj", *this));

        Layout();
    }

    RTTR_ENABLE(Light)

}; // AreaLight

class AmbientOccluder : public Light
{
public:
    AmbientOccluder()
        : Light("AmbientOccluder")
    {
        AddPin(std::make_shared<bp::Pin>(true, 0, PIN_SAMPLER, "Sampler", *this));

        Layout();
    }

    float scale_radiance = 1.0f;
    float min_amount = 0;

    RTTR_ENABLE(Light)

}; // AmbientOccluder

}
}
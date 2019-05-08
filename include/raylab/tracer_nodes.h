#pragma once

#include "raylab/Node.h"

#include <blueprint/Pin.h>

namespace raylab
{
namespace node
{

class Tracer : public Node
{
public:
    Tracer(const std::string& title)
        : Node(title)
    {
        AddPin(std::make_shared<bp::Pin>(false, 0, bp::PIN_ANY_VAR, "Out", *this));

        Layout();
    }

    RTTR_ENABLE(Node)

}; // Tracer

class RayCast : public Tracer
{
public:
    RayCast() : Tracer("RayCast") {}

    RTTR_ENABLE(Tracer)

}; // RayCast

class AreaLighting : public Tracer
{
public:
    AreaLighting() : Tracer("AreaLighting") {}

    RTTR_ENABLE(Tracer)

}; // AreaLighting

}
}
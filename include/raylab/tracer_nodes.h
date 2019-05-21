#pragma once

#include "raylab/Node.h"
#include "raylab/PinType.h"

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
        AddPin(std::make_shared<bp::Pin>(false, 0, PIN_TRACER, "Out", *this));

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

class FirstHit : public Tracer
{
public:
    FirstHit() : Tracer("FirstHit") {}

    RTTR_ENABLE(Tracer)

}; // FirstHit

class Whitted : public Tracer
{
public:
    Whitted() : Tracer("Whitted") {}

    RTTR_ENABLE(Tracer)

}; // Whitted

class PathTrace : public Tracer
{
public:
    PathTrace() : Tracer("PathTrace") {}

    RTTR_ENABLE(Tracer)

}; // PathTrace

}
}
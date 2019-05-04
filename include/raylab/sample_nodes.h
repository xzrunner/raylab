#pragma once

#include "raylab/Node.h"

#include <blueprint/Pin.h>

namespace raylab
{
namespace node
{

class Sampler : public Node
{
public:
    Sampler(const std::string& title)
        : Node(title)
    {
        AddPin(std::make_shared<bp::Pin>(false, 0, bp::PIN_ANY_VAR, "Out", *this));

        Layout();
    }

    RTTR_ENABLE(Node)

}; // Sampler

class Jittered : public Sampler
{
public:
    Jittered() : Sampler("Jittered") {}

    RTTR_ENABLE(Sampler)

}; // Jittered

class MultiJittered : public Sampler
{
public:
    MultiJittered() : Sampler("MultiJittered") {}

    RTTR_ENABLE(Sampler)

}; // MultiJittered

class Regular : public Sampler
{
public:
    Regular() : Sampler("Regular") {}

    RTTR_ENABLE(Sampler)

}; // Regular

}
}
#pragma once

#include "raylab/Node.h"
#include "raylab/PinType.h"

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
        AddPin(std::make_shared<bp::Pin>(true, 0, bp::PIN_INTEGER, "Num Samples", *this));
        AddPin(std::make_shared<bp::Pin>(false, 0, PIN_SAMPLER, "Out", *this));

        Layout();
    }

    int num_samples = 1;

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

class PureRandom : public Sampler
{
public:
    PureRandom() : Sampler("PureRandom") {}

    RTTR_ENABLE(Sampler)

}; // PureRandom

class Hammersley : public Sampler
{
public:
    Hammersley() : Sampler("Hammersley") {}

    RTTR_ENABLE(Sampler)

}; // Hammersley

}
}
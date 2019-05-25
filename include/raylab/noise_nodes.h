#pragma once

#include "raylab/Node.h"

namespace raylab
{
namespace node
{

class LatticeNoise : public Node
{
public:
    LatticeNoise(const std::string& title)
        : Node(title)
    {
        AddPin(std::make_shared<bp::Pin>(false, 0, PIN_NOISE, "Out", *this));

        Layout();
    }

    int   octaves    = 1;
    float lacunarity = 2.0f;
    float gain       = 0.5f;

    RTTR_ENABLE(Node)

}; // LatticeNoise

class LinearNoise : public LatticeNoise
{
public:
    LinearNoise() : LatticeNoise("LinearNoise") {}

    RTTR_ENABLE(LatticeNoise)

}; // LinearNoise

class CubicNoise : public LatticeNoise
{
public:
    CubicNoise() : LatticeNoise("CubicNoise") {}

    RTTR_ENABLE(LatticeNoise)

}; // CubicNoise

}
}
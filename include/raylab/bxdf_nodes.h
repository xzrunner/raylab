#pragma once

#include "raylab/Node.h"

#include <blueprint/Pin.h>

namespace raylab
{
namespace node
{

class BxDF : public Node
{
public:
    BxDF(const std::string& title) : Node(title) {}

    RTTR_ENABLE(Node)

}; // BxDF

class PerfectSpecular : public BxDF
{
public:
    PerfectSpecular() : BxDF("PerfectSpecular") {}

    float      kr = 0.0f;
    pt0::Color cr = pt0::Color(1.0f, 1.0f, 1.0f);

    RTTR_ENABLE(BxDF)

}; // PerfectSpecular

}
}
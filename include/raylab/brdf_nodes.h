#pragma once

#include "raylab/Node.h"

#include <blueprint/Pin.h>

namespace raylab
{
namespace node
{

class BRDF : public Node
{
public:
    BRDF(const std::string& title) : Node(title) {}

    RTTR_ENABLE(Node)

}; // BRDF

class PerfectSpecular : public BRDF
{
public:
    PerfectSpecular() : BRDF("PerfectSpecular") {}

    float      kr = 0.0f;
    pt0::Color cr;

    RTTR_ENABLE(BRDF)

}; // PerfectSpecular

}
}
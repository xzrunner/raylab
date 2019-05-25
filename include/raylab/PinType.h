#pragma once

namespace raylab
{

enum PinType
{
    PIN_BRDF = 1000,
    PIN_CAMERA,
    PIN_LIGHT,
    PIN_MATERIAL,
    PIN_OBJECT,
    PIN_SAMPLER,
    PIN_TEXTURE,
    PIN_TRACER,
    PIN_MAPPING,
    PIN_NOISE,

    PIN_VIEW_PLANE,
    PIN_WORLD,
};

}
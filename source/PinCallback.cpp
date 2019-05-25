#include "raylab/PinCallback.h"
#include "raylab/PinType.h"

#include <painting0/Color.h>
#include <blueprint/Pin.h>

#include <string>

namespace
{

const pt0::Color COL_DEFAULT    = pt0::Color(255, 255, 255);

const pt0::Color COL_BRDF       = pt0::Color(132, 228, 231);
const pt0::Color COL_CAMERA     = pt0::Color(154, 239, 146);
const pt0::Color COL_LIGHT      = pt0::Color(246, 255, 154);
const pt0::Color COL_MATERIAL   = pt0::Color(251, 203, 244);
const pt0::Color COL_OBJECT     = pt0::Color(251, 203, 244);
const pt0::Color COL_SAMPLER    = pt0::Color(255, 139, 139);
const pt0::Color COL_TEXTURE    = pt0::Color(148, 129, 230);
const pt0::Color COL_TRACER     = pt0::Color(148, 239, 230);
const pt0::Color COL_MAPPING    = pt0::Color(230, 148, 239);
const pt0::Color COL_NOISE      = pt0::Color(132, 231, 228);

const pt0::Color COL_VIEW_PLANE = pt0::Color(148, 129, 148);
const pt0::Color COL_WORLD      = pt0::Color(148, 255, 148);

std::string get_desc_func(const std::string& name, int type)
{
    std::string ret = name;
    //switch (type)
    //{
    //case raylab::PIN_TEXTURE:
    //    ret += "(T)";
    //    break;
    //}
    return ret;
}

const pt0::Color& get_color_func(int type)
{
    switch (type)
    {
    case raylab::PIN_BRDF:
        return COL_BRDF;
    case raylab::PIN_CAMERA:
        return COL_CAMERA;
    case raylab::PIN_LIGHT:
        return COL_LIGHT;
    case raylab::PIN_MATERIAL:
        return COL_MATERIAL;
    case raylab::PIN_OBJECT:
        return COL_OBJECT;
    case raylab::PIN_SAMPLER:
        return COL_SAMPLER;
    case raylab::PIN_TEXTURE:
        return COL_TEXTURE;
    case raylab::PIN_TRACER:
        return COL_TRACER;
    case raylab::PIN_MAPPING:
        return COL_MAPPING;
    case raylab::PIN_NOISE:
        return COL_NOISE;

    case raylab::PIN_VIEW_PLANE:
        return COL_VIEW_PLANE;
    case raylab::PIN_WORLD:
        return COL_WORLD;

    default:
        return COL_DEFAULT;
    }
}

bool can_type_cast_func(int type_from, int type_to)
{
    if (type_from == type_to) {
        return true;
    }
    if (type_from == bp::PIN_ANY_VAR ||
        type_to == bp::PIN_ANY_VAR) {
        return true;
    }

    if (type_from == bp::PIN_INTEGER && type_to == bp::PIN_FLOAT1 ||
        type_from == bp::PIN_FLOAT1 && type_to == bp::PIN_INTEGER) {
        return true;
    }

    return false;
}

}

namespace raylab
{

void InitPinCallback()
{
    bp::Pin::SetExtendFuncs({
        get_desc_func,
        get_color_func,
        can_type_cast_func
    });
}

}
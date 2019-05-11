#include "raylab/RayLab.h"
#include "raylab/PinCallback.h"
#include "raylab/light_nodes.h"
#include "raylab/camera_nodes.h"
#include "raylab/tracer_nodes.h"
#include "raylab/object_nodes.h"
#include "raylab/material_nodes.h"
#include "raylab/sampler_nodes.h"
#include "raylab/texture_nodes.h"
#include "raylab/brdf_nodes.h"
#include "raylab/world_nodes.h"

#include <blueprint/node/Hub.h>
#include <blueprint/node/Vector1.h>

namespace raylab
{

CU_SINGLETON_DEFINITION(RayLab);

extern void regist_rttr();

RayLab::RayLab()
{
    regist_rttr();

    Init();
    InitNodes();

    InitPinCallback();
}

void RayLab::Init()
{

}

void RayLab::InitNodes()
{
    auto add_node = [&](rttr::type t)
    {
        auto obj = t.create();
        if (!obj.is_valid()) {
            auto str = t.get_name().to_string();
            printf("%s\n", str.c_str());
        }
        assert(obj.is_valid());
        auto node = obj.get_value<bp::NodePtr>();
        assert(node);
        m_nodes.push_back(node);
    };

    auto add_nodes = [&](const rttr::array_range<rttr::type>& list) {
        for (auto& t : list) {
            add_node(t);
        }
    };

    add_node(rttr::type::get<bp::node::Hub>());
    add_node(rttr::type::get<bp::node::Vector1>());

    add_node(rttr::type::get<node::World>());
    add_node(rttr::type::get<node::Output>());
    add_node(rttr::type::get<node::ViewPlane>());

    add_nodes(rttr::type::get<node::Light>().get_derived_classes());
    add_nodes(rttr::type::get<node::Camera>().get_derived_classes());
    add_nodes(rttr::type::get<node::Tracer>().get_derived_classes());
    add_nodes(rttr::type::get<node::Object>().get_derived_classes());
    add_nodes(rttr::type::get<node::Material>().get_derived_classes());
    add_nodes(rttr::type::get<node::Sampler>().get_derived_classes());
    add_nodes(rttr::type::get<node::Texture>().get_derived_classes());
    add_nodes(rttr::type::get<node::BRDF>().get_derived_classes());
}

}
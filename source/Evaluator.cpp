#include "raylab/Evaluator.h"
#include "raylab/light_nodes.h"
#include "raylab/tracer_nodes.h"
#include "raylab/camera_nodes.h"
#include "raylab/object_nodes.h"
#include "raylab/world_nodes.h"
#include "raylab/material_nodes.h"

#include <blueprint/Connecting.h>
#include <blueprint/node/Hub.h>

#include <raytracing/world/World.h>
#include <raytracing/lights/Light.h>
#include <raytracing/lights/Ambient.h>
#include <raytracing/lights/PointLight.h>
#include <raytracing/tracer/RayCast.h>
#include <raytracing/cameras/Pinhole.h>
#include <raytracing/objects/Box.h>
#include <raytracing/objects/Sphere.h>
#include <raytracing/materials/Matte.h>

namespace
{

rt::RGBColor to_rt_color(const pt0::Color& col)
{
    rt::RGBColor ret;
    ret.r = col.r / 255.0f;
    ret.g = col.g / 255.0f;
    ret.b = col.b / 255.0f;
    return ret;
}

rt::Point3D to_rt_p3d(const sm::vec3& pos)
{
    return rt::Point3D(pos.x, pos.y, pos.z);
}

}

namespace raylab
{

void Evaluator::Build(rt::World& dst, const node::World& src)
{
    auto& src_inputs = src.GetAllInput();

    auto& light_conns = src_inputs[node::World::ID_LIGHT]->GetConnecting();
    if (!light_conns.empty())
    {
        std::unique_ptr<rt::Light> dst_light = nullptr;

        auto& light_node = light_conns[0]->GetFrom()->GetParent();
        auto light_type = light_node.get_type();
        if (light_type == rttr::type::get<node::Ambient>())
        {
            auto& src_light = static_cast<const node::Ambient&>(light_node);
            auto light = std::make_unique<rt::Ambient>();
            light->ScaleRadiance(src_light.radiant_scale);
            light->SetColor(to_rt_color(src_light.color));
            dst_light = std::move(light);
        }
        else if (light_type == rttr::type::get<node::PointLight>())
        {
            auto& src_light = static_cast<const node::PointLight&>(light_node);
            auto light = std::make_unique<rt::PointLight>();
            light->SetLocation(to_rt_p3d(src_light.location));
            light->ScaleRadiance(src_light.radiant_scale);
            light->SetColor(to_rt_color(src_light.color));
            dst_light = std::move(light);
        }
        else
        {
            assert(0);
        }

        if (dst_light) {
            dst.AddLight(std::move(dst_light));
        }
    }

    auto& tracer_conns = src_inputs[node::World::ID_TRACER]->GetConnecting();
    if (!tracer_conns.empty())
    {
        std::unique_ptr<rt::Tracer> dst_tracer = nullptr;

        auto& tracer_node = tracer_conns[0]->GetFrom()->GetParent();
        auto tracer_type = tracer_node.get_type();
        if (tracer_type == rttr::type::get<node::RayCast>())
        {
            auto& src_tracer = static_cast<const node::RayCast&>(tracer_node);
            auto tracer = std::make_unique<rt::RayCast>(dst);
            dst_tracer = std::move(tracer);
        }
        else
        {
            assert(0);
        }

        if (dst_tracer) {
            dst.SetTracer(std::move(dst_tracer));
        }
    }

    auto& camera_conns = src_inputs[node::World::ID_CAMERA]->GetConnecting();
    if (!camera_conns.empty())
    {
        std::unique_ptr<rt::Camera> dst_camera = nullptr;

        auto& camera_node = camera_conns[0]->GetFrom()->GetParent();
        auto camera_type = camera_node.get_type();
        if (camera_type == rttr::type::get<node::Pinhole>())
        {
            auto& src_camera = static_cast<const node::Pinhole&>(camera_node);
            auto camera = std::make_unique<rt::Pinhole>();
            camera->SetEye(to_rt_p3d(src_camera.pos));
            camera->SetLookat(to_rt_p3d(src_camera.target));
            camera->SetViewDistance(src_camera.dis);
            camera->SetZoomFactor(src_camera.zoom);
            dst_camera = std::move(camera);
        }
        else
        {
            assert(0);
        }

        if (dst_camera) {
            dst.SetCamera(std::move(dst_camera));
        }
    }

    auto& object_conns = src_inputs[node::World::ID_OBJECT]->GetConnecting();
    if (!object_conns.empty())
    {
        auto& object_node = object_conns[0]->GetFrom()->GetParent();
        auto object_type = object_node.get_type();
        if (object_type == rttr::type::get<bp::node::Hub>())
        {
            auto& hub = static_cast<const bp::node::Hub&>(object_node);
            for (auto& input : hub.GetAllInput()) {
                for (auto& conn : input->GetConnecting()) {
                    auto& from_node = conn->GetFrom()->GetParent();
                    auto obj = CreateObject(from_node);
                    if (obj) {
                        dst.AddObject(std::move(obj));
                    }
                }
            }
        }
        else
        {
            auto obj = CreateObject(object_node);
            if (obj) {
                dst.AddObject(std::move(obj));
            }
        }
    }

    auto& vp_conns = src_inputs[node::World::ID_VIEW_PLANE]->GetConnecting();
    if (!vp_conns.empty())
    {
        std::unique_ptr<rt::ViewPlane> dst_vp = nullptr;

        auto& vp_node = vp_conns[0]->GetFrom()->GetParent();
        auto vp_type = vp_node.get_type();
        if (vp_type == rttr::type::get<node::ViewPlane>())
        {
            auto& src_vp = static_cast<const node::ViewPlane&>(vp_node);
            auto vp = std::make_unique<rt::ViewPlane>();
            vp->SetWidth(src_vp.width);
            vp->SetHeight(src_vp.height);
            vp->SetPixelSize(src_vp.pixel_size);
            vp->SetSamples(src_vp.num_samples);
            dst_vp = std::move(vp);
        }
        else
        {
            assert(0);
        }

        if (dst_vp) {
            dst.SetViewPlane(*dst_vp);
        }
    }

    auto& ambient_conns = src_inputs[node::World::ID_AMBIENT]->GetConnecting();
    if (!ambient_conns.empty())
    {
        std::unique_ptr<rt::Light> dst_ambient = nullptr;

        auto& ambient_node = ambient_conns[0]->GetFrom()->GetParent();
        auto ambient_type = ambient_node.get_type();
        if (ambient_type == rttr::type::get<node::Ambient>())
        {
            auto& src_ambient = static_cast<const node::Ambient&>(ambient_node);
            auto ambient = std::make_unique<rt::Ambient>();
            ambient->ScaleRadiance(src_ambient.radiant_scale);
            ambient->SetColor(to_rt_color(src_ambient.color));
            dst_ambient = std::move(ambient);
        }
        else
        {
            assert(0);
        }

        if (dst_ambient) {
            dst.SetAmbient(std::move(dst_ambient));
        }
    }
}

std::unique_ptr<rt::Object>
Evaluator::CreateObject(const bp::Node& node)
{
    std::unique_ptr<rt::Object> dst_object = nullptr;

    auto object_type = node.get_type();
    if (object_type == rttr::type::get<bp::node::Hub>())
    {
        auto& hub = static_cast<const bp::node::Hub&>(node);
        hub.GetAllInput();

    }
    else if (object_type == rttr::type::get<node::Box>())
    {
        auto& src_object = static_cast<const node::Box&>(node);
        auto object = std::make_unique<rt::Box>(
            to_rt_p3d(src_object.min), to_rt_p3d(src_object.max)
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::Sphere>())
    {
        auto& src_object = static_cast<const node::Sphere&>(node);
        auto object = std::make_unique<rt::Sphere>();
        object->SetCenter(to_rt_p3d(src_object.center));
        object->SetRadius(src_object.radius);
        dst_object = std::move(object);
    }
    else
    {
        assert(0);
    }

    // material
    auto& material_conns = node.GetAllInput()[0]->GetConnecting();
    if (!material_conns.empty())
    {
        auto dst_material = CreateMaterial(material_conns[0]->GetFrom()->GetParent());
        if (dst_material) {
            dst_object->SetMaterial(dst_material);
        }
    }

    return dst_object;
}

std::shared_ptr<rt::Material>
Evaluator::CreateMaterial(const bp::Node& node)
{
    std::shared_ptr<rt::Material> dst_material = nullptr;

    auto material_type = node.get_type();
    if (material_type == rttr::type::get<node::Matte>())
    {
        auto& src_material = static_cast<const node::Matte&>(node);
        auto material = std::make_shared<rt::Matte>();
        material->SetKa(src_material.ka);
        material->SetKd(src_material.kd);
        material->SetCd(to_rt_color(src_material.cd));
        dst_material = material;
    }
    else
    {
        assert(0);
    }

    return dst_material;
}

}
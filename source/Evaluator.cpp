#include "raylab/Evaluator.h"
#include "raylab/light_nodes.h"
#include "raylab/tracer_nodes.h"
#include "raylab/camera_nodes.h"
#include "raylab/object_nodes.h"
#include "raylab/world_nodes.h"
#include "raylab/material_nodes.h"
#include "raylab/texture_nodes.h"
#include "raylab/brdf_nodes.h"
#include "raylab/sampler_nodes.h"
#include "raylab/mapping_nodes.h"

#include <blueprint/Connecting.h>
#include <blueprint/node/Hub.h>
#include <blueprint/node/Vector1.h>
#include <blueprint/node/Vector3.h>

#include <raytracing/world/World.h>
#include <raytracing/lights/Light.h>
#include <raytracing/lights/Ambient.h>
#include <raytracing/lights/Directional.h>
#include <raytracing/lights/PointLight.h>
#include <raytracing/lights/AreaLight.h>
#include <raytracing/lights/AmbientOccluder.h>
#include <raytracing/lights/EnvironmentLight.h>
#include <raytracing/lights/FakeSphericalLight.h>
#include <raytracing/tracer/RayCast.h>
#include <raytracing/tracer/AreaLighting.h>
#include <raytracing/tracer/FirstHit.h>
#include <raytracing/cameras/Pinhole.h>
#include <raytracing/cameras/ThinLens.h>
#include <raytracing/cameras/FishEye.h>
#include <raytracing/cameras/Spherical.h>
#include <raytracing/cameras/Stereo.h>
#include <raytracing/cameras/Orthographic.h>
#include <raytracing/objects/Box.h>
#include <raytracing/objects/Sphere.h>
#include <raytracing/objects/Plane.h>
#include <raytracing/objects/Rectangle.h>
#include <raytracing/objects/Triangle.h>
#include <raytracing/objects/WireframeBox.h>
#include <raytracing/objects/Instance.h>
#include <raytracing/objects/Disk.h>
#include <raytracing/objects/SolidCylinder.h>
#include <raytracing/objects/ConvexPartCylinder.h>
#include <raytracing/objects/OpenPartCylinder.h>
#include <raytracing/objects/Grid.h>
#include <raytracing/objects/ConcaveSphere.h>
#include <raytracing/objects/Torus.h>
#include <raytracing/objects/OpenCone.h>
#include <raytracing/objects/ConvexPartSphere.h>
#include <raytracing/objects/FlatRimmedBowl.h>
#include <raytracing/objects/RoundRimmedBowl.h>
#include <raytracing/objects/ThickRing.h>
#include <raytracing/objects/BeveledCylinder.h>
#include <raytracing/objects/BeveledRing.h>
#include <raytracing/objects/BeveledBox.h>
#include <raytracing/objects/BeveledWedge.h>
#include <raytracing/materials/Matte.h>
#include <raytracing/materials/SV_Matte.h>
#include <raytracing/materials/Emissive.h>
#include <raytracing/materials/Phong.h>
#include <raytracing/materials/Reflective.h>
#include <raytracing/texture/Checker3D.h>
#include <raytracing/texture/ImageTexture.h>
#include <raytracing/texture/Image.h>
#include <raytracing/texture/ConeChecker.h>
#include <raytracing/brdfs/PerfectSpecular.h>
#include <raytracing/samplers/Jittered.h>
#include <raytracing/samplers/MultiJittered.h>
#include <raytracing/samplers/Regular.h>
#include <raytracing/samplers/PureRandom.h>
#include <raytracing/mapping/SphericalMap.h>
#include <raytracing/mapping/LightProbe.h>

#include <boost/filesystem.hpp>

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

rt::Vector3D to_rt_v3d(const sm::vec3& pos)
{
    return rt::Vector3D(pos.x, pos.y, pos.z);
}

rt::Normal to_rt_normal(const sm::vec3& pos)
{
    return rt::Normal(pos.x, pos.y, pos.z);
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
        auto& light_node = light_conns[0]->GetFrom()->GetParent();
        auto object_type = light_node.get_type();
        if (object_type == rttr::type::get<bp::node::Hub>())
        {
            auto& hub = static_cast<const bp::node::Hub&>(light_node);
            for (auto& input : hub.GetAllInput()) {
                for (auto& conn : input->GetConnecting()) {
                    auto& from_node = conn->GetFrom()->GetParent();
                    if (auto light = CreateLight(from_node)) {
                        dst.AddLight(std::move(light));
                    }
                }
            }
        }
        else
        {
            if (auto light = CreateLight(light_node)) {
                dst.AddLight(std::move(light));
            }
        }
    }

    auto& tracer_conns = src_inputs[node::World::ID_TRACER]->GetConnecting();
    if (!tracer_conns.empty()) {
        if (auto dst_tracer = CreateTracer(tracer_conns[0]->GetFrom()->GetParent(), dst)) {
            dst.SetTracer(std::move(dst_tracer));
        }
    }

    auto& camera_conns = src_inputs[node::World::ID_CAMERA]->GetConnecting();
    if (!camera_conns.empty()) {
        if (auto cam = CreateCamera(camera_conns[0]->GetFrom()->GetParent())) {
            dst.SetCamera(std::move(cam));
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
                    if (auto obj = CreateObject(from_node)) {
                        dst.AddObject(std::move(obj));
                    }
                }
            }
        }
        else
        {
            if (auto obj = CreateObject(object_node)) {
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
            vp->SetMaxDepth(static_cast<int>(src_vp.max_depth));
            vp->SetShowOutOfGamut(src_vp.show_out_of_gamut);

            auto& sampler_conns = vp_node.GetAllInput()[node::ViewPlane::ID_SAMPLER]->GetConnecting();
            if (!sampler_conns.empty()) {
                vp->SetSampler(CreateSampler(sampler_conns[0]->GetFrom()->GetParent()));
            } else {
                vp->SetSamples(src_vp.num_samples);
            }

            auto& num_conns = vp_node.GetAllInput()[node::ViewPlane::ID_NUM_SAMPLES]->GetConnecting();
            if (!num_conns.empty()) {
                int num = static_cast<int>(CalcFloat(*num_conns[0]));
                vp->SetSamples(num);
            }

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
        auto dst_ambient = CreateLight(ambient_conns[0]->GetFrom()->GetParent());
        if (dst_ambient) {
            dst.SetAmbient(std::move(dst_ambient));
        }
    }
    else
    {
        dst.SetAmbient(std::make_unique<rt::Ambient>());
    }
}

std::unique_ptr<rt::Light>
Evaluator::CreateLight(const bp::Node& node)
{
    std::unique_ptr<rt::Light> dst_light = nullptr;

    auto light_type = node.get_type();
    if (light_type == rttr::type::get<node::Ambient>())
    {
        auto& src_light = static_cast<const node::Ambient&>(node);
        auto light = std::make_unique<rt::Ambient>();
        light->ScaleRadiance(src_light.radiant_scale);
        light->SetColor(to_rt_color(src_light.color));
        dst_light = std::move(light);
    }
    else if (light_type == rttr::type::get<node::Directional>())
    {
        auto& src_light = static_cast<const node::Directional&>(node);
        auto light = std::make_unique<rt::Directional>();
        light->SetDirection(to_rt_p3d(src_light.dir));
        light->ScaleRadiance(src_light.radiant_scale);
        light->SetColor(to_rt_color(src_light.color));
        dst_light = std::move(light);
    }
    else if (light_type == rttr::type::get<node::PointLight>())
    {
        auto& src_light = static_cast<const node::PointLight&>(node);
        auto light = std::make_unique<rt::PointLight>();
        light->SetLocation(to_rt_p3d(src_light.location));
        light->ScaleRadiance(src_light.radiant_scale);
        light->SetColor(to_rt_color(src_light.color));
        dst_light = std::move(light);
    }
    else if (light_type == rttr::type::get<node::AreaLight>())
    {
        auto& src_light = static_cast<const node::AreaLight&>(node);
        auto light = std::make_unique<rt::AreaLight>();

        auto& conns = node.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            std::shared_ptr<rt::Object> obj = CreateObject(conns[0]->GetFrom()->GetParent());
            if (obj) {
                light->SetObject(obj);
            }
        }

        dst_light = std::move(light);
    }
    else if (light_type == rttr::type::get<node::AmbientOccluder>())
    {
        auto& src_light = static_cast<const node::AmbientOccluder&>(node);
        auto light = std::make_unique<rt::AmbientOccluder>();

        light->ScaleRadiance(src_light.scale_radiance);
        light->SetMinAmount(src_light.min_amount);

        auto& conns = node.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            auto sampler = CreateSampler(conns[0]->GetFrom()->GetParent());
            if (sampler) {
                light->SetSampler(sampler);
            }
        }

        dst_light = std::move(light);
    }
    else if (light_type == rttr::type::get<node::EnvironmentLight>())
    {
        auto& src_light = static_cast<const node::EnvironmentLight&>(node);
        auto light = std::make_unique<rt::EnvironmentLight>();

        auto& samp_conns = node.GetAllInput()[node::EnvironmentLight::ID_SAMPLER]->GetConnecting();
        if (!samp_conns.empty()) {
            auto sampler = CreateSampler(samp_conns[0]->GetFrom()->GetParent());
            if (sampler) {
                light->SetSampler(sampler);
            }
        }

        auto& mat_conns = node.GetAllInput()[node::EnvironmentLight::ID_MATERIAL]->GetConnecting();
        if (!mat_conns.empty()) {
            auto material = CreateMaterial(mat_conns[0]->GetFrom()->GetParent());
            if (material) {
                light->SetMaterial(material);
            }
        }

        dst_light = std::move(light);
    }
    else if (light_type == rttr::type::get<node::FakeSphericalLight>())
    {
        auto& src_light = static_cast<const node::FakeSphericalLight&>(node);
        auto light = std::make_unique<rt::FakeSphericalLight>();
        light->ScaleRadiance(src_light.scale_radiance);
        light->SetJitterAmount(src_light.jitter_amount);
        light->SetDistanceAttenuation(src_light.distance_attenuation);
        dst_light = std::move(light);
    }
    else
    {
        assert(0);
    }

    auto& src_light = static_cast<const node::Light&>(node);
    dst_light->SetShadows(src_light.shadows);

    return dst_light;
}

std::unique_ptr<rt::Tracer>
Evaluator::CreateTracer(const bp::Node& node, rt::World& dst)
{
    std::unique_ptr<rt::Tracer> dst_tracer = nullptr;

    auto tracer_type = node.get_type();
    if (tracer_type == rttr::type::get<node::RayCast>())
    {
        auto& src_tracer = static_cast<const node::RayCast&>(node);
        auto tracer = std::make_unique<rt::RayCast>(dst);
        dst_tracer = std::move(tracer);
    }
    else if (tracer_type == rttr::type::get<node::AreaLighting>())
    {
        auto& src_tracer = static_cast<const node::AreaLighting&>(node);
        auto tracer = std::make_unique<rt::AreaLighting>(dst);
        dst_tracer = std::move(tracer);
    }
    else if (tracer_type == rttr::type::get<node::FirstHit>())
    {
        auto& src_tracer = static_cast<const node::FirstHit&>(node);
        auto tracer = std::make_unique<rt::FirstHit>(dst);
        dst_tracer = std::move(tracer);
    }
    else
    {
        assert(0);
    }

    return dst_tracer;
}

std::unique_ptr<rt::Object>
Evaluator::CreateObject(const bp::Node& node)
{
    std::unique_ptr<rt::Object> dst_object = nullptr;

    auto object_type = node.get_type();
    if (object_type == rttr::type::get<node::Box>())
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
    else if (object_type == rttr::type::get<node::Plane>())
    {
        auto& src_object = static_cast<const node::Plane&>(node);

        auto pos  = to_rt_p3d(src_object.pos);
        auto norm = to_rt_normal(src_object.normal);
        auto object = std::make_unique<rt::Plane>(pos, norm);
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::Rectangle>())
    {
        auto& src_object = static_cast<const node::Rectangle&>(node);

        rt::Point3D p0;
        auto& conns_p0 = node.GetAllInput()[node::Rectangle::ID_P0]->GetConnecting();
        if (!conns_p0.empty()) {
            p0 = to_rt_p3d(CalcFloat3(*conns_p0[0]));
        } else {
            p0 = to_rt_p3d(src_object.p0);
        }
        auto a    = to_rt_v3d(src_object.a);
        auto b    = to_rt_v3d(src_object.b);
        auto norm = to_rt_normal(src_object.normal);
        auto object = std::make_unique<rt::Rectangle>(p0, a, b/*, norm*/);

        auto& conns_sampler = node.GetAllInput()[node::Rectangle::ID_SAMPLER]->GetConnecting();
        if (!conns_sampler.empty()) {
            if (auto sampler = CreateSampler(conns_sampler[0]->GetFrom()->GetParent())) {
                object->SetSampler(sampler);
            }
        }

        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::Triangle>())
    {
        auto& src_object = static_cast<const node::Triangle&>(node);

        auto v0 = to_rt_p3d(src_object.v0);
        auto v1 = to_rt_p3d(src_object.v1);
        auto v2 = to_rt_p3d(src_object.v2);
        auto object = std::make_unique<rt::Triangle>(v0, v1, v2);
        object->SetNormal(to_rt_normal(src_object.normal));

        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::WireframeBox>())
    {
        auto& src_object = static_cast<const node::WireframeBox&>(node);

        auto p0 = to_rt_p3d(src_object.p0);
        auto p1 = to_rt_p3d(src_object.p1);
        auto object = std::make_unique<rt::WireframeBox>(p0, p1, src_object.bevel_radius);

        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::Instance>())
    {
        auto& src_object = static_cast<const node::Instance&>(node);

        std::unique_ptr<rt::Instance> object = nullptr;
        auto& conns = node.GetAllInput()[node::Instance::ID_OBJECT]->GetConnecting();
        if (conns.empty()) {
            object = std::make_unique<rt::Instance>();
        } else {
            object = std::make_unique<rt::Instance>(CreateObject(conns[0]->GetFrom()->GetParent()));
        }
        //for (auto& op : src_object.ops)
        //{
        //    switch (op.type)
        //    {
        //    case node::Instance::OpType::Scale:
        //        object->Scale(to_rt_v3d(op.val));
        //        break;
        //    case node::Instance::OpType::Rotate:
        //        if (op.val.x != 0) {
        //            object->RotateX(op.val.x);
        //        }
        //        if (op.val.y != 0) {
        //            object->RotateY(op.val.y);
        //        }
        //        if (op.val.z != 0) {
        //            object->RotateZ(op.val.z);
        //        }
        //        break;
        //    case node::Instance::OpType::Translate:
        //        object->Translate(to_rt_v3d(op.val));
        //        break;
        //    }
        //}

        if (src_object.scale != sm::vec3(1, 1, 1)) {
            object->Scale(to_rt_v3d(src_object.scale));
        }
        if (src_object.rotate != sm::vec3(0, 0, 0))
        {
            if (src_object.rotate.x != 0) {
                object->RotateX(src_object.rotate.x);
            }
            if (src_object.rotate.y != 0) {
                object->RotateY(src_object.rotate.y);
            }
            if (src_object.rotate.z != 0) {
                object->RotateZ(src_object.rotate.z);
            }
        }
        if (src_object.translate != sm::vec3(0, 0, 0)) {
            object->Translate(to_rt_v3d(src_object.translate));
        }

        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::Disk>())
    {
        auto& src_object = static_cast<const node::Disk&>(node);

        auto center = to_rt_p3d(src_object.center);
        auto normal = to_rt_normal(src_object.normal);
        auto object = std::make_unique<rt::Disk>(center, normal, src_object.radius);

        auto& conns = node.GetAllInput()[node::Disk::ID_SAMPLER]->GetConnecting();
        if (!conns.empty()) {
            object->SetSampler(CreateSampler(conns[0]->GetFrom()->GetParent()));
        }

        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::SolidCylinder>())
    {
        auto& src_object = static_cast<const node::SolidCylinder&>(node);
        auto object = std::make_unique<rt::SolidCylinder>(
            src_object.bottom, src_object.top, src_object.radius
        );

        auto& conns_b = node.GetAllInput()[node::SolidCylinder::ID_BOTTOM_MATERIAL]->GetConnecting();
        if (!conns_b.empty()) {
            object->SetBottomMaterial(CreateMaterial(conns_b[0]->GetFrom()->GetParent()));
        }
        auto& conns_t = node.GetAllInput()[node::SolidCylinder::ID_TOP_MATERIAL]->GetConnecting();
        if (!conns_t.empty()) {
            object->SetTopMaterial(CreateMaterial(conns_t[0]->GetFrom()->GetParent()));
        }
        auto& conns_w = node.GetAllInput()[node::SolidCylinder::ID_WALL_MATERIAL]->GetConnecting();
        if (!conns_w.empty()) {
            object->SetWallMaterial(CreateMaterial(conns_w[0]->GetFrom()->GetParent()));
        }

        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::ConvexPartCylinder>())
    {
        auto& src_object = static_cast<const node::ConvexPartCylinder&>(node);
        auto object = std::make_unique<rt::ConvexPartCylinder>(
            src_object.bottom, src_object.top, src_object.radius, src_object.polar_min, src_object.polar_max
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::OpenPartCylinder>())
    {
        auto& src_object = static_cast<const node::OpenPartCylinder&>(node);
        auto object = std::make_unique<rt::OpenPartCylinder>(
            src_object.bottom, src_object.top, src_object.radius, src_object.polar_min, src_object.polar_max
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::Grid>())
    {
        auto& src_object = static_cast<const node::Grid&>(node);
        auto object = std::make_unique<rt::Grid>();

        // read from file
        if (!src_object.filename.empty() &&
            boost::filesystem::exists(src_object.filename))
        {
            switch (src_object.triangle_type)
            {
            case node::Grid::TriangleType::Flat:
                object->ReadFlatTriangles(src_object.filename);
                break;
            case node::Grid::TriangleType::Smooth:
                object->ReadSmoothTriangles(src_object.filename);
                break;
            }
        }

        // gen tessellate sphere
        if (src_object.hori_steps != 0 &&
            src_object.vert_steps != 0)
        {
            switch (src_object.triangle_type)
            {
            case node::Grid::TriangleType::Flat:
                object->TessellateFlatSphere(src_object.hori_steps, src_object.vert_steps);
                break;
            case node::Grid::TriangleType::Smooth:
                object->TessellateSmoothSphere(src_object.hori_steps, src_object.vert_steps);
                break;
            }
        }

        auto& conns = node.GetAllInput()[node::Grid::ID_CHILDREN]->GetConnecting();
        if (!conns.empty())
        {
            auto& node = conns[0]->GetFrom()->GetParent();
            if (node.get_type() == rttr::type::get<bp::node::Hub>())
            {
                auto& hub = static_cast<const bp::node::Hub&>(node);
                for (auto& input : hub.GetAllInput()) {
                    for (auto& conn : input->GetConnecting()) {
                        auto& from_node = conn->GetFrom()->GetParent();
                        if (auto obj = CreateObject(from_node)) {
                            object->AddObject(std::move(obj));
                        }
                    }
                }
            }
        }

        object->SetupCells();

        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::ConcaveSphere>())
    {
        auto& src_object = static_cast<const node::ConcaveSphere&>(node);

        auto center = to_rt_p3d(src_object.center);
        auto object = std::make_unique<rt::ConcaveSphere>(center, src_object.radius);
        object->SetShadows(src_object.shadows);

        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::Torus>())
    {
        auto& src_object = static_cast<const node::Torus&>(node);
        auto object = std::make_unique<rt::Torus>(
            src_object.swept_radius, src_object.tube_radius
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::OpenCone>())
    {
        auto& src_object = static_cast<const node::OpenCone&>(node);
        auto object = std::make_unique<rt::OpenCone>(
            src_object.height, src_object.radius
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::ConvexPartSphere>())
    {
        auto& src_object = static_cast<const node::ConvexPartSphere&>(node);
        auto center = to_rt_p3d(src_object.center);
        auto object = std::make_unique<rt::ConvexPartSphere>(
            center, src_object.radius, src_object.azimuth_min, src_object.azimuth_max, src_object.polar_min, src_object.polar_max
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::FlatRimmedBowl>())
    {
        auto& src_object = static_cast<const node::FlatRimmedBowl&>(node);
        auto object = std::make_unique<rt::FlatRimmedBowl>();
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::RoundRimmedBowl>())
    {
        auto& src_object = static_cast<const node::RoundRimmedBowl&>(node);
        auto object = std::make_unique<rt::RoundRimmedBowl>(
            src_object.inner_radius, src_object.outer_radius, src_object.openning_angle
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::ThickRing>())
    {
        auto& src_object = static_cast<const node::ThickRing&>(node);
        auto object = std::make_unique<rt::ThickRing>(
            src_object.bottom, src_object.top, src_object.inner_radius, src_object.outer_radius
        );

        auto& conns_b = node.GetAllInput()[node::ThickRing::ID_BOTTOM_MATERIAL]->GetConnecting();
        if (!conns_b.empty()) {
            object->SetBottomMaterial(CreateMaterial(conns_b[0]->GetFrom()->GetParent()));
        }
        auto& conns_t = node.GetAllInput()[node::ThickRing::ID_TOP_MATERIAL]->GetConnecting();
        if (!conns_t.empty()) {
            object->SetTopMaterial(CreateMaterial(conns_t[0]->GetFrom()->GetParent()));
        }
        auto& conns_iw = node.GetAllInput()[node::ThickRing::ID_INNER_WALL_MATERIAL]->GetConnecting();
        if (!conns_iw.empty()) {
            object->SetInnerWallMaterial(CreateMaterial(conns_iw[0]->GetFrom()->GetParent()));
        }
        auto& conns_ow = node.GetAllInput()[node::ThickRing::ID_OUTER_WALL_MATERIAL]->GetConnecting();
        if (!conns_ow.empty()) {
            object->SetOuterWallMaterial(CreateMaterial(conns_ow[0]->GetFrom()->GetParent()));
        }

        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::BeveledCylinder>())
    {
        auto& src_object = static_cast<const node::BeveledCylinder&>(node);
        auto object = std::make_unique<rt::BeveledCylinder>(
            src_object.bottom, src_object.top, src_object.radius, src_object.bevel_radius
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::BeveledRing>())
    {
        auto& src_object = static_cast<const node::BeveledRing&>(node);
        auto object = std::make_unique<rt::BeveledRing>(
            src_object.bottom, src_object.top, src_object.inner_radius, src_object.outer_radius, src_object.bevel_radius
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::BeveledBox>())
    {
        auto& src_object = static_cast<const node::BeveledBox&>(node);
        auto object = std::make_unique<rt::BeveledBox>(
            to_rt_p3d(src_object.bottom), to_rt_p3d(src_object.top), src_object.bevel_radius
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::BeveledWedge>())
    {
        auto& src_object = static_cast<const node::BeveledWedge&>(node);
        auto object = std::make_unique<rt::BeveledWedge>(
            src_object.min_y, src_object.max_y, src_object.inner_radius, src_object.outer_radius,
            src_object.bevel_radius, src_object.min_azimuth, src_object.max_azimuth
        );
        dst_object = std::move(object);
    }
    else
    {
        assert(0);
    }

    // material
//    if (node.get_type() != rttr::type::get<node::Grid>())
    {
        auto& material_conns = node.GetAllInput()[0]->GetConnecting();
        if (!material_conns.empty())
        {
            auto dst_material = CreateMaterial(material_conns[0]->GetFrom()->GetParent());
            if (dst_material) {
                dst_object->SetMaterial(dst_material);
            }
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

        auto& inputs = node.GetAllInput();

        float ka = src_material.ka;
        auto& ka_conns = inputs[node::Matte::ID_KA]->GetConnecting();
        if (!ka_conns.empty()) {
            ka = CalcFloat(*ka_conns[0]);
        }

        float kd = src_material.kd;
        auto& kd_conns = inputs[node::Matte::ID_KD]->GetConnecting();
        if (!kd_conns.empty()) {
            kd = CalcFloat(*kd_conns[0]);
        }

        material->SetKa(ka);
        material->SetKd(kd);
        material->SetCd(to_rt_color(src_material.cd));

        dst_material = material;
    }
    else if (material_type == rttr::type::get<node::SV_Matte>())
    {
        auto& src_material = static_cast<const node::SV_Matte&>(node);
        auto material = std::make_shared<rt::SV_Matte>();

        auto& inputs = node.GetAllInput();

        float ka = src_material.ka;
        auto& ka_conns = inputs[node::SV_Matte::ID_KA]->GetConnecting();
        if (!ka_conns.empty()) {
            ka = CalcFloat(*ka_conns[0]);
        }

        float kd = src_material.kd;
        auto& kd_conns = inputs[node::SV_Matte::ID_KD]->GetConnecting();
        if (!kd_conns.empty()) {
            kd = CalcFloat(*kd_conns[0]);
        }

        std::shared_ptr<rt::Texture> tex = nullptr;
        auto& tex_conns = inputs[node::SV_Matte::ID_CD]->GetConnecting();
        if (!tex_conns.empty()) {
            tex = CreateTexture(tex_conns[0]->GetFrom()->GetParent());
        }

        material->SetKa(ka);
        material->SetKd(kd);
        material->SetCd(tex);

        dst_material = material;
    }
    else if (material_type == rttr::type::get<node::Emissive>())
    {
        auto& src_material = static_cast<const node::Emissive&>(node);
        auto material = std::make_shared<rt::Emissive>();

        material->SetRadianceScaleFactor(src_material.radiance_scale_factor);
        material->SetColor(to_rt_color(src_material.color));

        dst_material = material;
    }
    else if (material_type == rttr::type::get<node::Phong>())
    {
        auto& src_material = static_cast<const node::Phong&>(node);
        auto material = std::make_shared<rt::Phong>();

        material->SetKa(src_material.ka);
        material->SetKd(src_material.kd);
        material->SetKs(src_material.ks);
        material->SetCd(to_rt_color(src_material.cd));
        material->SetCs(to_rt_color(src_material.cs));
        material->SetExp(src_material.exp);

        dst_material = material;
    }
    else if (material_type == rttr::type::get<node::Reflective>())
    {
        auto& src_material = static_cast<const node::Reflective&>(node);
        auto material = std::make_shared<rt::Reflective>();

        // fixme: copy from phong
        material->SetKa(src_material.ka);
        material->SetKd(src_material.kd);
        material->SetKs(src_material.ks);
        material->SetCd(to_rt_color(src_material.cd));
        material->SetCs(to_rt_color(src_material.cs));
        material->SetExp(src_material.exp);

        material->SetKr(src_material.kr);
        material->SetCr(to_rt_color(src_material.cr));

        dst_material = material;
    }
    else
    {
        assert(0);
    }

    return dst_material;
}

std::unique_ptr<rt::Camera>
Evaluator::CreateCamera(const bp::Node& node)
{
    std::unique_ptr<rt::Camera> dst_camera = nullptr;

    auto camera_type = node.get_type();
    if (camera_type == rttr::type::get<node::Pinhole>())
    {
        auto& src_camera = static_cast<const node::Pinhole&>(node);
        auto camera = std::make_unique<rt::Pinhole>();
        camera->SetViewDistance(src_camera.dis);
        camera->SetZoomFactor(src_camera.zoom);
        dst_camera = std::move(camera);
    }
    else if (camera_type == rttr::type::get<node::ThinLens>())
    {
        auto& src_camera = static_cast<const node::ThinLens&>(node);
        auto camera = std::make_unique<rt::ThinLens>();

        camera->SetLensRadius(src_camera.lens_radius);
        camera->SetViewDistance(src_camera.d);
        camera->SetFocalDistance(src_camera.f);
        camera->SetZoom(src_camera.zoom);

        auto& conns = node.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            auto sampler = CreateSampler(conns[0]->GetFrom()->GetParent());
            if (sampler) {
                camera->SetSampler(sampler);
            }
        }

        dst_camera = std::move(camera);
    }
    else if (camera_type == rttr::type::get<node::FishEye>())
    {
        auto& src_camera = static_cast<const node::FishEye&>(node);
        auto camera = std::make_unique<rt::FishEye>();
        camera->SetFov(src_camera.fov);
        dst_camera = std::move(camera);
    }
    else if (camera_type == rttr::type::get<node::Spherical>())
    {
        auto& src_camera = static_cast<const node::Spherical&>(node);
        auto camera = std::make_unique<rt::Spherical>();
        camera->SetHorizontalFov(src_camera.hori_fov);
        camera->SetVerticalFov(src_camera.vert_fov);
        dst_camera = std::move(camera);
    }
    else if (camera_type == rttr::type::get<node::Stereo>())
    {
        auto& src_camera = static_cast<const node::Stereo&>(node);
        auto camera = std::make_unique<rt::Stereo>();

        auto& left_conns = node.GetAllInput()[node::Stereo::ID_LEFT_CAM]->GetConnecting();
        if (!left_conns.empty())
        {
            std::shared_ptr<rt::Camera> cam = CreateCamera(left_conns[0]->GetFrom()->GetParent());
            if (cam) {
                camera->SetLeftCam(cam);
            }
        }

        auto& right_conns = node.GetAllInput()[node::Stereo::ID_RIGHT_CAM]->GetConnecting();
        if (!right_conns.empty())
        {
            std::shared_ptr<rt::Camera> cam = CreateCamera(right_conns[0]->GetFrom()->GetParent());
            if (cam) {
                camera->SetRightCam(cam);
            }
        }

        switch (src_camera.viewing_type)
        {
        case node::Stereo::ViewingType::Parallel:
            camera->SetViewingType(rt::Stereo::ViewingType::Parallel);
            break;
        case node::Stereo::ViewingType::Transverse:
            camera->SetViewingType(rt::Stereo::ViewingType::Transverse);
            break;
        }

        camera->SetPixelGap(src_camera.pixel_gap);
        camera->SetStereoAngle(src_camera.stereo_angle);

        dst_camera = std::move(camera);
    }
    else if (camera_type == rttr::type::get<node::Orthographic>())
    {
        auto& src_camera = static_cast<const node::Orthographic&>(node);
        auto camera = std::make_unique<rt::Orthographic>();
        camera->SetViewDistance(src_camera.dis);
        camera->SetZoomFactor(src_camera.zoom);
        dst_camera = std::move(camera);
    }
    else
    {
        assert(0);
    }

    auto& src_camera = static_cast<const node::Camera&>(node);
    dst_camera->SetEye(to_rt_p3d(src_camera.pos));
    dst_camera->SetLookat(to_rt_p3d(src_camera.target));

    dst_camera->ComputeUVW();

    if (camera_type == rttr::type::get<node::Stereo>()) {
        static_cast<rt::Stereo*>(dst_camera.get())->SetupCameras();
    }

    return dst_camera;
}

std::shared_ptr<rt::Texture>
Evaluator::CreateTexture(const bp::Node& node)
{
    std::shared_ptr<rt::Texture> dst_tex = nullptr;

    auto tex_type = node.get_type();
    if (tex_type == rttr::type::get<node::Checker3D>())
    {
        auto& src_tex = static_cast<const node::Checker3D&>(node);
        auto tex = std::make_shared<rt::Checker3D>();

        tex->SetColor1(to_rt_color(src_tex.col0));
        tex->SetColor2(to_rt_color(src_tex.col1));
        tex->SetSize(src_tex.size);

        dst_tex = tex;
    }
    else if (tex_type == rttr::type::get<node::ImageTexture>())
    {
        auto& src_tex = static_cast<const node::ImageTexture&>(node);
        auto tex = std::make_shared<rt::ImageTexture>();

        auto& conns = node.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            tex->SetMapping(CreateMapping(conns[0]->GetFrom()->GetParent()));
        }

        auto image = std::make_shared<rt::Image>();
        image->ReadPPMFile(src_tex.filepath.c_str());
        tex->SetImage(image);

        dst_tex = tex;
    }
    else if (tex_type == rttr::type::get<node::ConeChecker>())
    {
        auto& src_tex = static_cast<const node::ConeChecker&>(node);
        auto tex = std::make_shared<rt::ConeChecker>();

        tex->SetNumHorizontalCheckers(src_tex.num_horizontal_checkers);
        tex->SetNumVerticalCheckers(src_tex.num_vertical_checkers);
        tex->SetHorizontalLineWidth(src_tex.horizontal_line_width);
        tex->SetVerticalLineWidth(src_tex.vertical_line_width);

        tex->SetColor1(to_rt_color(src_tex.color1));
        tex->SetColor2(to_rt_color(src_tex.color2));
        tex->SetLineColor(to_rt_color(src_tex.line_color));

        dst_tex = tex;
    }

    return dst_tex;
}

std::shared_ptr<rt::BRDF>
Evaluator::CreateBRDF(const bp::Node& node)
{
    std::shared_ptr<rt::BRDF> dst_brdf = nullptr;

    auto tex_type = node.get_type();
    if (tex_type == rttr::type::get<node::PerfectSpecular>())
    {
        auto& src_brdf = static_cast<const node::PerfectSpecular&>(node);
        auto brdf = std::make_shared<rt::PerfectSpecular>();

        brdf->SetKr(src_brdf.kr);
        brdf->SetCr(to_rt_color(src_brdf.cr));

        dst_brdf = brdf;
    }

    return dst_brdf;
}

std::shared_ptr<rt::Sampler>
Evaluator::CreateSampler(const bp::Node& node)
{
    std::shared_ptr<rt::Sampler> dst_sampler = nullptr;

    int num_samples = 0;
    auto& conns = node.GetAllInput()[0]->GetConnecting();
    if (!conns.empty()) {
        num_samples = static_cast<int>(CalcFloat(*conns[0]));
    }

    auto sampler_type = node.get_type();
    if (sampler_type == rttr::type::get<node::Jittered>())
    {
        auto& src_sampler = static_cast<const node::Jittered&>(node);
        if (num_samples == 0) {
            num_samples = src_sampler.num_samples;
        }
        auto sampler = std::make_shared<rt::Jittered>(num_samples);
        dst_sampler = sampler;
    }
    else if (sampler_type == rttr::type::get<node::MultiJittered>())
    {
        auto& src_sampler = static_cast<const node::MultiJittered&>(node);
        if (num_samples == 0) {
            num_samples = src_sampler.num_samples;
        }
        auto sampler = std::make_shared<rt::MultiJittered>(num_samples);
        dst_sampler = sampler;
    }
    else if (sampler_type == rttr::type::get<node::Regular>())
    {
        auto& src_sampler = static_cast<const node::Regular&>(node);
        if (num_samples == 0) {
            num_samples = src_sampler.num_samples;
        }
        auto sampler = std::make_shared<rt::Regular>(num_samples);
        dst_sampler = sampler;
    }
    else if (sampler_type == rttr::type::get<node::PureRandom>())
    {
        auto& src_sampler = static_cast<const node::PureRandom&>(node);
        if (num_samples == 0) {
            num_samples = src_sampler.num_samples;
        }
        auto sampler = std::make_shared<rt::PureRandom>(num_samples);
        dst_sampler = sampler;
    }

    return dst_sampler;
}

std::shared_ptr<rt::Mapping>
Evaluator::CreateMapping(const bp::Node& node)
{
    std::shared_ptr<rt::Mapping> dst_mapping = nullptr;

    auto mapping_type = node.get_type();
    if (mapping_type == rttr::type::get<node::SphericalMap>())
    {
        auto& src_mapping = static_cast<const node::SphericalMap&>(node);
        auto mapping = std::make_shared<rt::SphericalMap>();
        dst_mapping = mapping;
    }
    else if (mapping_type == rttr::type::get<node::LightProbe>())
    {
        auto& src_mapping = static_cast<const node::LightProbe&>(node);
        auto mapping = std::make_shared<rt::LightProbe>();
        dst_mapping = mapping;
    }

    return dst_mapping;
}

float Evaluator::CalcFloat(const bp::Connecting& conn)
{
    float ret = 0;

    auto& node = conn.GetFrom()->GetParent();
    auto node_type = node.get_type();
    if (node_type == rttr::type::get<bp::node::Vector1>()) {
        ret = static_cast<const bp::node::Vector1&>(node).GetValue();
    }

    return ret;
}

sm::vec3 Evaluator::CalcFloat3(const bp::Connecting& conn)
{
    sm::vec3 ret;

    auto& node = conn.GetFrom()->GetParent();
    auto node_type = node.get_type();
    if (node_type == rttr::type::get<bp::node::Vector3>()) {
        ret = static_cast<const bp::node::Vector3&>(node).GetValue();
    }

    return ret;
}

}
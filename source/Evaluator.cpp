#include "raylab/Evaluator.h"
#include "raylab/light_nodes.h"
#include "raylab/tracer_nodes.h"
#include "raylab/camera_nodes.h"
#include "raylab/primitive_nodes.h"
#include "raylab/world_nodes.h"
#include "raylab/material_nodes.h"
#include "raylab/texture_nodes.h"
#include "raylab/bxdf_nodes.h"
#include "raylab/sampler_nodes.h"
#include "raylab/mapping_nodes.h"
#include "raylab/noise_nodes.h"

#include <blueprint/Connecting.h>
#include <blueprint/Evaluator.h>
#include <blueprint/node/Hub.h>
#include <blueprint/node/For.h>
#include <blueprint/node/For2.h>
#include <blueprint/node/Vector1.h>
#include <blueprint/node/Vector3.h>

#include <raytracing/world/World.h>
#include <raytracing/light/Light.h>
#include <raytracing/light/Ambient.h>
#include <raytracing/light/Directional.h>
#include <raytracing/light/PointLight.h>
#include <raytracing/light/AreaLight.h>
#include <raytracing/light/AmbientOccluder.h>
#include <raytracing/light/EnvironmentLight.h>
#include <raytracing/light/FakeSphericalLight.h>
#include <raytracing/tracer/RayCast.h>
#include <raytracing/tracer/AreaLighting.h>
#include <raytracing/tracer/FirstHit.h>
#include <raytracing/tracer/Whitted.h>
#include <raytracing/tracer/PathTrace.h>
#include <raytracing/tracer/GlobalTrace.h>
#include <raytracing/camera/Pinhole.h>
#include <raytracing/camera/ThinLens.h>
#include <raytracing/camera/FishEye.h>
#include <raytracing/camera/Spherical.h>
#include <raytracing/camera/Stereo.h>
#include <raytracing/camera/Orthographic.h>
#include <raytracing/primitive/Box.h>
#include <raytracing/primitive/Sphere.h>
#include <raytracing/primitive/Plane.h>
#include <raytracing/primitive/Rectangle.h>
#include <raytracing/primitive/Triangle.h>
#include <raytracing/primitive/WireframeBox.h>
#include <raytracing/primitive/GeoInstance.h>
#include <raytracing/primitive/Disk.h>
#include <raytracing/primitive/SolidCylinder.h>
#include <raytracing/primitive/ConvexPartCylinder.h>
#include <raytracing/primitive/OpenPartCylinder.h>
#include <raytracing/primitive/Grid.h>
#include <raytracing/primitive/ConcaveSphere.h>
#include <raytracing/primitive/Torus.h>
#include <raytracing/primitive/OpenCone.h>
#include <raytracing/primitive/ConvexPartSphere.h>
#include <raytracing/primitive/ConcavePartSphere.h>
#include <raytracing/primitive/FlatRimmedBowl.h>
#include <raytracing/primitive/RoundRimmedBowl.h>
#include <raytracing/primitive/ThickRing.h>
#include <raytracing/primitive/BeveledCylinder.h>
#include <raytracing/primitive/BeveledRing.h>
#include <raytracing/primitive/BeveledBox.h>
#include <raytracing/primitive/BeveledWedge.h>
#include <raytracing/primitive/SolidCone.h>
#include <raytracing/primitive/OpenCylinder.h>
#include <raytracing/primitive/ConcavePartCylinder.h>
#include <raytracing/primitive/FishBowl.h>
#include <raytracing/primitive/GlassOfWater.h>
#include <raytracing/primitive/CutCube.h>
#include <raytracing/primitive/ConcaveLens.h>
#include <raytracing/primitive/ProductJar.h>
#include <raytracing/primitive/ConcaveHemisphere.h>
#include <raytracing/primitive/CutFace.h>
#include <raytracing/primitive/BeveledBoxShell.h>

#include <raytracing/material/Matte.h>
#include <raytracing/material/SV_Matte.h>
#include <raytracing/material/Emissive.h>
#include <raytracing/material/Phong.h>
#include <raytracing/material/Reflective.h>
#include <raytracing/material/SV_Emissive.h>
#include <raytracing/material/GlossyReflector.h>
#include <raytracing/material/Transparent.h>
#include <raytracing/material/Dielectric.h>
#include <raytracing/material/SphereMaterials.h>
#include <raytracing/material/SV_Phong.h>
#include <raytracing/material/SV_Reflector.h>
#include <raytracing/texture/Checker3D.h>
#include <raytracing/texture/ImageTexture.h>
#include <raytracing/texture/Image.h>
#include <raytracing/texture/ConeChecker.h>
#include <raytracing/texture/PlaneChecker.h>
#include <raytracing/texture/SphereChecker.h>
#include <raytracing/texture/ConstantColor.h>
#include <raytracing/texture/WrappedFBmTexture.h>
#include <raytracing/texture/RampFBmTexture.h>
#include <raytracing/texture/TextureInstance.h>
#include <raytracing/texture/Wood.h>
#include <raytracing/texture/TurbulenceTexture.h>
#include <raytracing/texture/WrappedTwoColors.h>
#include <raytracing/texture/WrappedThreeColors.h>
#include <raytracing/texture/NestedNoisesTexture.h>
#include <raytracing/texture/WrappedRamp.h>
#include <raytracing/texture/SphereTextures.h>
#include <raytracing/texture/FBmTexture.h>
#include <raytracing/texture/CylinderChecker.h>
#include <raytracing/texture/RectangleChecker.h>
#include <raytracing/texture/DiskChecker.h>
#include <raytracing/bxdf/PerfectSpecular.h>
#include <raytracing/sampler/Jittered.h>
#include <raytracing/sampler/MultiJittered.h>
#include <raytracing/sampler/Regular.h>
#include <raytracing/sampler/PureRandom.h>
#include <raytracing/sampler/Hammersley.h>
#include <raytracing/mapping/SphericalMap.h>
#include <raytracing/mapping/LightProbe.h>
#include <raytracing/mapping/SquareMap.h>
#include <raytracing/mapping/CylindricalMap.h>
#include <raytracing/mapping/HemisphericalMap.h>
#include <raytracing/noise/LinearNoise.h>
#include <raytracing/noise/CubicNoise.h>

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

void init_phong(const raylab::node::Phong& src, rt::Phong& dst)
{
    dst.SetKa(src.ka);
    dst.SetKd(src.kd);
    dst.SetKs(src.ks);
    dst.SetCd(to_rt_color(src.cd));
    dst.SetCs(to_rt_color(src.cs));
    dst.SetExp(src.exp);
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
        std::vector<std::unique_ptr<rt::GeoPrimitive>> objects;
        CreateObjects(*object_conns[0], objects);
        for (auto& obj : objects) {
            dst.AddObject(std::move(obj));
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
                int num = static_cast<int>(bp::Evaluator::CalcFloat(*num_conns[0]));
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

    dst.SetBackgroundColor(to_rt_color(src.background_color));
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
            std::shared_ptr<rt::GeoPrimitive> obj = CreateObject(conns[0]->GetFrom()->GetParent());
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
            std::shared_ptr<rt::Sampler> sampler = CreateSampler(conns[0]->GetFrom()->GetParent());
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
            std::shared_ptr<rt::Sampler> sampler = CreateSampler(samp_conns[0]->GetFrom()->GetParent());
            if (sampler) {
                light->SetSampler(sampler);
            }
        }

        auto& mat_conns = node.GetAllInput()[node::EnvironmentLight::ID_MATERIAL]->GetConnecting();
        if (!mat_conns.empty()) {
            std::shared_ptr<rt::Material> material = CreateMaterial(mat_conns[0]->GetFrom()->GetParent());
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
    else if (tracer_type == rttr::type::get<node::Whitted>())
    {
        auto& src_tracer = static_cast<const node::Whitted&>(node);
        auto tracer = std::make_unique<rt::Whitted>(dst);
        dst_tracer = std::move(tracer);
    }
    else if (tracer_type == rttr::type::get<node::PathTrace>())
    {
        auto& src_tracer = static_cast<const node::PathTrace&>(node);
        auto tracer = std::make_unique<rt::PathTrace>(dst);
        dst_tracer = std::move(tracer);
    }
    else if (tracer_type == rttr::type::get<node::GlobalTrace>())
    {
        auto& src_tracer = static_cast<const node::GlobalTrace&>(node);
        auto tracer = std::make_unique<rt::GlobalTrace>(dst);
        dst_tracer = std::move(tracer);
    }
    else
    {
        assert(0);
    }

    return dst_tracer;
}

void Evaluator::CreateObjects(const bp::Connecting& conn, std::vector<std::unique_ptr<rt::GeoPrimitive>>& objects)
{
    auto& object_node = conn.GetFrom()->GetParent();
    auto object_type = object_node.get_type();
    if (object_type == rttr::type::get<bp::node::Hub>())
    {
        auto& hub = static_cast<const bp::node::Hub&>(object_node);
        for (auto& input : hub.GetAllInput()) {
            for (auto& c : input->GetConnecting()) {
                CreateObjects(*c, objects);
            }
        }
    }
    else if (object_type == rttr::type::get<bp::node::For>())
    {
        auto& f = static_cast<const bp::node::For&>(object_node);
        auto& conns = f.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            for (int i = f.index_begin; i < f.index_end; i += f.index_step) 
            {
                const_cast<bp::node::For&>(f).index_curr = i;
                CreateObjects(*conns[0], objects);
            }
        }
    }
    else if (object_type == rttr::type::get<bp::node::For2>())
    {
        auto& f = static_cast<const bp::node::For2&>(object_node);
        auto& conns = f.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) 
        {
            for (int i = f.i_begin; i < f.i_end; i += f.i_step) 
            {
                const_cast<bp::node::For2&>(f).i_curr = i;
                for (int j = f.j_begin; j < f.j_end; j += f.j_step)
                {
                    const_cast<bp::node::For2&>(f).j_curr = j;
                    CreateObjects(*conns[0], objects);
                }
            }
        }
    }
    else if (object_type.is_derived_from<node::GeoPrimitive>())
    {
        if (auto obj = CreateObject(object_node)) {
            objects.push_back(std::move(obj));
        }
    }
}

std::unique_ptr<rt::GeoPrimitive>
Evaluator::CreateObject(const bp::Node& node)
{
    std::unique_ptr<rt::GeoPrimitive> dst_object = nullptr;

    bool need_set_material = true;
    auto set_material = [](const bp::Node& src, rt::GeoPrimitive& dst)
    {
        auto& material_conns = src.GetAllInput()[0]->GetConnecting();
        if (!material_conns.empty())
        {
            std::shared_ptr<rt::Material> dst_material = CreateMaterial(material_conns[0]->GetFrom()->GetParent());
            if (dst_material) {
                dst.SetMaterial(dst_material);
            }
        }
    };

    auto object_type = node.get_type();
    if (object_type == rttr::type::get<node::Compound>())
    {
        auto& src_object = static_cast<const node::Compound&>(node);
        auto object = std::make_unique<rt::Compound>();

        auto& conns = node.GetAllInput()[node::Compound::ID_CHILDREN]->GetConnecting();
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

        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::Box>())
    {
        auto& src_object = static_cast<const node::Box&>(node);

        sm::vec3 min = src_object.min;
        sm::vec3 max = src_object.max;

        auto& conns_min = node.GetAllInput()[node::Box::ID_MIN]->GetConnecting();
        if (!conns_min.empty()) {
            min = bp::Evaluator::CalcFloat3(*conns_min[0]);
        }

        auto& conns_max = node.GetAllInput()[node::Box::ID_MAX]->GetConnecting();
        if (!conns_max.empty()) {
            max = bp::Evaluator::CalcFloat3(*conns_max[0]);
        }

        auto object = std::make_unique<rt::Box>(to_rt_p3d(min), to_rt_p3d(max));
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::Sphere>())
    {
        auto& src_object = static_cast<const node::Sphere&>(node);
        auto object = std::make_unique<rt::Sphere>();

        object->SetCenter(to_rt_p3d(src_object.center));
        object->SetRadius(src_object.radius);

        auto& conns = node.GetAllInput()[node::Sphere::ID_SAMPLER]->GetConnecting();
        if (!conns.empty()) {
            object->SetSampler(CreateSampler(conns[0]->GetFrom()->GetParent()));
        }

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
            p0 = to_rt_p3d(bp::Evaluator::CalcFloat3(*conns_p0[0]));
        } else {
            p0 = to_rt_p3d(src_object.p0);
        }
        auto a    = to_rt_v3d(src_object.a);
        auto b    = to_rt_v3d(src_object.b);
        auto norm = to_rt_normal(src_object.normal);
        std::unique_ptr<rt::Rectangle> object = nullptr;
        if (norm.x == 0 && norm.x == 0 && norm.y == 0) {
            object = std::make_unique<rt::Rectangle>(p0, a, b);
        } else {
            object = std::make_unique<rt::Rectangle>(p0, a, b, norm);
        }

        auto& conns_sampler = node.GetAllInput()[node::Rectangle::ID_SAMPLER]->GetConnecting();
        if (!conns_sampler.empty()) {
            if (std::shared_ptr<rt::Sampler> sampler =
                CreateSampler(conns_sampler[0]->GetFrom()->GetParent())) {
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
    else if (object_type == rttr::type::get<node::GeoInstance>())
    {
        auto& src_object = static_cast<const node::GeoInstance&>(node);

        std::unique_ptr<rt::GeoInstance> object = nullptr;
        auto& conns = node.GetAllInput()[node::GeoInstance::ID_OBJECT]->GetConnecting();
        if (conns.empty()) {
            object = std::make_unique<rt::GeoInstance>();
        } else {
            object = std::make_unique<rt::GeoInstance>(CreateObject(conns[0]->GetFrom()->GetParent()));
        }
        //for (auto& op : src_object.ops)
        //{
        //    switch (op.type)
        //    {
        //    case node::GeoInstance::OpType::Scale:
        //        object->Scale(to_rt_v3d(op.val));
        //        break;
        //    case node::GeoInstance::OpType::Rotate:
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
        //    case node::GeoInstance::OpType::Translate:
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

        object->SetTransformTexture(src_object.transform_tex);

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
            std::vector<std::unique_ptr<rt::GeoPrimitive>> objects;
            CreateObjects(*conns[0], objects);
            for (auto& obj : objects) {
                object->AddObject(std::move(obj));
            }
        }

        object->SetupCells();

        // todo
        // need_set_material = false;

        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::ConcaveSphere>())
    {
        auto& src_object = static_cast<const node::ConcaveSphere&>(node);
        auto object = std::make_unique<rt::ConcaveSphere>(
            to_rt_p3d(src_object.center), src_object.radius
        );
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
    else if (object_type == rttr::type::get<node::ConcavePartSphere>())
    {
        auto& src_object = static_cast<const node::ConcavePartSphere&>(node);
        auto center = to_rt_p3d(src_object.center);
        auto object = std::make_unique<rt::ConcavePartSphere>(
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
    else if (object_type == rttr::type::get<node::SolidCone>())
    {
        auto& src_object = static_cast<const node::SolidCone&>(node);
        auto object = std::make_unique<rt::SolidCone>(src_object.radius, src_object.height);
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::OpenCylinder>())
    {
        auto& src_object = static_cast<const node::OpenCylinder&>(node);
        auto object = std::make_unique<rt::OpenCylinder>(src_object.bottom, src_object.top, src_object.radius);
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::ConcavePartCylinder>())
    {
        auto& src_object = static_cast<const node::ConcavePartCylinder&>(node);
        auto object = std::make_unique<rt::ConcavePartCylinder>(
            src_object.bottom, src_object.top, src_object.radius,
            src_object.polar_min, src_object.polar_max
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::FishBowl>())
    {
        auto& src_object = static_cast<const node::FishBowl&>(node);
        auto object = std::make_unique<rt::FishBowl>(
            src_object.inner_radius, src_object.glass_thickness, src_object.water_depth,
            src_object.meniscus_radius, src_object.opening_angle
        );

        auto& conns_ga = node.GetAllInput()[node::FishBowl::ID_GLASS_AIR_MATERIAL]->GetConnecting();
        if (!conns_ga.empty()) {
            object->SetGlassAirMaterial(CreateMaterial(conns_ga[0]->GetFrom()->GetParent()));
        }

        auto& conns_wa = node.GetAllInput()[node::FishBowl::ID_WATER_AIR_MATERIAL]->GetConnecting();
        if (!conns_wa.empty()) {
            object->SetWaterAirMaterial(CreateMaterial(conns_wa[0]->GetFrom()->GetParent()));
        }

        auto& conns_wg = node.GetAllInput()[node::FishBowl::ID_WATER_GLASS_MATERIAL]->GetConnecting();
        if (!conns_wg.empty()) {
            object->SetWaterGlassMaterial(CreateMaterial(conns_wg[0]->GetFrom()->GetParent()));
        }

        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::GlassOfWater>())
    {
        auto& src_object = static_cast<const node::GlassOfWater&>(node);
        auto object = std::make_unique<rt::GlassOfWater>(
            src_object.height, src_object.inner_radius, src_object.wall_thickness,
            src_object.base_thickness, src_object.water_height, src_object.meniscus_radius
        );

        auto& conns_ga = node.GetAllInput()[node::GlassOfWater::ID_GLASS_AIR_MATERIAL]->GetConnecting();
        if (!conns_ga.empty()) {
            object->SetGlassAirMaterial(CreateMaterial(conns_ga[0]->GetFrom()->GetParent()));
        }

        auto& conns_wa = node.GetAllInput()[node::GlassOfWater::ID_WATER_AIR_MATERIAL]->GetConnecting();
        if (!conns_wa.empty()) {
            object->SetWaterAirMaterial(CreateMaterial(conns_wa[0]->GetFrom()->GetParent()));
        }

        auto& conns_wg = node.GetAllInput()[node::GlassOfWater::ID_WATER_GLASS_MATERIAL]->GetConnecting();
        if (!conns_wg.empty()) {
            object->SetWaterGlassMaterial(CreateMaterial(conns_wg[0]->GetFrom()->GetParent()));
        }

        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::CutCube>())
    {
        auto& src_object = static_cast<const node::CutCube&>(node);
        auto object = std::make_unique<rt::CutCube>(
            to_rt_p3d(src_object.p0), to_rt_p3d(src_object.p1), src_object.radius
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::ConcaveLens>())
    {
        auto& src_object = static_cast<const node::ConcaveLens&>(node);
        auto object = std::make_unique<rt::ConcaveLens>(
            src_object.radius, src_object.thickness, src_object.min_distance
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::ProductJar>())
    {
        auto& src_object = static_cast<const node::ProductJar&>(node);
        auto object = std::make_unique<rt::ProductJar>(
            src_object.bottom, src_object.body_top, src_object.cap_top, src_object.body_radius,
            src_object.bottom_bevel_radius, src_object.top_bevel_radius, src_object.cap_bevel_radius
        );

        need_set_material = false;
        set_material(node, *object);

        auto& conns = node.GetAllInput()[node::ProductJar::ID_BODY_MATERIAL]->GetConnecting();
        if (!conns.empty()) {
            object->SetBodyMaterial(CreateMaterial(conns[0]->GetFrom()->GetParent()));
        }

        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::ConcaveHemisphere>())
    {
        auto& src_object = static_cast<const node::ConcaveHemisphere&>(node);
        auto object = std::make_unique<rt::ConcaveHemisphere>(
            to_rt_p3d(src_object.center), src_object.radius
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::CutFace>())
    {
        auto& src_object = static_cast<const node::CutFace&>(node);
        auto object = std::make_unique<rt::CutFace>(
            src_object.size, src_object.radius
        );
        dst_object = std::move(object);
    }
    else if (object_type == rttr::type::get<node::BeveledBoxShell>())
    {
        auto& src_object = static_cast<const node::BeveledBoxShell&>(node);
        auto object = std::make_unique<rt::BeveledBoxShell>(
            to_rt_p3d(src_object.bottom), to_rt_p3d(src_object.top), src_object.bevel_radius
        );
        dst_object = std::move(object);
    }
    else
    {
        assert(0);
    }

    dst_object->SetShadows(static_cast<const node::GeoPrimitive&>(node).shadows);

    if (need_set_material) {
        set_material(node, *dst_object);
    }

    return dst_object;
}

std::unique_ptr<rt::Material>
Evaluator::CreateMaterial(const bp::Node& node)
{
    std::unique_ptr<rt::Material> dst_material = nullptr;

    auto material_type = node.get_type();
    if (material_type == rttr::type::get<node::Matte>())
    {
        auto& src_material = static_cast<const node::Matte&>(node);
        auto material = std::make_unique<rt::Matte>();

        auto& inputs = node.GetAllInput();

        float ka = src_material.ka;
        auto& ka_conns = inputs[node::Matte::ID_KA]->GetConnecting();
        if (!ka_conns.empty()) {
            ka = bp::Evaluator::CalcFloat(*ka_conns[0]);
        }

        float kd = src_material.kd;
        auto& kd_conns = inputs[node::Matte::ID_KD]->GetConnecting();
        if (!kd_conns.empty()) {
            kd = bp::Evaluator::CalcFloat(*kd_conns[0]);
        }

        auto cd = src_material.cd;
        auto& cd_conns = inputs[node::Matte::ID_CD]->GetConnecting();
        if (!cd_conns.empty()) {
            auto v3 = bp::Evaluator::CalcFloat3(*cd_conns[0]);
            cd.FromFloat(v3.x, v3.y, v3.z);
        }

        auto& sp_conns = inputs[node::Matte::ID_SAMPLER]->GetConnecting();
        if (!sp_conns.empty()) {
            material->SetSampler(CreateSampler(sp_conns[0]->GetFrom()->GetParent()));
        }

        material->SetKa(ka);
        material->SetKd(kd);
        material->SetCd(to_rt_color(cd));

        dst_material = std::move(material);
    }
    else if (material_type == rttr::type::get<node::SV_Matte>())
    {
        auto& src_material = static_cast<const node::SV_Matte&>(node);
        auto material = std::make_unique<rt::SV_Matte>();

        auto& inputs = node.GetAllInput();

        float ka = src_material.ka;
        auto& ka_conns = inputs[node::SV_Matte::ID_KA]->GetConnecting();
        if (!ka_conns.empty()) {
            ka = bp::Evaluator::CalcFloat(*ka_conns[0]);
        }

        float kd = src_material.kd;
        auto& kd_conns = inputs[node::SV_Matte::ID_KD]->GetConnecting();
        if (!kd_conns.empty()) {
            kd = bp::Evaluator::CalcFloat(*kd_conns[0]);
        }

        std::shared_ptr<rt::Texture> tex = nullptr;
        auto& tex_conns = inputs[node::SV_Matte::ID_CD]->GetConnecting();
        if (!tex_conns.empty()) {
            tex = CreateTexture(tex_conns[0]->GetFrom()->GetParent());
        }

        material->SetKa(ka);
        material->SetKd(kd);
        material->SetCd(tex);

        dst_material = std::move(material);
    }
    else if (material_type == rttr::type::get<node::Emissive>())
    {
        auto& src_material = static_cast<const node::Emissive&>(node);
        auto material = std::make_unique<rt::Emissive>();

        material->SetRadianceScaleFactor(src_material.radiance_scale_factor);
        material->SetCe(to_rt_color(src_material.color));

        dst_material = std::move(material);
    }
    else if (material_type == rttr::type::get<node::Phong>())
    {
        auto& src_material = static_cast<const node::Phong&>(node);
        auto material = std::make_unique<rt::Phong>();

        init_phong(src_material, *material);

        dst_material = std::move(material);
    }
    else if (material_type == rttr::type::get<node::Reflective>())
    {
        auto& src_material = static_cast<const node::Reflective&>(node);
        auto material = std::make_unique<rt::Reflective>();

        init_phong(src_material, *material);

        material->SetKr(src_material.kr);
        material->SetCr(to_rt_color(src_material.cr));

        dst_material = std::move(material);
    }
    else if (material_type == rttr::type::get<node::SV_Emissive>())
    {
        auto& src_material = static_cast<const node::SV_Emissive&>(node);
        auto material = std::make_unique<rt::SV_Emissive>();

        material->SetRadianceScaleFactor(src_material.radiance_scale_factor);
        material->SetCe(to_rt_color(src_material.color));

        auto& conns = src_material.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            material->SetTexture(CreateTexture(conns[0]->GetFrom()->GetParent()));
        }

        dst_material = std::move(material);
    }
    else if (material_type == rttr::type::get<node::GlossyReflector>())
    {
        auto& src_material = static_cast<const node::GlossyReflector&>(node);
        auto material = std::make_unique<rt::GlossyReflector>();

        init_phong(src_material, *material);

        material->SetKr(src_material.kr);
        material->SetCr(to_rt_color(src_material.cr));

        auto& conns = src_material.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            material->SetSampler(CreateSampler(conns[0]->GetFrom()->GetParent()), src_material.exp);
        } else {
            material->SetSamples(src_material.num_samples, src_material.exp);
        }

        dst_material = std::move(material);
    }
    else if (material_type == rttr::type::get<node::Transparent>())
    {
        auto& src_material = static_cast<const node::Transparent&>(node);
        auto material = std::make_unique<rt::Transparent>();

        init_phong(src_material, *material);

        material->SetIor(src_material.ior);
        material->SetKr(src_material.kr);
        material->SetKt(src_material.kt);

        dst_material = std::move(material);
    }
    else if (material_type == rttr::type::get<node::Dielectric>())
    {
        auto& src_material = static_cast<const node::Dielectric&>(node);
        auto material = std::make_unique<rt::Dielectric>();

        init_phong(src_material, *material);

        material->SetEtaIn(src_material.eta_in);
        material->SetEtaOut(src_material.eta_out);
        material->SetCfIn(to_rt_color(src_material.cf_in));
        material->SetCfOut(to_rt_color(src_material.cf_out));

        dst_material = std::move(material);
    }
    else if (material_type == rttr::type::get<node::SphereMaterials>())
    {
        auto& src_material = static_cast<const node::SphereMaterials&>(node);
        auto material = std::make_unique<rt::SphereMaterials>();

        material->SetNumHorizontal(src_material.num_horizontal_checkers);
        material->SetNumVertical(src_material.num_vertical_checkers);

        material->SetHoriLineWidth(src_material.horizontal_line_width);
        material->SetVertLineWidth(src_material.vertical_line_width);

        material->SetChecker1Color(to_rt_color(src_material.color1));
        material->SetChecker2Color(to_rt_color(src_material.color2));
        material->SetLineColor(to_rt_color(src_material.line_color));

        auto& c1_conns = src_material.GetAllInput()[node::SphereMaterials::ID_CHECKER1_MATERIAL]->GetConnecting();
        if (!c1_conns.empty()) {
            material->SetChecker1Material(CreateMaterial(c1_conns[0]->GetFrom()->GetParent()));
        }
        auto& c2_conns = src_material.GetAllInput()[node::SphereMaterials::ID_CHECKER2_MATERIAL]->GetConnecting();
        if (!c2_conns.empty()) {
            material->SetChecker2Material(CreateMaterial(c2_conns[0]->GetFrom()->GetParent()));
        }
        auto& line_conns = src_material.GetAllInput()[node::SphereMaterials::ID_LINE_MATERIAL]->GetConnecting();
        if (!line_conns.empty()) {
            material->SetLineMaterial(CreateMaterial(line_conns[0]->GetFrom()->GetParent()));
        }

        dst_material = std::move(material);
    }
    else if (material_type == rttr::type::get<node::SV_Phong>())
    {
        auto& src_material = static_cast<const node::SV_Phong&>(node);
        auto material = std::make_unique<rt::SV_Phong>();

        material->SetKa(src_material.ka);
        material->SetKd(src_material.kd);
        material->SetKs(src_material.ks);
        material->SetExp(src_material.exp);

        auto& cd_conns = src_material.GetAllInput()[node::SV_Phong::ID_CD_TEXTURE]->GetConnecting();
        if (!cd_conns.empty()) {
            material->SetCd(CreateTexture(cd_conns[0]->GetFrom()->GetParent()));
        }
        auto& cs_conns = src_material.GetAllInput()[node::SV_Phong::ID_CS_TEXTURE]->GetConnecting();
        if (!cs_conns.empty()) {
            material->SetCs(CreateTexture(cs_conns[0]->GetFrom()->GetParent()));
        }

        dst_material = std::move(material);
    }
    else if (material_type == rttr::type::get<node::SV_Reflector>())
    {
        auto& src_material = static_cast<const node::SV_Reflector&>(node);
        auto material = std::make_unique<rt::SV_Reflector>();

        init_phong(src_material, *material);

        material->SetKr(src_material.kr);

        auto& conns = src_material.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            material->SetCr(CreateTexture(conns[0]->GetFrom()->GetParent()));
        }

        dst_material = std::move(material);
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
            std::shared_ptr<rt::Sampler> sampler = CreateSampler(conns[0]->GetFrom()->GetParent());
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
        camera->SetRectangular(src_camera.rectangular);
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

std::unique_ptr<rt::Texture>
Evaluator::CreateTexture(const bp::Node& node)
{
    std::unique_ptr<rt::Texture> dst_tex = nullptr;

    auto tex_type = node.get_type();
    if (tex_type == rttr::type::get<node::Checker3D>())
    {
        auto& src_tex = static_cast<const node::Checker3D&>(node);
        auto tex = std::make_unique<rt::Checker3D>();

        tex->SetColor1(to_rt_color(src_tex.col0));
        tex->SetColor2(to_rt_color(src_tex.col1));
        tex->SetSize(src_tex.size);

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::ImageTexture>())
    {
        auto& src_tex = static_cast<const node::ImageTexture&>(node);
        auto tex = std::make_unique<rt::ImageTexture>();

        auto& conns = node.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            tex->SetMapping(CreateMapping(conns[0]->GetFrom()->GetParent()));
        }

        auto image = std::make_shared<rt::Image>();
        image->ReadPPMFile(src_tex.filepath.c_str());
        tex->SetImage(image);

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::ConeChecker>())
    {
        auto& src_tex = static_cast<const node::ConeChecker&>(node);
        auto tex = std::make_unique<rt::ConeChecker>();

        tex->SetNumHorizontalCheckers(src_tex.num_horizontal_checkers);
        tex->SetNumVerticalCheckers(src_tex.num_vertical_checkers);
        tex->SetHorizontalLineWidth(src_tex.horizontal_line_width);
        tex->SetVerticalLineWidth(src_tex.vertical_line_width);

        tex->SetColor1(to_rt_color(src_tex.color1));
        tex->SetColor2(to_rt_color(src_tex.color2));
        tex->SetLineColor(to_rt_color(src_tex.line_color));

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::PlaneChecker>())
    {
        auto& src_tex = static_cast<const node::PlaneChecker&>(node);
        auto tex = std::make_unique<rt::PlaneChecker>();

        tex->SetOutlineWidth(src_tex.outline_width);
        tex->SetSize(src_tex.size);

        tex->SetColor1(to_rt_color(src_tex.color1));
        tex->SetColor2(to_rt_color(src_tex.color2));
        tex->SetOutlineColor(to_rt_color(src_tex.line_color));

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::SphereChecker>())
    {
        auto& src_tex = static_cast<const node::SphereChecker&>(node);
        auto tex = std::make_unique<rt::SphereChecker>();

        tex->SetNumHorizontalCheckers(src_tex.num_horizontal_checkers);
        tex->SetNumVerticalCheckers(src_tex.num_vertical_checkers);
        tex->SetHorizontalLineWidth(src_tex.horizontal_line_width);
        tex->SetVerticalLineWidth(src_tex.vertical_line_width);

        tex->SetColor1(to_rt_color(src_tex.color1));
        tex->SetColor2(to_rt_color(src_tex.color2));
        tex->SetLineColor(to_rt_color(src_tex.line_color));

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::ConstantColor>())
    {
        auto& src_tex = static_cast<const node::ConstantColor&>(node);
        auto tex = std::make_unique<rt::ConstantColor>();

        tex->SetColor(to_rt_color(src_tex.color));

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::WrappedFBmTexture>())
    {
        auto& src_tex = static_cast<const node::WrappedFBmTexture&>(node);
        auto tex = std::make_unique<rt::WrappedFBmTexture>(
            src_tex.min_value, src_tex.max_value, src_tex.expansion_number
        );

        tex->SetColor(to_rt_color(src_tex.color));

        auto& conns = node.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            tex->SetNoise(CreateNoise(conns[0]->GetFrom()->GetParent()));
        }

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::RampFBmTexture>())
    {
        auto& src_tex = static_cast<const node::RampFBmTexture&>(node);

        auto image = std::make_shared<rt::Image>();
        image->ReadPPMFile(src_tex.filepath.c_str());
        auto tex = std::make_unique<rt::RampFBmTexture>(
            image, src_tex.num_octaves, src_tex.fbm_amount
        );

        tex->SetLacunarity(src_tex.lacunarity);
        tex->SetGain(src_tex.gain);

        auto& conns = node.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            tex->SetNoise(CreateNoise(conns[0]->GetFrom()->GetParent()));
        }

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::TextureInstance>())
    {
        auto& src_tex = static_cast<const node::TextureInstance&>(node);
        auto tex = std::make_unique<rt::TextureInstance>();

        auto& conns = node.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            tex->SetTexture(CreateTexture(conns[0]->GetFrom()->GetParent()));
        }

        if (src_tex.scale != sm::vec3(1, 1, 1)) {
            tex->Scale(to_rt_v3d(src_tex.scale));
        }
        if (src_tex.rotate != sm::vec3(0, 0, 0))
        {
            if (src_tex.rotate.x != 0) {
                tex->RotateX(src_tex.rotate.x);
            }
            if (src_tex.rotate.y != 0) {
                tex->RotateY(src_tex.rotate.y);
            }
            if (src_tex.rotate.z != 0) {
                tex->RotateZ(src_tex.rotate.z);
            }
        }
        if (src_tex.translate != sm::vec3(0, 0, 0)) {
            tex->Translate(to_rt_v3d(src_tex.translate));
        }

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::Wood>())
    {
        auto& src_tex = static_cast<const node::Wood&>(node);
        auto tex = std::make_unique<rt::Wood>(
            to_rt_color(src_tex.light), to_rt_color(src_tex.dark)
        );
        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::TurbulenceTexture>())
    {
        auto& src_tex = static_cast<const node::TurbulenceTexture&>(node);

        auto tex = std::make_unique<rt::TurbulenceTexture>(
            to_rt_color(src_tex.color), src_tex.min_val, src_tex.max_val
        );

        auto& conns = node.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            tex->SetNoise(CreateNoise(conns[0]->GetFrom()->GetParent()));
        }

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::WrappedTwoColors>())
    {
        auto& src_tex = static_cast<const node::WrappedTwoColors&>(node);

        auto tex = std::make_unique<rt::WrappedTwoColors>(
            src_tex.min_val, src_tex.max_val, src_tex.exp
        );

        auto& conns = node.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            tex->SetNoise(CreateNoise(conns[0]->GetFrom()->GetParent()));
        }

        tex->SetColor1(to_rt_color(src_tex.color1));
        tex->SetColor2(to_rt_color(src_tex.color2));

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::WrappedThreeColors>())
    {
        auto& src_tex = static_cast<const node::WrappedThreeColors&>(node);

        auto tex = std::make_unique<rt::WrappedThreeColors>(
            src_tex.min_val, src_tex.max_val, src_tex.exp
        );

        auto& conns = node.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            tex->SetNoise(CreateNoise(conns[0]->GetFrom()->GetParent()));
        }

        tex->SetColor1(to_rt_color(src_tex.color1));
        tex->SetColor2(to_rt_color(src_tex.color2));
        tex->SetColor3(to_rt_color(src_tex.color3));

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::NestedNoisesTexture>())
    {
        auto& src_tex = static_cast<const node::NestedNoisesTexture&>(node);

        auto tex = std::make_unique<rt::NestedNoisesTexture>(
            src_tex.min_val, src_tex.max_val, src_tex.exp,
            src_tex.wrap_factor, to_rt_color(src_tex.color)
        );

        auto& noise_conns = node.GetAllInput()[node::NestedNoisesTexture::ID_NOISE]->GetConnecting();
        if (!noise_conns.empty()) {
            tex->SetNoise(CreateNoise(noise_conns[0]->GetFrom()->GetParent()));
        }

        auto& tex_conns = node.GetAllInput()[node::NestedNoisesTexture::ID_TEXTURE]->GetConnecting();
        if (!tex_conns.empty()) {
            tex->SetTexture(CreateTexture(tex_conns[0]->GetFrom()->GetParent()));
        }

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::WrappedRamp>())
    {
        auto& src_tex = static_cast<const node::WrappedRamp&>(node);

        std::shared_ptr<rt::Image> image = nullptr;
        if (!src_tex.filepath.empty()) {
            image = std::make_shared<rt::Image>();
            image->ReadPPMFile(src_tex.filepath.c_str());
        }
        auto tex = std::make_unique<rt::WrappedRamp>(
            image, src_tex.perturbation, src_tex.wrap_number
        );

        auto& conns = node.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            tex->SetNoise(CreateNoise(conns[0]->GetFrom()->GetParent()));
        }

        tex->SetColor(to_rt_color(src_tex.color));

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::SphereTextures>())
    {
        auto& src_tex = static_cast<const node::SphereTextures&>(node);
        auto tex = std::make_unique<rt::SphereTextures>();

        tex->SetNumHorizontalCheckers(src_tex.num_hori_checkers);
        tex->SetNumVerticalCheckers(src_tex.num_vert_checkers);

        auto& tex1_conns = node.GetAllInput()[node::SphereTextures::ID_TEXTURE1]->GetConnecting();
        if (!tex1_conns.empty()) {
            tex->SetTexture1(CreateTexture(tex1_conns[0]->GetFrom()->GetParent()));
        }

        auto& tex2_conns = node.GetAllInput()[node::SphereTextures::ID_TEXTURE2]->GetConnecting();
        if (!tex2_conns.empty()) {
            tex->SetTexture2(CreateTexture(tex2_conns[0]->GetFrom()->GetParent()));
        }

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::FBmTexture>())
    {
        std::shared_ptr<rt::LatticeNoise> noise = nullptr;
        auto& conns = node.GetAllInput()[0]->GetConnecting();
        if (!conns.empty()) {
            noise = CreateNoise(conns[0]->GetFrom()->GetParent());
        }

        auto& src_tex = static_cast<const node::FBmTexture&>(node);
        auto tex = std::make_unique<rt::FBmTexture>(noise);

        tex->SetColor(to_rt_color(src_tex.color));
        tex->SetMinValue(src_tex.min_value);
        tex->SetMaxValue(src_tex.max_value);

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::CylinderChecker>())
    {
        auto& src_tex = static_cast<const node::CylinderChecker&>(node);
        auto tex = std::make_unique<rt::CylinderChecker>();

        tex->SetNumHorizontalCheckers(src_tex.num_horizontal_checkers);
        tex->SetNumVerticalCheckers(src_tex.num_vertical_checkers);
        tex->SetHorizontalLineWidth(src_tex.horizontal_line_width);
        tex->SetVerticalLineWidth(src_tex.vertical_line_width);

        tex->SetColor1(to_rt_color(src_tex.color1));
        tex->SetColor2(to_rt_color(src_tex.color2));
        tex->SetLineColor(to_rt_color(src_tex.line_color));

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::RectangleChecker>())
    {
        auto& src_tex = static_cast<const node::RectangleChecker&>(node);
        auto tex = std::make_unique<rt::RectangleChecker>();

        tex->SetNumXCheckers(src_tex.num_x_checkers);
        tex->SetNumZCheckers(src_tex.num_z_checkers);
        tex->SetXLineWidth(src_tex.x_line_width);
        tex->SetZLineWidth(src_tex.z_line_width);

        tex->SetP0(to_rt_p3d(src_tex.p0));
        tex->SetA(to_rt_v3d(src_tex.a));
        tex->SetB(to_rt_v3d(src_tex.b));

        tex->SetColor1(to_rt_color(src_tex.color1));
        tex->SetColor2(to_rt_color(src_tex.color2));
        tex->SetLineColor(to_rt_color(src_tex.line_color));

        dst_tex = std::move(tex);
    }
    else if (tex_type == rttr::type::get<node::DiskChecker>())
    {
        auto& src_tex = static_cast<const node::DiskChecker&>(node);
        auto tex = std::make_unique<rt::DiskChecker>();

        tex->SetNumAngularCheckers(src_tex.num_angular_checkers);
        tex->SetNumRadialCheckers(src_tex.num_radial_checkers);
        tex->SetAngularLineWidth(src_tex.angular_line_width);
        tex->SetRadialLineWidth(src_tex.radial_line_width);

        tex->SetRadius(src_tex.radius);
        tex->SetCenter(to_rt_p3d(src_tex.center));

        tex->SetColor1(to_rt_color(src_tex.color1));
        tex->SetColor2(to_rt_color(src_tex.color2));
        tex->SetLineColor(to_rt_color(src_tex.line_color));

        dst_tex = std::move(tex);
    }

    return dst_tex;
}

std::unique_ptr<rt::BRDF>
Evaluator::CreateBRDF(const bp::Node& node)
{
    std::unique_ptr<rt::BRDF> dst_brdf = nullptr;

    auto tex_type = node.get_type();
    if (tex_type == rttr::type::get<node::PerfectSpecular>())
    {
        auto& src_brdf = static_cast<const node::PerfectSpecular&>(node);
        auto brdf = std::make_unique<rt::PerfectSpecular>();

        brdf->SetKr(src_brdf.kr);
        brdf->SetCr(to_rt_color(src_brdf.cr));

        dst_brdf = std::move(brdf);
    }

    return dst_brdf;
}

std::unique_ptr<rt::Sampler>
Evaluator::CreateSampler(const bp::Node& node)
{
    std::unique_ptr<rt::Sampler> dst_sampler = nullptr;

    int num_samples = 0;
    auto& conns = node.GetAllInput()[0]->GetConnecting();
    if (!conns.empty()) {
        num_samples = static_cast<int>(bp::Evaluator::CalcFloat(*conns[0]));
    }

    auto sampler_type = node.get_type();
    if (sampler_type == rttr::type::get<node::Jittered>())
    {
        auto& src_sampler = static_cast<const node::Jittered&>(node);
        if (num_samples == 0) {
            num_samples = src_sampler.num_samples;
        }
        auto sampler = std::make_unique<rt::Jittered>(num_samples);
        dst_sampler = std::move(sampler);
    }
    else if (sampler_type == rttr::type::get<node::MultiJittered>())
    {
        auto& src_sampler = static_cast<const node::MultiJittered&>(node);
        if (num_samples == 0) {
            num_samples = src_sampler.num_samples;
        }
        auto sampler = std::make_unique<rt::MultiJittered>(num_samples);
        dst_sampler = std::move(sampler);
    }
    else if (sampler_type == rttr::type::get<node::Regular>())
    {
        auto& src_sampler = static_cast<const node::Regular&>(node);
        if (num_samples == 0) {
            num_samples = src_sampler.num_samples;
        }
        auto sampler = std::make_unique<rt::Regular>(num_samples);
        dst_sampler = std::move(sampler);
    }
    else if (sampler_type == rttr::type::get<node::PureRandom>())
    {
        auto& src_sampler = static_cast<const node::PureRandom&>(node);
        if (num_samples == 0) {
            num_samples = src_sampler.num_samples;
        }
        auto sampler = std::make_unique<rt::PureRandom>(num_samples);
        dst_sampler = std::move(sampler);
    }
    else if (sampler_type == rttr::type::get<node::Hammersley>())
    {
        auto& src_sampler = static_cast<const node::Hammersley&>(node);
        if (num_samples == 0) {
            num_samples = src_sampler.num_samples;
        }
        auto sampler = std::make_unique<rt::Hammersley>(num_samples);
        dst_sampler = std::move(sampler);
    }

    return dst_sampler;
}

std::unique_ptr<rt::Mapping>
Evaluator::CreateMapping(const bp::Node& node)
{
    std::unique_ptr<rt::Mapping> dst_mapping = nullptr;

    auto mapping_type = node.get_type();
    if (mapping_type == rttr::type::get<node::SphericalMap>())
    {
        auto& src_mapping = static_cast<const node::SphericalMap&>(node);
        auto mapping = std::make_unique<rt::SphericalMap>();
        dst_mapping = std::move(mapping);
    }
    else if (mapping_type == rttr::type::get<node::LightProbe>())
    {
        auto& src_mapping = static_cast<const node::LightProbe&>(node);
        auto mapping = std::make_unique<rt::LightProbe>();

        switch (src_mapping.mapping_type)
        {
        case node::LightProbe::MapType::Regular:
            mapping->SetMapType(rt::LightProbe::MapType::Regular);
            break;
        case node::LightProbe::MapType::Panoramic:
            mapping->SetMapType(rt::LightProbe::MapType::Panoramic);
            break;
        }

        dst_mapping = std::move(mapping);
    }
    else if (mapping_type == rttr::type::get<node::SquareMap>())
    {
        auto& src_mapping = static_cast<const node::SquareMap&>(node);
        auto mapping = std::make_unique<rt::SquareMap>();
        dst_mapping = std::move(mapping);
    }
    else if (mapping_type == rttr::type::get<node::CylindricalMap>())
    {
        auto& src_mapping = static_cast<const node::CylindricalMap&>(node);
        auto mapping = std::make_unique<rt::CylindricalMap>();
        dst_mapping = std::move(mapping);
    }
    else if (mapping_type == rttr::type::get<node::HemisphericalMap>())
    {
        auto& src_mapping = static_cast<const node::HemisphericalMap&>(node);
        auto mapping = std::make_unique<rt::HemisphericalMap>();
        dst_mapping = std::move(mapping);
    }

    return dst_mapping;
}

std::unique_ptr<rt::LatticeNoise>
Evaluator::CreateNoise(const bp::Node& node)
{
    std::unique_ptr<rt::LatticeNoise> dst_noise = nullptr;

    auto noise_type = node.get_type();
    if (noise_type == rttr::type::get<node::LinearNoise>())
    {
        auto& src_noise = static_cast<const node::LinearNoise&>(node);
        auto noise = std::make_unique<rt::LinearNoise>(
            src_noise.octaves, src_noise.lacunarity, src_noise.gain
        );
        dst_noise = std::move(noise);
    }
    else if (noise_type == rttr::type::get<node::CubicNoise>())
    {
        auto& src_noise = static_cast<const node::CubicNoise&>(node);
        auto noise = std::make_unique<rt::CubicNoise>(
            src_noise.octaves, src_noise.lacunarity, src_noise.gain
        );
        dst_noise = std::move(noise);
    }

    return dst_noise;
}

}
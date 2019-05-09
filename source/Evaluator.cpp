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

#include <blueprint/Connecting.h>
#include <blueprint/node/Hub.h>
#include <blueprint/node/Vector1.h>

#include <raytracing/world/World.h>
#include <raytracing/lights/Light.h>
#include <raytracing/lights/Ambient.h>
#include <raytracing/lights/Directional.h>
#include <raytracing/lights/PointLight.h>
#include <raytracing/lights/AreaLight.h>
#include <raytracing/tracer/RayCast.h>
#include <raytracing/tracer/AreaLighting.h>
#include <raytracing/cameras/Pinhole.h>
#include <raytracing/cameras/ThinLens.h>
#include <raytracing/objects/Box.h>
#include <raytracing/objects/Sphere.h>
#include <raytracing/objects/Plane.h>
#include <raytracing/objects/Rectangle.h>
#include <raytracing/objects/Triangle.h>
#include <raytracing/materials/Matte.h>
#include <raytracing/materials/SV_Matte.h>
#include <raytracing/materials/Emissive.h>
#include <raytracing/materials/Phong.h>
#include <raytracing/materials/Reflective.h>
#include <raytracing/texture/Checker3D.h>
#include <raytracing/brdfs/PerfectSpecular.h>
#include <raytracing/samplers/Jittered.h>
#include <raytracing/samplers/MultiJittered.h>
#include <raytracing/samplers/Regular.h>

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
    if (!light_conns.empty()) {
        if (auto dst_light = CreateLight(light_conns[0]->GetFrom()->GetParent())) {
            dst.AddLight(std::move(dst_light));
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

            auto& sampler_conns = vp_node.GetAllInput()[0]->GetConnecting();
            if (!sampler_conns.empty()) {
                vp->SetSampler(CreateSampler(sampler_conns[0]->GetFrom()->GetParent()));
            } else {
                vp->SetSamples(src_vp.num_samples);
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

        auto p0   = to_rt_p3d(src_object.p0);
        auto a    = to_rt_v3d(src_object.a);
        auto b    = to_rt_v3d(src_object.b);
        auto norm = to_rt_normal(src_object.normal);
        auto object = std::make_unique<rt::Rectangle>(p0, a, b, norm);

        auto& conns = node.GetAllInput()[node::Rectangle::ID_SAMPLER]->GetConnecting();
        if (!conns.empty()) {
            if (auto sampler = CreateSampler(conns[0]->GetFrom()->GetParent())) {
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
    else
    {
        assert(0);
    }

    auto& src_camera = static_cast<const node::Camera&>(node);
    dst_camera->SetEye(to_rt_p3d(src_camera.pos));
    dst_camera->SetLookat(to_rt_p3d(src_camera.target));

    dst_camera->ComputeUVW();

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

    auto sampler_type = node.get_type();
    if (sampler_type == rttr::type::get<node::Jittered>())
    {
        auto& src_sampler = static_cast<const node::Jittered&>(node);
        auto sampler = std::make_shared<rt::Jittered>(src_sampler.num_samples);
        dst_sampler = sampler;
    }
    else if (sampler_type == rttr::type::get<node::MultiJittered>())
    {
        auto& src_sampler = static_cast<const node::MultiJittered&>(node);
        auto sampler = std::make_shared<rt::MultiJittered>(src_sampler.num_samples);
        dst_sampler = sampler;
    }
    else if (sampler_type == rttr::type::get<node::Regular>())
    {
        auto& src_sampler = static_cast<const node::Regular&>(node);
        auto sampler = std::make_shared<rt::Regular>(src_sampler.num_samples);
        dst_sampler = sampler;
    }

    return dst_sampler;
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

}
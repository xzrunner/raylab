#include "raylab/world_nodes.h"
#include "raylab/light_nodes.h"
#include "raylab/camera_nodes.h"
#include "raylab/tracer_nodes.h"
#include "raylab/object_nodes.h"
#include "raylab/material_nodes.h"
#include "raylab/texture_nodes.h"
#include "raylab/brdf_nodes.h"
#include "raylab/sampler_nodes.h"

#include <ee0/ReflectPropTypes.h>

RTTR_REGISTRATION
{

// World

rttr::registration::class_<raylab::node::World>("raylab::World")
    .constructor<>()
;

rttr::registration::class_<raylab::node::Output>("raylab::Output")
    .constructor<>()
;

rttr::registration::class_<raylab::node::ViewPlane>("raylab::ViewPlane")
    .constructor<>()
    .property("width", &raylab::node::ViewPlane::width)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Width"))
    )
    .property("height", &raylab::node::ViewPlane::height)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Height"))
    )
    .property("pixel_size", &raylab::node::ViewPlane::pixel_size)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("PixelSize"))
    )
    .property("num_samples", &raylab::node::ViewPlane::num_samples)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("NumSamples"))
    )
;

// Light

rttr::registration::class_<raylab::node::Light>("raylab::Light")
    .constructor<const std::string&>()
    .property("shadows", &raylab::node::Light::shadows)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Shadows"))
    )
;

rttr::registration::class_<raylab::node::Ambient>("raylab::Ambient")
    .constructor<>()
    .property("ls", &raylab::node::Ambient::radiant_scale)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radiant Scale"))
    )
    .property("color", &raylab::node::Ambient::color)
    (
        rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color"))
    )
;

rttr::registration::class_<raylab::node::Directional>("raylab::Directional")
    .constructor<>()
    .property("ls", &raylab::node::Directional::radiant_scale)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radiant Scale"))
    )
    .property("color", &raylab::node::Directional::color)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color"))
    )
    .property("dir", &raylab::node::Directional::dir)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Dir"))
    )
;

rttr::registration::class_<raylab::node::PointLight>("raylab::PointLight")
    .constructor<>()
    .property("ls", &raylab::node::PointLight::radiant_scale)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radiant Scale"))
    )
    .property("color", &raylab::node::PointLight::color)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color"))
    )
    .property("location", &raylab::node::PointLight::location)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Location"))
    )
;

rttr::registration::class_<raylab::node::AreaLight>("raylab::AreaLight")
    .constructor<>()
;

// Camera

rttr::registration::class_<raylab::node::Camera>("raylab::Camera")
    .property("pos", &raylab::node::Camera::pos)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Pos"))
    )
    .property("target", &raylab::node::Camera::target)
    (
        rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Target"))
    )
;

rttr::registration::class_<raylab::node::Pinhole>("raylab::Pinhole")
    .constructor<>()
    .property("dis", &raylab::node::Pinhole::dis)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Dis"))
    )
    .property("zoom", &raylab::node::Pinhole::zoom)
    (
        rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Zoom"))
    )
;

// Tracer

rttr::registration::class_<raylab::node::RayCast>("raylab::RayCast")
    .constructor<>()
;
rttr::registration::class_<raylab::node::AreaLighting>("raylab::AreaLighting")
    .constructor<>()
;

// Object

rttr::registration::class_<raylab::node::Box>("raylab::Box")
    .constructor<>()
    .property("min", &raylab::node::Box::min)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Min"))
    )
    .property("max", &raylab::node::Box::max)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Max"))
    )
;

rttr::registration::class_<raylab::node::Sphere>("raylab::Sphere")
    .constructor<>()
    .property("center", &raylab::node::Sphere::center)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Center"))
    )
    .property("radius", &raylab::node::Sphere::radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radius"))
    )
;

rttr::registration::class_<raylab::node::Plane>("raylab::Plane")
    .constructor<>()
    .property("pos", &raylab::node::Plane::pos)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Pos"))
    )
    .property("normal", &raylab::node::Plane::normal)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Normal"))
    )
;

rttr::registration::class_<raylab::node::Rectangle>("raylab::Rectangle")
    .constructor<>()
    .property("p0", &raylab::node::Rectangle::p0)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("P0"))
    )
    .property("a", &raylab::node::Rectangle::a)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("A"))
    )
    .property("b", &raylab::node::Rectangle::b)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("B"))
    )
    .property("normal", &raylab::node::Rectangle::normal)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Normal"))
    )
;

// Material

rttr::registration::class_<raylab::node::Matte>("raylab::Matte")
    .constructor<>()
    .property("ka", &raylab::node::Matte::ka)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("ka"))
    )
    .property("kd", &raylab::node::Matte::kd)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("kd"))
    )
    .property("cd", &raylab::node::Matte::cd)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("cd"))
    )
;

rttr::registration::class_<raylab::node::SV_Matte>("raylab::SV_Matte")
    .constructor<>()
    .property("ka", &raylab::node::SV_Matte::ka)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("ka"))
    )
    .property("kd", &raylab::node::SV_Matte::kd)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("kd"))
    )
;

rttr::registration::class_<raylab::node::Emissive>("raylab::Emissive")
    .constructor<>()
    .property("radiance_scale_factor", &raylab::node::Emissive::radiance_scale_factor)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("RadianceScaleFactor"))
    )
    .property("color", &raylab::node::Emissive::color)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color"))
    )
;

rttr::registration::class_<raylab::node::Phong>("raylab::Phong")
    .constructor<>()
    .property("ka", &raylab::node::Phong::ka)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("ka"))
    )
    .property("kd", &raylab::node::Phong::kd)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("kd"))
    )
    .property("ks", &raylab::node::Phong::ks)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("ks"))
    )
    .property("cd", &raylab::node::Phong::cd)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("cd"))
    )
    .property("cs", &raylab::node::Phong::cs)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("cs"))
    )
    .property("exp", &raylab::node::Phong::exp)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("exp"))
    )
;

rttr::registration::class_<raylab::node::Reflective>("raylab::Reflective")
    .constructor<>()
    .property("kr", &raylab::node::Reflective::kr)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("kr"))
    )
    .property("cr", &raylab::node::Reflective::cr)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("cr"))
    )
;

// texture

rttr::registration::class_<raylab::node::Checker3D>("raylab::Checker3D")
    .constructor<>()
    .property("col0", &raylab::node::Checker3D::col0)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color 0"))
    )
    .property("col1", &raylab::node::Checker3D::col1)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color 1"))
    )
    .property("size", &raylab::node::Checker3D::size)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Size"))
    )
;

// brdf

rttr::registration::class_<raylab::node::PerfectSpecular>("raylab::PerfectSpecular")
    .constructor<>()
    .property("kr", &raylab::node::PerfectSpecular::kr)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("kr"))
    )
    .property("cr", &raylab::node::PerfectSpecular::cr)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("cr"))
    )
;

// sampler

rttr::registration::class_<raylab::node::Jittered>("raylab::Jittered")
    .constructor<>()
;

rttr::registration::class_<raylab::node::MultiJittered>("raylab::MultiJittered")
    .constructor<>()
;

rttr::registration::class_<raylab::node::Regular>("raylab::Regular")
    .constructor<>()
;

}

namespace raylab
{

void regist_rttr()
{
}

}
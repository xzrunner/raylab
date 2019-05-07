#include "raylab/world_nodes.h"
#include "raylab/light_nodes.h"
#include "raylab/camera_nodes.h"
#include "raylab/tracer_nodes.h"
#include "raylab/object_nodes.h"
#include "raylab/material_nodes.h"

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

}

namespace raylab
{

void regist_rttr()
{
}

}
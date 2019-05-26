#include "raylab/world_nodes.h"
#include "raylab/light_nodes.h"
#include "raylab/camera_nodes.h"
#include "raylab/tracer_nodes.h"
#include "raylab/object_nodes.h"
#include "raylab/material_nodes.h"
#include "raylab/texture_nodes.h"
#include "raylab/brdf_nodes.h"
#include "raylab/sampler_nodes.h"
#include "raylab/mapping_nodes.h"
#include "raylab/noise_nodes.h"

#include <ee0/ReflectPropTypes.h>

#include <js/RTTR.h>

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
    .property("max_depth", &raylab::node::ViewPlane::max_depth)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Max Depth"))
    )
    .property("show_out_of_gamut", &raylab::node::ViewPlane::show_out_of_gamut)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("ShowOutOfGamut"))
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

rttr::registration::class_<raylab::node::AmbientOccluder>("raylab::AmbientOccluder")
    .constructor<>()
    .property("scale_radiance", &raylab::node::AmbientOccluder::scale_radiance)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("ScaleRadiance"))
    )
    .property("min_amount", &raylab::node::AmbientOccluder::min_amount)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("MinAmount"))
    )
;

rttr::registration::class_<raylab::node::EnvironmentLight>("raylab::EnvironmentLight")
    .constructor<>()
;

rttr::registration::class_<raylab::node::FakeSphericalLight>("raylab::FakeSphericalLight")
    .constructor<>()
    .property("ls", &raylab::node::FakeSphericalLight::scale_radiance)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radiant Scale"))
    )
    .property("jitter_amount", &raylab::node::FakeSphericalLight::jitter_amount)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Jitter Amount"))
    )
    .property("distance_attenuation", &raylab::node::FakeSphericalLight::distance_attenuation)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Distance Attenuation"))
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

rttr::registration::class_<raylab::node::ThinLens>("raylab::ThinLens")
    .constructor<>()
    .property("lens_radius", &raylab::node::ThinLens::lens_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("LensRadius"))
    )
    .property("d", &raylab::node::ThinLens::d)
    (
        rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("ViewPlaneDis"))
    )
    .property("f", &raylab::node::ThinLens::f)
    (
        rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("FocalDis"))
    )
    .property("zoom", &raylab::node::ThinLens::zoom)
    (
        rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Zoom"))
    )
;

rttr::registration::class_<raylab::node::FishEye>("raylab::FishEye")
    .constructor<>()
    .property("fov", &raylab::node::FishEye::fov)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Fov"))
    )
;

rttr::registration::class_<raylab::node::Spherical>("raylab::Spherical")
    .constructor<>()
    .property("hori_fov", &raylab::node::Spherical::hori_fov)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("HoriFov"))
    )
    .property("vert_fov", &raylab::node::Spherical::vert_fov)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("VertFov"))
    )
;

rttr::registration::enumeration<raylab::node::Stereo::ViewingType>("rlab_stereo_viewing_type")
(
	rttr::value("parallel",   raylab::node::Stereo::ViewingType::Parallel),
    rttr::value("Transverse", raylab::node::Stereo::ViewingType::Transverse)
);

rttr::registration::class_<raylab::node::Stereo>("raylab::Stereo")
    .constructor<>()
    .property("viewing_type", &raylab::node::Stereo::viewing_type)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("ViewingType"))
    )
    .property("pixel_gap", &raylab::node::Stereo::pixel_gap)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("PixelGap"))
    )
    .property("stereo_angle", &raylab::node::Stereo::stereo_angle)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("StereoAngle"))
    )
;

rttr::registration::class_<raylab::node::Orthographic>("raylab::Orthographic")
    .constructor<>()
    .property("dis", &raylab::node::Orthographic::dis)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Dis"))
    )
    .property("zoom", &raylab::node::Orthographic::zoom)
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

rttr::registration::class_<raylab::node::FirstHit>("raylab::FirstHit")
    .constructor<>()
;

rttr::registration::class_<raylab::node::Whitted>("raylab::Whitted")
    .constructor<>()
;

rttr::registration::class_<raylab::node::PathTrace>("raylab::PathTrace")
    .constructor<>()
;

rttr::registration::class_<raylab::node::GlobalTrace>("raylab::GlobalTrace")
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

rttr::registration::class_<raylab::node::Triangle>("raylab::Triangle")
    .constructor<>()
    .property("v0", &raylab::node::Triangle::v0)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("V0"))
    )
    .property("v1", &raylab::node::Triangle::v1)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("V1"))
    )
    .property("v2", &raylab::node::Triangle::v2)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("V2"))
    )
    .property("normal", &raylab::node::Triangle::normal)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Normal"))
    )
;

rttr::registration::class_<raylab::node::WireframeBox>("raylab::WireframeBox")
    .constructor<>()
    .property("p0", &raylab::node::WireframeBox::p0)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("P0"))
    )
    .property("p1", &raylab::node::WireframeBox::p1)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("P1"))
    )
    .property("br", &raylab::node::WireframeBox::bevel_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("BevelRadius"))
    )
;

rttr::registration::class_<raylab::node::Instance>("raylab::Instance")
    .constructor<>()
    .property("scale", &raylab::node::Instance::scale)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Scale"))
    )
    .property("rotate", &raylab::node::Instance::rotate)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Rotate"))
    )
    .property("translate", &raylab::node::Instance::translate)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Translate"))
    )
;

rttr::registration::class_<raylab::node::Disk>("raylab::Disk")
    .constructor<>()
    .property("center", &raylab::node::Disk::center)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Center"))
    )
    .property("normal", &raylab::node::Disk::normal)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Normal"))
    )
    .property("radius", &raylab::node::Disk::radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radius"))
    )
    .property("shadows", &raylab::node::Disk::shadows)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Shadows"))
    )
;

rttr::registration::class_<raylab::node::SolidCylinder>("raylab::SolidCylinder")
    .constructor<>()
    .property("bottom", &raylab::node::SolidCylinder::bottom)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Bottom"))
    )
    .property("top", &raylab::node::SolidCylinder::top)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Top"))
    )
    .property("radius", &raylab::node::SolidCylinder::radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radius"))
    )
;

rttr::registration::class_<raylab::node::ConvexPartCylinder>("raylab::ConvexPartCylinder")
    .constructor<>()
    .property("bottom", &raylab::node::ConvexPartCylinder::bottom)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Bottom"))
    )
    .property("top", &raylab::node::ConvexPartCylinder::top)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Top"))
    )
    .property("radius", &raylab::node::ConvexPartCylinder::radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radius"))
    )
    .property("polar_min", &raylab::node::ConvexPartCylinder::polar_min)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("PolarMin"))
    )
    .property("polar_max", &raylab::node::ConvexPartCylinder::polar_max)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("PolarMax"))
    )
;

rttr::registration::class_<raylab::node::OpenPartCylinder>("raylab::OpenPartCylinder")
    .constructor<>()
    .property("bottom", &raylab::node::OpenPartCylinder::bottom)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Bottom"))
    )
    .property("top", &raylab::node::OpenPartCylinder::top)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Top"))
    )
    .property("radius", &raylab::node::OpenPartCylinder::radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radius"))
    )
    .property("polar_min", &raylab::node::OpenPartCylinder::polar_min)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("PolarMin"))
    )
    .property("polar_max", &raylab::node::OpenPartCylinder::polar_max)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("PolarMax"))
    )
;

rttr::registration::enumeration<raylab::node::Grid::TriangleType>("rlab_grid_triangle_type")
(
	rttr::value("flat",   raylab::node::Grid::TriangleType::Flat),
    rttr::value("smooth", raylab::node::Grid::TriangleType::Smooth)
);

rttr::registration::class_<raylab::node::Grid>("raylab::Grid")
    .constructor<>()
    .property("filename", &raylab::node::Grid::filename)
    (
        rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Filename")),
        rttr::metadata(js::RTTR::FilePathTag(), true),
        rttr::metadata(ee0::PropOpenFileTag(), ee0::PropOpenFile("*.ply"))
    )
    .property("triangle_type", &raylab::node::Grid::triangle_type)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("TriangleType"))
    )
    .property("hori_steps", &raylab::node::Grid::hori_steps)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Hori Steps"))
    )
    .property("vert_steps", &raylab::node::Grid::vert_steps)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Vert Steps"))
    )
;

rttr::registration::class_<raylab::node::ConcaveSphere>("raylab::ConcaveSphere")
    .constructor<>()
    .property("center", &raylab::node::ConcaveSphere::center)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Center"))
    )
    .property("radius", &raylab::node::ConcaveSphere::radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radius"))
    )
    .property("shadows", &raylab::node::ConcaveSphere::shadows)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Shadows"))
    )
;

rttr::registration::class_<raylab::node::Torus>("raylab::Torus")
    .constructor<>()
    .property("swept_radius", &raylab::node::Torus::swept_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Swept Radius"))
    )
    .property("tube_radius", &raylab::node::Torus::tube_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Tube Radius"))
    )
;

rttr::registration::class_<raylab::node::OpenCone>("raylab::OpenCone")
    .constructor<>()
    .property("height", &raylab::node::OpenCone::height)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Height"))
    )
    .property("radius", &raylab::node::OpenCone::radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radius"))
    )
;

rttr::registration::class_<raylab::node::ConvexPartSphere>("raylab::ConvexPartSphere")
    .constructor<>()
    .property("center", &raylab::node::ConvexPartSphere::center)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Center"))
    )
    .property("radius", &raylab::node::ConvexPartSphere::radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radius"))
    )
    .property("azimuth_min", &raylab::node::ConvexPartSphere::azimuth_min)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Azimuth Min"))
    )
    .property("azimuth_max", &raylab::node::ConvexPartSphere::azimuth_max)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Azimuth Max"))
    )
    .property("polar_min", &raylab::node::ConvexPartSphere::polar_min)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Polar Min"))
    )
    .property("polar_max", &raylab::node::ConvexPartSphere::polar_max)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Polar Max"))
    )
;

rttr::registration::class_<raylab::node::ConcavePartSphere>("raylab::ConcavePartSphere")
    .constructor<>()
    .property("center", &raylab::node::ConcavePartSphere::center)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Center"))
    )
    .property("radius", &raylab::node::ConcavePartSphere::radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radius"))
    )
    .property("azimuth_min", &raylab::node::ConcavePartSphere::azimuth_min)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Azimuth Min"))
    )
    .property("azimuth_max", &raylab::node::ConcavePartSphere::azimuth_max)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Azimuth Max"))
    )
    .property("polar_min", &raylab::node::ConcavePartSphere::polar_min)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Polar Min"))
    )
    .property("polar_max", &raylab::node::ConcavePartSphere::polar_max)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Polar Max"))
    )
;

rttr::registration::class_<raylab::node::FlatRimmedBowl>("raylab::FlatRimmedBowl")
    .constructor<>()
    .property("inner_radius", &raylab::node::FlatRimmedBowl::inner_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Inner Radius"))
    )
    .property("outer_radius", &raylab::node::FlatRimmedBowl::outer_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Outer Radius"))
    )
;

rttr::registration::class_<raylab::node::RoundRimmedBowl>("raylab::RoundRimmedBowl")
    .constructor<>()
    .property("inner_radius", &raylab::node::RoundRimmedBowl::inner_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Inner Radius"))
    )
    .property("outer_radius", &raylab::node::RoundRimmedBowl::outer_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Outer Radius"))
    )
    .property("openning_angle", &raylab::node::RoundRimmedBowl::openning_angle)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Openning Angle"))
    )
;

rttr::registration::class_<raylab::node::ThickRing>("raylab::ThickRing")
    .constructor<>()
    .property("bottom", &raylab::node::ThickRing::bottom)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Bottom"))
    )
    .property("top", &raylab::node::ThickRing::top)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Top"))
    )
    .property("inner_radius", &raylab::node::ThickRing::inner_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Inner Radius"))
    )
    .property("outer_radius", &raylab::node::ThickRing::outer_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Outer Radius"))
    )
;

rttr::registration::class_<raylab::node::BeveledCylinder>("raylab::BeveledCylinder")
    .constructor<>()
    .property("bottom", &raylab::node::BeveledCylinder::bottom)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Bottom"))
    )
    .property("top", &raylab::node::BeveledCylinder::top)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Top"))
    )
    .property("radius", &raylab::node::BeveledCylinder::radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radius"))
    )
    .property("bevel_radius", &raylab::node::BeveledCylinder::bevel_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Bevel Radius"))
    )
;

rttr::registration::class_<raylab::node::BeveledRing>("raylab::BeveledRing")
    .constructor<>()
    .property("bottom", &raylab::node::BeveledRing::bottom)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Bottom"))
    )
    .property("top", &raylab::node::BeveledRing::top)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Top"))
    )
    .property("inner_radius", &raylab::node::BeveledRing::inner_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Inner Radius"))
    )
    .property("outer_radius", &raylab::node::BeveledRing::outer_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Outer Radius"))
    )
    .property("bevel_radius", &raylab::node::BeveledRing::bevel_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Bevel Radius"))
    )
;

rttr::registration::class_<raylab::node::BeveledBox>("raylab::BeveledBox")
    .constructor<>()
    .property("bottom", &raylab::node::BeveledBox::bottom)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Bottom"))
    )
    .property("top", &raylab::node::BeveledBox::top)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Top"))
    )
    .property("bevel_radius", &raylab::node::BeveledBox::bevel_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Bevel Radius"))
    )
;

rttr::registration::class_<raylab::node::BeveledWedge>("raylab::BeveledWedge")
    .constructor<>()
    .property("min_y", &raylab::node::BeveledWedge::min_y)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("MinY"))
    )
    .property("max_y", &raylab::node::BeveledWedge::max_y)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("MaxY"))
    )
    .property("inner_radius", &raylab::node::BeveledWedge::inner_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Inner Radius"))
    )
    .property("outer_radius", &raylab::node::BeveledWedge::outer_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Outer Radius"))
    )
    .property("bevel_radius", &raylab::node::BeveledWedge::bevel_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Bevel Radius"))
    )
    .property("min_azimuth", &raylab::node::BeveledWedge::min_azimuth)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Min Azimuth"))
    )
    .property("max_azimuth", &raylab::node::BeveledWedge::max_azimuth)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Max Azimuth"))
    )
;

rttr::registration::class_<raylab::node::SolidCone>("raylab::SolidCone")
    .constructor<>()
    .property("radius", &raylab::node::SolidCone::radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radius"))
    )
    .property("height", &raylab::node::SolidCone::height)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Height"))
    )
;

rttr::registration::class_<raylab::node::OpenCylinder>("raylab::OpenCylinder")
    .constructor<>()
    .property("bottom", &raylab::node::OpenCylinder::bottom)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Bottom"))
    )
    .property("top", &raylab::node::OpenCylinder::top)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Top"))
    )
    .property("radius", &raylab::node::OpenCylinder::radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radius"))
    )
;

rttr::registration::class_<raylab::node::ConcavePartCylinder>("raylab::ConcavePartCylinder")
    .constructor<>()
    .property("bottom", &raylab::node::ConcavePartCylinder::bottom)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Bottom"))
    )
    .property("top", &raylab::node::ConcavePartCylinder::top)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Top"))
    )
    .property("radius", &raylab::node::ConcavePartCylinder::radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radius"))
    )
    .property("polar_min", &raylab::node::ConcavePartCylinder::polar_min)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Polar Min"))
    )
    .property("polar_max", &raylab::node::ConcavePartCylinder::polar_max)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Polar Max"))
    )
;

rttr::registration::class_<raylab::node::FishBowl>("raylab::FishBowl")
    .constructor<>()
    .property("inner_radius", &raylab::node::FishBowl::inner_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Inner Radius"))
    )
    .property("glass_thickness", &raylab::node::FishBowl::glass_thickness)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Glass Thickness"))
    )
    .property("water_depth", &raylab::node::FishBowl::water_depth)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Water Depth"))
    )
    .property("meniscus_radius", &raylab::node::FishBowl::meniscus_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Meniscus Radius"))
    )
    .property("opening_angle", &raylab::node::FishBowl::opening_angle)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Opening Angle"))
    )
;

rttr::registration::class_<raylab::node::GlassOfWater>("raylab::GlassOfWater")
    .constructor<>()
    .property("height", &raylab::node::GlassOfWater::height)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Height"))
    )
    .property("inner_radius", &raylab::node::GlassOfWater::inner_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Inner Radius"))
    )
    .property("wall_thickness", &raylab::node::GlassOfWater::wall_thickness)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Wall Thickness"))
    )
    .property("base_thickness", &raylab::node::GlassOfWater::base_thickness)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Base Thickness"))
    )
    .property("water_height", &raylab::node::GlassOfWater::water_height)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Water Height"))
    )
    .property("meniscus_radius", &raylab::node::GlassOfWater::meniscus_radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Meniscus Radius"))
    )
;

rttr::registration::class_<raylab::node::CutCube>("raylab::CutCube")
    .constructor<>()
    .property("p0", &raylab::node::CutCube::p0)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("P0"))
    )
    .property("p1", &raylab::node::CutCube::p1)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("P1"))
    )
    .property("radius", &raylab::node::CutCube::radius)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Radius"))
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

rttr::registration::class_<raylab::node::SV_Emissive>("raylab::SV_Emissive")
    .constructor<>()
    .property("radiance_scale_factor", &raylab::node::SV_Emissive::radiance_scale_factor)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("RadianceScaleFactor"))
    )
    .property("color", &raylab::node::SV_Emissive::color)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color"))
    )
;

rttr::registration::class_<raylab::node::GlossyReflector>("raylab::GlossyReflector")
    .constructor<>()
    .property("kr", &raylab::node::GlossyReflector::kr)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("kr"))
    )
    .property("cr", &raylab::node::GlossyReflector::cr)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("cr"))
    )
    .property("num_samples", &raylab::node::GlossyReflector::num_samples)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Num Samples"))
    )
;

rttr::registration::class_<raylab::node::Transparent>("raylab::Transparent")
    .constructor<>()
    .property("ior", &raylab::node::Transparent::ior)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("ior"))
    )
    .property("kr", &raylab::node::Transparent::kr)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("kr"))
    )
    .property("kt", &raylab::node::Transparent::kt)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("kt"))
    )
;

rttr::registration::class_<raylab::node::Dielectric>("raylab::Dielectric")
    .constructor<>()
    .property("eta_in", &raylab::node::Dielectric::eta_in)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("EtaIn"))
    )
    .property("eta_out", &raylab::node::Dielectric::eta_out)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("EtaOut"))
    )
    .property("cf_in", &raylab::node::Dielectric::cf_in)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("CfIn"))
    )
    .property("cf_out", &raylab::node::Dielectric::cf_out)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("CfOut"))
    )
;

rttr::registration::class_<raylab::node::SphereMaterials>("raylab::SphereMaterials")
    .constructor<>()
    .property("num_hori_checkers", &raylab::node::SphereMaterials::num_horizontal_checkers)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Num Hori Checkers"))
    )
    .property("num_vert_checkers", &raylab::node::SphereMaterials::num_vertical_checkers)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Num Vert Checkers"))
    )
    .property("hori_line_width", &raylab::node::SphereMaterials::horizontal_line_width)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Hori Line Width"))
    )
    .property("vert_line_width", &raylab::node::SphereMaterials::vertical_line_width)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Vert Line Width"))
    )
    .property("color1", &raylab::node::SphereMaterials::color1)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color1"))
    )
    .property("color2", &raylab::node::SphereMaterials::color2)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color2"))
    )
    .property("line_color", &raylab::node::SphereMaterials::line_color)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Line Color"))
    )
;

rttr::registration::class_<raylab::node::SV_Phong>("raylab::SV_Phong")
    .constructor<>()
    .property("ka", &raylab::node::SV_Phong::ka)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("ka"))
    )
    .property("kd", &raylab::node::SV_Phong::kd)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("kd"))
    )
    .property("ks", &raylab::node::SV_Phong::ks)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("ks"))
    )
    .property("exp", &raylab::node::SV_Phong::exp)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("exp"))
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

rttr::registration::class_<raylab::node::ImageTexture>("raylab::ImageTexture")
    .constructor<>()
    .property("filepath", &raylab::node::ImageTexture::filepath)
    (
        rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Filepath")),
        rttr::metadata(js::RTTR::FilePathTag(), true),
        rttr::metadata(ee0::PropOpenFileTag(), ee0::PropOpenFile("*.ppm"))
    )
;

rttr::registration::class_<raylab::node::ConeChecker>("raylab::ConeChecker")
    .constructor<>()
    .property("num_horizontal_checkers", &raylab::node::ConeChecker::num_horizontal_checkers)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("NumHorizontalCheckers"))
    )
    .property("num_vertical_checkers", &raylab::node::ConeChecker::num_vertical_checkers)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("NumVerticalCheckers"))
    )
    .property("horizontal_line_width", &raylab::node::ConeChecker::horizontal_line_width)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("HorizontalLineWidth"))
    )
    .property("vertical_line_width", &raylab::node::ConeChecker::vertical_line_width)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("VerticalLineWidth"))
    )
    .property("color1", &raylab::node::ConeChecker::color1)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color1"))
    )
    .property("color2", &raylab::node::ConeChecker::color2)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color2"))
    )
    .property("line_color", &raylab::node::ConeChecker::line_color)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("LineColor"))
    )
;

rttr::registration::class_<raylab::node::PlaneChecker>("raylab::PlaneChecker")
    .constructor<>()
    .property("outline_width", &raylab::node::PlaneChecker::outline_width)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Outline Width"))
    )
    .property("size", &raylab::node::PlaneChecker::size)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Size"))
    )
    .property("color1", &raylab::node::PlaneChecker::color1)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color1"))
    )
    .property("color2", &raylab::node::PlaneChecker::color2)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color2"))
    )
    .property("line_color", &raylab::node::PlaneChecker::line_color)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("LineColor"))
    )
;

rttr::registration::class_<raylab::node::SphereChecker>("raylab::SphereChecker")
    .constructor<>()
    .property("num_horizontal_checkers", &raylab::node::SphereChecker::num_horizontal_checkers)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("NumHorizontalCheckers"))
    )
    .property("num_vertical_checkers", &raylab::node::SphereChecker::num_vertical_checkers)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("NumVerticalCheckers"))
    )
    .property("horizontal_line_width", &raylab::node::SphereChecker::horizontal_line_width)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("HorizontalLineWidth"))
    )
    .property("vertical_line_width", &raylab::node::SphereChecker::vertical_line_width)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("VerticalLineWidth"))
    )
    .property("color1", &raylab::node::SphereChecker::color1)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color1"))
    )
    .property("color2", &raylab::node::SphereChecker::color2)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color2"))
    )
    .property("line_color", &raylab::node::SphereChecker::line_color)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("LineColor"))
    )
;

rttr::registration::class_<raylab::node::ConstantColor>("raylab::ConstantColor")
    .constructor<>()
    .property("color", &raylab::node::ConstantColor::color)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color"))
    )
;

rttr::registration::class_<raylab::node::WrappedFBmTexture>("raylab::WrappedFBmTexture")
    .constructor<>()
    .property("max_value", &raylab::node::WrappedFBmTexture::max_value)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Max Value"))
    )
    .property("min_value", &raylab::node::WrappedFBmTexture::min_value)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Min Value"))
    )
    .property("expansion_number", &raylab::node::WrappedFBmTexture::expansion_number)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Expansion Number"))
    )
    .property("color", &raylab::node::WrappedFBmTexture::color)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color"))
    )
;

rttr::registration::class_<raylab::node::RampFBmTexture>("raylab::RampFBmTexture")
    .constructor<>()
    .property("filepath", &raylab::node::RampFBmTexture::filepath)
    (
        rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Filepath")),
        rttr::metadata(js::RTTR::FilePathTag(), true),
        rttr::metadata(ee0::PropOpenFileTag(), ee0::PropOpenFile("*.ppm"))
    )
    .property("num_octaves", &raylab::node::RampFBmTexture::num_octaves)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Num Octaves"))
    )
    .property("fbm_amount", &raylab::node::RampFBmTexture::fbm_amount)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Fbm Amount"))
    )
;

rttr::registration::class_<raylab::node::TextureInstance>("raylab::TextureInstance")
    .constructor<>()
    .property("scale", &raylab::node::TextureInstance::scale)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Scale"))
    )
    .property("rotate", &raylab::node::TextureInstance::rotate)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Rotate"))
    )
    .property("translate", &raylab::node::TextureInstance::translate)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Translate"))
    )
;

rttr::registration::class_<raylab::node::Wood>("raylab::Wood")
    .constructor<>()
    .property("light", &raylab::node::Wood::light)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Light"))
    )
    .property("dark", &raylab::node::Wood::dark)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Dark"))
    )
;

rttr::registration::class_<raylab::node::TurbulenceTexture>("raylab::TurbulenceTexture")
    .constructor<>()
    .property("color", &raylab::node::TurbulenceTexture::color)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color"))
    )
    .property("min_val", &raylab::node::TurbulenceTexture::min_val)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Min Value"))
    )
    .property("max_val", &raylab::node::TurbulenceTexture::max_val)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Max Value"))
    )
;

rttr::registration::class_<raylab::node::WrappedTwoColors>("raylab::WrappedTwoColors")
    .constructor<>()
    .property("min_val", &raylab::node::WrappedTwoColors::min_val)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Min Value"))
    )
    .property("max_val", &raylab::node::WrappedTwoColors::max_val)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Max Value"))
    )
    .property("exp", &raylab::node::WrappedTwoColors::exp)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Exp"))
    )
    .property("color1", &raylab::node::WrappedTwoColors::color1)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color1"))
    )
    .property("color2", &raylab::node::WrappedTwoColors::color2)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color2"))
    )
;

rttr::registration::class_<raylab::node::WrappedThreeColors>("raylab::WrappedThreeColors")
    .constructor<>()
    .property("min_val", &raylab::node::WrappedThreeColors::min_val)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Min Value"))
    )
    .property("max_val", &raylab::node::WrappedThreeColors::max_val)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Max Value"))
    )
    .property("exp", &raylab::node::WrappedThreeColors::exp)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Exp"))
    )
    .property("color1", &raylab::node::WrappedThreeColors::color1)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color1"))
    )
    .property("color2", &raylab::node::WrappedThreeColors::color2)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color2"))
    )
    .property("color3", &raylab::node::WrappedThreeColors::color3)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color3"))
    )
;

rttr::registration::class_<raylab::node::NestedNoisesTexture>("raylab::NestedNoisesTexture")
    .constructor<>()
    .property("min_val", &raylab::node::NestedNoisesTexture::min_val)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Min Value"))
    )
    .property("max_val", &raylab::node::NestedNoisesTexture::max_val)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Max Value"))
    )
    .property("exp", &raylab::node::NestedNoisesTexture::exp)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Exp"))
    )
    .property("wrap_factor", &raylab::node::NestedNoisesTexture::wrap_factor)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Wrap Factor"))
    )
    .property("color", &raylab::node::NestedNoisesTexture::color)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color"))
    )
;

rttr::registration::class_<raylab::node::WrappedRamp>("raylab::WrappedRamp")
    .constructor<>()
    .property("min_val", &raylab::node::WrappedRamp::min_val)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Min Value"))
    )
    .property("max_val", &raylab::node::WrappedRamp::max_val)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Max Value"))
    )
    .property("exp", &raylab::node::WrappedRamp::exp)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Exp"))
    )
    .property("color", &raylab::node::WrappedRamp::color)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Color"))
    )
    .property("perturbation", &raylab::node::WrappedRamp::perturbation)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Perturbation"))
    )
    .property("wrap_number", &raylab::node::WrappedRamp::wrap_number)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Wrap Number"))
    )
    .property("filepath", &raylab::node::WrappedRamp::filepath)
    (
        rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Filepath")),
        rttr::metadata(js::RTTR::FilePathTag(), true),
        rttr::metadata(ee0::PropOpenFileTag(), ee0::PropOpenFile("*.ppm"))
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

rttr::registration::class_<raylab::node::Sampler>("raylab::Sampler")
    .constructor<const std::string&>()
    .property("num_samples", &raylab::node::Sampler::num_samples)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("NumSamples"))
    )
;

rttr::registration::class_<raylab::node::Jittered>("raylab::Jittered")
    .constructor<>()
;

rttr::registration::class_<raylab::node::MultiJittered>("raylab::MultiJittered")
    .constructor<>()
;

rttr::registration::class_<raylab::node::Regular>("raylab::Regular")
    .constructor<>()
;

rttr::registration::class_<raylab::node::PureRandom>("raylab::PureRandom")
    .constructor<>()
;

// mapping

rttr::registration::class_<raylab::node::SphericalMap>("raylab::SphericalMap")
    .constructor<>()
;

rttr::registration::enumeration<raylab::node::LightProbe::MapType>("rlab_lightprobe_map_type")
(
	rttr::value("regular",   raylab::node::LightProbe::MapType::Regular),
    rttr::value("panoramic", raylab::node::LightProbe::MapType::Panoramic)
);

rttr::registration::class_<raylab::node::LightProbe>("raylab::LightProbe")
    .constructor<>()
    .property("mapping_type", &raylab::node::LightProbe::mapping_type)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Mapping Type"))
    )
;

rttr::registration::class_<raylab::node::SquareMap>("raylab::SquareMap")
    .constructor<>()
;

rttr::registration::class_<raylab::node::CylindricalMap>("raylab::CylindricalMap")
    .constructor<>()
;

// noise

rttr::registration::class_<raylab::node::LatticeNoise>("raylab::LatticeNoise")
    .constructor<const std::string&>()
    .property("octaves", &raylab::node::LatticeNoise::octaves)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Octaves"))
    )
    .property("lacunarity", &raylab::node::LatticeNoise::lacunarity)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Lacunarity"))
    )
    .property("gain", &raylab::node::LatticeNoise::gain)
    (
	    rttr::metadata(ee0::UIMetaInfoTag(), ee0::UIMetaInfo("Gain"))
    )
;

rttr::registration::class_<raylab::node::LinearNoise>("raylab::LinearNoise")
    .constructor<>()
;

rttr::registration::class_<raylab::node::CubicNoise>("raylab::CubicNoise")
    .constructor<>()
;

}

namespace raylab
{

void regist_rttr()
{
}

}
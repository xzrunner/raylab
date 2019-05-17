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

rttr::registration::class_<raylab::node::ImageTexture>("raylab::ImageTexture")
    .constructor<>()
    .property("filepath", &raylab::node::ImageTexture::filepath)
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

rttr::registration::class_<raylab::node::LightProbe>("raylab::LightProbe")
    .constructor<>()
;

}

namespace raylab
{

void regist_rttr()
{
}

}
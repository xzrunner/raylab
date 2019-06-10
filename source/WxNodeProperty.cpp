#include "raylab/WxNodeProperty.h"
#include "raylab/camera_nodes.h"
#include "raylab/primitive_nodes.h"
#include "raylab/mapping_nodes.h"

#include <ee0/SubjectMgr.h>
#include <ee0/ReflectPropTypes.h>
#include <ee0/MessageID.h>
#include <ee0/WxPropHelper.h>
#include <blueprint/Node.h>
#include <blueprint/MessageID.h>

#include <cpputil/StringHelper.h>
#include <node0/SceneNode.h>
#include <node2/CompBoundingBox.h>

#include <wx/sizer.h>
#include <wx/propgrid/propgrid.h>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

namespace raylab
{

WxNodeProperty::WxNodeProperty(wxWindow* parent, const ee0::SubjectMgrPtr& sub_mgr)
	: bp::WxNodeProperty(parent, sub_mgr)
{
}

bool WxNodeProperty::InitView(const rttr::property& prop, const bp::NodePtr& node)
{
    bool ret = true;

    auto ui_info_obj = prop.get_metadata(ee0::UIMetaInfoTag());
    if (!ui_info_obj.is_valid()) {
        return false;
    }

    auto ui_info = ui_info_obj.get_value<ee0::UIMetaInfo>();
    auto prop_type = prop.get_type();

    if (prop_type == rttr::type::get<node::Stereo::ViewingType>())
    {
        const wxChar* TYPES[] = { wxT("Parallel"), wxT("Transverse"), NULL };
        auto type_prop = new wxEnumProperty(ui_info.desc, wxPG_LABEL, TYPES);
        auto type = prop.get_value(node).get_value<node::Stereo::ViewingType>();
        type_prop->SetValue(static_cast<int>(type));
        m_pg->Append(type_prop);
    }
    else if (prop_type == rttr::type::get<node::Grid::TriangleType>())
    {
        const wxChar* TYPES[] = { wxT("Flat"), wxT("Smooth"), NULL };
        auto type_prop = new wxEnumProperty(ui_info.desc, wxPG_LABEL, TYPES);
        auto type = prop.get_value(node).get_value<node::Grid::TriangleType>();
        type_prop->SetValue(static_cast<int>(type));
        m_pg->Append(type_prop);
    }
    else if (prop_type == rttr::type::get<node::LightProbe::MapType>())
    {
        const wxChar* TYPES[] = { wxT("Regular"), wxT("Panoramic"), NULL };
        auto type_prop = new wxEnumProperty(ui_info.desc, wxPG_LABEL, TYPES);
        auto type = prop.get_value(node).get_value<node::LightProbe::MapType>();
        type_prop->SetValue(static_cast<int>(type));
        m_pg->Append(type_prop);
    }
    else
    {
        ret = false;
    }

    return ret;
}

bool WxNodeProperty::UpdateView(const rttr::property& prop, const wxPGProperty& wx_prop)
{
    bool ret = true;

    auto ui_info_obj = prop.get_metadata(ee0::UIMetaInfoTag());
    if (!ui_info_obj.is_valid()) {
        return false;
    }

    auto key = wx_prop.GetName();
    wxAny val = wx_prop.GetValue();

    auto ui_info = ui_info_obj.get_value<ee0::UIMetaInfo>();
    auto prop_type = prop.get_type();

    if (prop_type == rttr::type::get<node::Stereo::ViewingType>() && key == ui_info.desc)
    {
        prop.set_value(m_node, node::Stereo::ViewingType(wxANY_AS(val, int)));
    }
    else if (prop_type == rttr::type::get<node::Grid::TriangleType>() && key == ui_info.desc)
    {
        prop.set_value(m_node, node::Grid::TriangleType(wxANY_AS(val, int)));
    }
    else if (prop_type == rttr::type::get<node::LightProbe::MapType>() && key == ui_info.desc)
    {
        prop.set_value(m_node, node::LightProbe::MapType(wxANY_AS(val, int)));
    }
    else
    {
        ret = false;
    }

    return ret;
}

}
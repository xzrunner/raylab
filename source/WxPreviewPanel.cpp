#include "raylab/WxPreviewPanel.h"
#include "raylab/bg.xpm"
#include "raylab/Evaluator.h"
#include "raylab/world_nodes.h"

#include <ee0/WxStagePage.h>
#include <blueprint/CompNode.h>
#include <blueprint/Node.h>
#include <blueprint/Connecting.h>
#include <raytracing/world/World.h>
#include <raytracing/cameras/Camera.h>

#include <node0/SceneNode.h>

#include <wx/bitmap.h>
#include <wx/dcbuffer.h>

namespace raylab
{

BEGIN_EVENT_TABLE(WxPreviewPanel, wxPanel)
	EVT_MOUSE_EVENTS(WxPreviewPanel::OnMouse)
    EVT_COMMAND(ID_RENDER_NEWPIXEL, wxEVT_RENDER, WxPreviewPanel::OnNewPixel)
    EVT_COMMAND(ID_RENDER_COMPLETED, wxEVT_RENDER, WxPreviewPanel::OnRenderCompleted)
    EVT_TIMER(ID_RENDER_UPDATE, WxPreviewPanel::OnTimerUpdate)
END_EVENT_TABLE()

WxPreviewPanel::WxPreviewPanel(wxWindow* parent, const wxSize& size, ee0::WxStagePage* stage)
    : wxPanel(parent, wxID_ANY, wxDefaultPosition, size)
    , m_stage(stage)
    //, m_image(NULL)
    //, m_world(NULL)
    //, thread(NULL)
    //, timer(NULL)
    //, updateTimer(this, ID_RENDER_UPDATE)
{
}

void WxPreviewPanel::OnDraw(wxDC& dc)
{
    if (m_image != NULL && m_image->IsOk()) {
        wxBufferedDC bdc(&dc, *m_image);
    }
}

void WxPreviewPanel::renderStart()
{
//	m_frame->SetStatusText( wxT( "Building world..." ) );

    BuildScene();

    if (!m_world) {
        return;
    }

//	m_frame->SetStatusText( wxT( "Rendering..." ) );

	int w = m_world->GetViewPlane().GetWidth(),
		h = m_world->GetViewPlane().GetHeight();

	pixelsRendered = 0;
	pixelsToRender = w * h;

	//set the background
	wxBitmap bitmap(w, h, -1);
	wxMemoryDC dc;
	dc.SelectObject(bitmap);
	dc.SetBackground(*wxGREY_BRUSH);
	dc.Clear();

	wxBitmap tile(bg_xpm);

	for(int x = 0; x < w; x += 16)
	{
		for(int y = 0; y < h; y += 16)
			dc.DrawBitmap(tile, x, y, FALSE);
	}

	dc.SelectObject(wxNullBitmap);

	wxImage temp = bitmap.ConvertToImage();
	SetImage(temp);

	updateTimer.Start(250);

	//start timer
    m_timer = new wxStopWatch();

//    m_thread = std::make_unique<RenderThread>(this, m_world);
    m_thread = new RenderThread(this, m_world);
    m_thread->Create();
	m_world->SetRenderOutput(m_thread/*.get()*/);
    m_thread->SetPriority(20);
    m_thread->Run();
}

void WxPreviewPanel::renderPause()
{

}

void WxPreviewPanel::renderResume()
{

}

void WxPreviewPanel::OnRenderCompleted( wxCommandEvent& event )
{
//	m_thread.reset();
    //if (m_thread) {
    //    delete m_thread;
    //}
    m_thread = nullptr;

//    m_world.reset();

	if(m_timer != NULL)
	{
		long interval = m_timer->Time();

		wxTimeSpan timeElapsed(0, 0, 0, interval);
		wxString timeString = timeElapsed.Format(wxT("Elapsed Time: %H:%M:%S"));
//		m_frame->SetStatusText( timeString, 1);

		delete m_timer;
        m_timer = NULL;
	}
}

void WxPreviewPanel::OnTimerUpdate( wxTimerEvent& event )
{
	if(m_timer == NULL)
		return;

	//percent
	float completed = (float)pixelsRendered / (float)pixelsToRender;

	wxString progressString = wxString::Format(wxT("Rendering...%d%%"),
		(int)(completed*100));
//	m_frame->SetStatusText( progressString , 0);

	//time elapsed
	long interval = m_timer->Time();

	wxTimeSpan timeElapsed(0, 0, 0, interval);

	//time remaining
	float remaining = 1.0f - completed;
	long msecRemain = (long)
		(((double)interval / (completed*100)) * 100 * remaining);

	wxTimeSpan timeRemaining(0, 0, 0, msecRemain);

	wxString timeRemainString = timeRemaining.Format(wxT(" / ETA: %H:%M:%S"));
	wxString timeString = timeElapsed.Format(wxT("Elapsed Time: %H:%M:%S"));

	////only display ETA if something has been completed
	//if(msecRemain >= 0)
	//	m_frame->SetStatusText( timeString + timeRemainString, 1);
	//else
	//	m_frame->SetStatusText( timeString, 1);
}

void WxPreviewPanel::OnNewPixel( wxCommandEvent& event )
{
	//set up double buffered device context
	wxClientDC cdc(this);
//	DoPrepareDC(cdc);
	wxBufferedDC bufferedDC(&cdc, *m_image);

	//iterate over all pixels in the event
	std::vector<Pixel*> *pixelsUpdate =
		(std::vector<Pixel*> *)event.GetClientData();

	for(std::vector<Pixel*>::iterator itr = pixelsUpdate->begin();
		itr != pixelsUpdate->end(); itr++)
	{
		Pixel* pixel = *itr;

		wxPen pen(wxColour(pixel->red, pixel->green, pixel->blue));
		bufferedDC.SetPen(pen);
		bufferedDC.DrawPoint(pixel->x, pixel->y);

		pixelsRendered++;
		delete pixel;
	}

	pixelsUpdate->clear();
	delete pixelsUpdate;
}

void WxPreviewPanel::SetImage(wxImage& image)
{
	m_image = std::make_unique<wxBitmap>(image);

	//SetScrollbars(10, 10, (int)(m_image->GetWidth()  / 10.0f),
	//	(int)(m_image->GetHeight() / 10.0f), 0, 0, true);

	Refresh();
}

wxImage WxPreviewPanel::GetImage()
{
    if (m_image != nullptr) {
        return m_image->ConvertToImage();
    }
	return wxImage();
}

void WxPreviewPanel::OnMouse(wxMouseEvent& event)
{
    if (event.LeftDClick()) {
        renderStart();
    }
}

void WxPreviewPanel::BuildScene()
{
    const node::World* src_world = nullptr;
    m_stage->Traverse([&](const ee0::GameObj& obj)->bool
    {
        if (obj->HasUniqueComp<bp::CompNode>())
        {
            auto& cnode = obj->GetUniqueComp<bp::CompNode>();
            auto bp_node = cnode.GetNode();
            if (bp_node->get_type() == rttr::type::get<node::Output>())
            {
                auto& conns = bp_node->GetAllInput()[0]->GetConnecting();
                if (!conns.empty())
                {
                    auto& prev_node = conns[0]->GetFrom()->GetParent();
                    if (prev_node.get_type() == rttr::type::get<node::World>()) {
                        src_world = &static_cast<const node::World&>(prev_node);
                        return false;
                    }
                }
            }
        }
        return true;
    });

    if (src_world) {
        m_world = std::make_shared<rt::World>();
        Evaluator::Build(*m_world, *src_world);
    }
}

//////////////////////////////////////////////////////////////////////////
// class WxPreviewPanel::RenderThread
//////////////////////////////////////////////////////////////////////////

DEFINE_EVENT_TYPE(wxEVT_RENDER)

WxPreviewPanel::RenderThread::~RenderThread()
{
}

void WxPreviewPanel::RenderThread::
SetPixel(int x, int y, int red, int green, int blue)
{
   m_pixels.push_back(new Pixel(x, y, red, green, blue));

   if (m_timer->Time() - m_lastUpdateTime > 40) {
       NotifyCanvas();
   }

   TestDestroy();
}

void WxPreviewPanel::RenderThread::
NotifyCanvas()
{
   m_lastUpdateTime = m_timer->Time();

   //copy rendered pixels into a new vector and reset
   std::vector<Pixel*> *pixelsUpdate = new std::vector<Pixel*>(m_pixels);
   m_pixels.clear();

   wxCommandEvent event(wxEVT_RENDER, ID_RENDER_NEWPIXEL);
   event.SetClientData(pixelsUpdate);
   m_preview->GetEventHandler()->AddPendingEvent(event);
}

void WxPreviewPanel::RenderThread::
OnExit()
{
   NotifyCanvas();

   wxCommandEvent event(wxEVT_RENDER, ID_RENDER_COMPLETED);

   m_preview->GetEventHandler()->AddPendingEvent(event);

   m_preview->GetParent()->GetEventHandler()->AddPendingEvent(event);
}

void *WxPreviewPanel::RenderThread::
Entry()
{
   m_lastUpdateTime = 0;
   m_timer = new wxStopWatch();

   //world->render_scene(); //for bare bones ray tracer only

   auto& cam = m_world->GetCamera();
   if (cam) {
       cam->RenderScene(*m_world);
   }

   return nullptr;
}

}
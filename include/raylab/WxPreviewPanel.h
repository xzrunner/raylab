#pragma once

#include <raytracing/world/RenderOutput.h>

//#include <wx/scrolwin.h>
#include <wx/panel.h>
#include <wx/thread.h>
#include <wx/event.h>
#include <wx/stopwatch.h>
#include <wx/timer.h>

namespace ee0 { class WxStagePage; }
namespace rt { class World; }

namespace raylab
{

DECLARE_EVENT_TYPE(wxEVT_RENDER, -1)
#define ID_RENDER_COMPLETED 100
#define ID_RENDER_NEWPIXEL  101
#define ID_RENDER_UPDATE    102

class WxPreviewPanel : public wxPanel
{
public:
    WxPreviewPanel(wxWindow* parent, const wxSize& size,
        ee0::WxStagePage* stage);

    virtual void OnDraw(wxDC& dc);

    void renderStart();
    void renderPause();
    void renderResume();

    void OnRenderCompleted(wxCommandEvent& event);
    void OnTimerUpdate(wxTimerEvent& event);
    void OnNewPixel(wxCommandEvent& event);

    void SetImage(wxImage& image);
    wxImage GetImage();

private:
    void OnMouse(wxMouseEvent& event);

    void BuildScene();

private:
    class Pixel
    {
    public:
	    Pixel(int x, int y, int red, int green, int blue)
            : x(x), y(y), red(red), green(green), blue(blue)
        {}

    public:
	    int x, y;
	    int red, green, blue;
    };

    class RenderThread : public wxThread, public rt::RenderOutput
    {
    public:
        RenderThread(WxPreviewPanel* preview, const std::shared_ptr<rt::World>& world)
            : wxThread()
            , m_preview(preview)
            , m_world(world)
        {
        }
        virtual ~RenderThread();

        virtual void *Entry();
        virtual void OnExit();

        virtual void SetPixel(int x, int y, int red, int green, int blue) override;

    private:
        void NotifyCanvas();

    private:
        WxPreviewPanel* m_preview = nullptr;

        std::shared_ptr<rt::World> m_world = nullptr;

        std::vector<Pixel*> m_pixels;
        wxStopWatch*        m_timer = nullptr;
        long                m_lastUpdateTime = 0;

    }; // RenderThread

private:
    ee0::WxStagePage* m_stage;

    std::unique_ptr<wxBitmap> m_image = nullptr;

    std::shared_ptr<rt::World> m_world = nullptr;

    wxStopWatch* m_timer = nullptr;
    wxTimer updateTimer;

    long pixelsRendered;
    long pixelsToRender;

    std::shared_ptr<RenderThread> m_thread = nullptr;

    DECLARE_EVENT_TABLE()

}; // WxPreviewPanel

}
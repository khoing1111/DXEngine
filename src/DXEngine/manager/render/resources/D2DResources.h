#pragma once


/////////////
// INCLUDE

// Direct 2D library
#include <d2d1_2.h>
#include <map>

#include <hkUtilLib\dataManage\LinkedList.h>
#include <hkUtilLib\core\Color.h>


//////////////////
// LINK LIBRARY
#pragma comment(lib, "d2d1.lib")

namespace RenderManager
{
    ///////////////////////////////
    // CLASS: D2DDeviceResources
    struct D2DDeviceResources
    {
    public:
        D2DDeviceResources();
        D2DDeviceResources(HWND);
        D2DDeviceResources(const D2DDeviceResources&);
        ~D2DDeviceResources();

        bool Initialize(HWND);
        void Release();

        WPtr<ID2D1SolidColorBrush> GetBrush(hkRGBAColor color = hkRGBAColor::BLUE);
        WPtr<ID2D1RenderTarget> GetRenderTarget();

    private:
        void CreateDefaultBrush();
        void CreateBrush(const hkRGBAColor& color);
        bool HaveBrush(const hkRGBAColor& color);
        void ReleaseAllBrush();

    private:
        SPtr<ID2D1Factory> m_direct2dFactory;
        SPtr<ID2D1RenderTarget> m_renderTarget;
        std::map<uint, SPtr<ID2D1SolidColorBrush>> m_colorBrush;
        HWND m_hwnd;

        bool m_isValid;
    };
}
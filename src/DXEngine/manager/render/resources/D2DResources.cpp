/////////////
// INCLUDE
#include "D2DResources.h"


//=============================================================================
#pragma region // 2D device resource
//=============================================================================

RenderManager::D2DDeviceResources::D2DDeviceResources()
{
    m_isValid = false;
    m_hwnd = nullptr;
}

RenderManager::D2DDeviceResources::D2DDeviceResources(HWND m_hwnd)
{
    m_isValid = Initialize(m_hwnd);
}

RenderManager::D2DDeviceResources::D2DDeviceResources(const RenderManager::D2DDeviceResources& clone)
{
    m_direct2dFactory = clone.m_direct2dFactory;
    m_renderTarget = clone.m_renderTarget;
    m_hwnd = clone.m_hwnd;

    m_colorBrush = clone.m_colorBrush;
    m_isValid = clone.m_isValid;
}

RenderManager::D2DDeviceResources::~D2DDeviceResources()
{
    Release();
}

bool RenderManager::D2DDeviceResources::Initialize(HWND inHwnd)
{
    HRESULT hr = S_OK;

    // Get application handler
    if (inHwnd == nullptr)
        return false;

    m_hwnd = inHwnd;

    // Create a Direct2D factory.
    ID2D1Factory* newFactory;
    hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &newFactory);
    if (!SUCCEEDED(hr))
        return false;

    m_direct2dFactory.Set(newFactory);

    // Get window size
    RECT rc;
    GetClientRect(m_hwnd, &rc);

    D2D1_SIZE_U size = D2D1::SizeU(
        rc.right - rc.left,
        rc.bottom - rc.top
        );

    // Get desk topdpi
    static D2D_POINT_2F dpi = { 96, 96 }; // The default DPI
    m_direct2dFactory->GetDesktopDpi(&dpi.x, &dpi.y);

    // Set the DPI to be the default system DPI to allow direct mapping
    // between image pixels and desktop pixels in different system DPI settings
    D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties =
        D2D1::RenderTargetProperties();

    renderTargetProperties.dpiX = dpi.x;
    renderTargetProperties.dpiY = dpi.y;

    // Create a Direct2D render target.
    ID2D1HwndRenderTarget* newRenderTarget;
    hr = m_direct2dFactory->CreateHwndRenderTarget(
        renderTargetProperties,
        D2D1::HwndRenderTargetProperties(m_hwnd, size),
        &newRenderTarget);
    if (!SUCCEEDED(hr))
        return false;

    m_renderTarget.Set(newRenderTarget);
    m_renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

    // Create all default brush
    CreateDefaultBrush();

    m_isValid = true;
    return true;
}

void RenderManager::D2DDeviceResources::CreateDefaultBrush()
{
    if (!m_isValid)
        return;

    CreateBrush(hkRGBAColor::WHITE);
    CreateBrush(hkRGBAColor::BLACK);
    CreateBrush(hkRGBAColor::BLUE);
    CreateBrush(hkRGBAColor::RED);
    CreateBrush(hkRGBAColor::GREEN);
    CreateBrush(hkRGBAColor::YELLOW);
}

bool RenderManager::D2DDeviceResources::HaveBrush(const hkRGBAColor& color)
{
    if (!m_isValid)
        return false;

    std::map<uint, SPtr<ID2D1SolidColorBrush>>::const_iterator it = m_colorBrush.find(color.ToNumber());
    return it != m_colorBrush.end();
}

void RenderManager::D2DDeviceResources::CreateBrush(const hkRGBAColor& color)
{
    if (!m_isValid)
        return;

    if (!HaveBrush(color))
    {
        // Convert hkColor to D2D Color
        D2D1::ColorF d2dColor(color.r, color.g, color.b, color.a);

        // Create new color brush pointer
        ID2D1SolidColorBrush* newBrush;
        m_renderTarget->CreateSolidColorBrush(d2dColor, &newBrush);

        // Set new brush into map list
        m_colorBrush[color.ToNumber()].Set(newBrush);
    }
}

WPtr<ID2D1SolidColorBrush> RenderManager::D2DDeviceResources::GetBrush(const hkRGBAColor color)
{
    if (!m_isValid)
        return WPtr<ID2D1SolidColorBrush>();

    if (!HaveBrush(color))
        CreateBrush(color);
    
    return m_colorBrush[color.ToNumber()];
}

void RenderManager::D2DDeviceResources::ReleaseAllBrush()
{
    // Release all Brush
    for (std::map<uint, SPtr<ID2D1SolidColorBrush>>::iterator it = m_colorBrush.begin(); it != m_colorBrush.end(); ++it)
    {
        SPtr<ID2D1SolidColorBrush> brush = it->second;
        if (brush != nullptr)
            brush->Release();
    }

    // Clear map content
    m_colorBrush.clear();
}

void RenderManager::D2DDeviceResources::Release()
{
    if (m_renderTarget != nullptr)
    {
        m_renderTarget->Release();
        m_renderTarget.Release();
    }

    if (m_direct2dFactory != nullptr)
    {
        m_direct2dFactory->Release();
        m_direct2dFactory.Release();
    }

    // Release all default and created brush
    ReleaseAllBrush();

    m_hwnd = nullptr;
}

WPtr<ID2D1RenderTarget> RenderManager::D2DDeviceResources::GetRenderTarget()
{
    if (!m_isValid)
        return nullptr;

    return m_renderTarget;
}

//=============================================================================
#pragma endregion // 2D device resource
//=============================================================================
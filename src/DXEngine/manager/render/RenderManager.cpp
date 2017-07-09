#include "RenderManager.h"


//=============================================================================
#pragma region // D2D Manager
//=============================================================================

RenderManager::D2DManager::D2DManager()
{
    m_isTransformMatrixUpdate = false;
}

RenderManager::D2DManager::D2DManager(const D2DManager& clone)
{
    m_deviceResource = clone.m_deviceResource;
    m_brush = clone.m_brush;
    m_matrixList = clone.m_matrixList;
    m_transformMatrix = clone.m_transformMatrix;
    m_drawColor = clone.m_drawColor;
    m_bitmapManager = clone.m_bitmapManager;
    m_isTransformMatrixUpdate = clone.m_isTransformMatrixUpdate;
}

RenderManager::D2DManager::D2DManager(HWND hwnd)
{
    Initialize(hwnd);
}

RenderManager::D2DManager::~D2DManager()
{
    Release();
}

bool RenderManager::D2DManager::Initialize(HWND hwnd)
{
    m_drawColor = hkRGBAColor::BLUE;

    // Set up device resource
    if (!m_deviceResource.Initialize(hwnd))
        return false;

    m_renderTarget = m_deviceResource.GetRenderTarget();
    
    // Get defaut brush
    m_brush = m_deviceResource.GetBrush(m_drawColor);

    // Create bitmap manager
    if (!m_bitmapManager.Initialize(m_renderTarget))
        return false;

    // Initialize matrix list with one matrix
    IntitializeMatrixList(1);
    m_isTransformMatrixUpdate = false;

    return true;
}

void RenderManager::D2DManager::IntitializeMatrixList(uint numMatrix)
{
    m_matrixList.Set(new hkMatrix3x3[numMatrix](), numMatrix);
}

void RenderManager::D2DManager::Release()
{
}

bool RenderManager::D2DManager::Begin(hkRGBAColor c)
{
    if (m_renderTarget == nullptr)
        return false;

    m_renderTarget->BeginDraw();

    D2D_COLOR_F color;
    color.r = c.r;
    color.g = c.g;
    color.b = c.b;
    color.a = c.a;

    // Clear background
    m_renderTarget->Clear(color);

    // If transform matrix has changed
    if (m_isTransformMatrixUpdate)
    {
        // Translate to D2D1 matrix
        D2D1_MATRIX_3X2_F d2dMatrix;
        d2dMatrix._11 = m_transformMatrix[0][0];
        d2dMatrix._12 = m_transformMatrix[0][1];
        d2dMatrix._21 = m_transformMatrix[1][0];
        d2dMatrix._22 = m_transformMatrix[1][1];
        d2dMatrix._31 = m_transformMatrix[2][0];
        d2dMatrix._32 = m_transformMatrix[2][1];

        m_renderTarget->SetTransform(d2dMatrix);

        m_isTransformMatrixUpdate = false;
    }

    return true;
}

bool RenderManager::D2DManager::End()
{
    if (m_renderTarget == nullptr)
        return false;

    HRESULT hr;

    hr = m_renderTarget->EndDraw();
    if (!SUCCEEDED(hr))
        return false;

    return true;
}

void RenderManager::D2DManager::DrawRect(hkRect2F& rect, float w)
{
    D2D1_RECT_F d2dRect;
    memcpy(&d2dRect, &rect, sizeof(D2D1_RECT_F));

    m_renderTarget->DrawRectangle(d2dRect, &m_brush.Get(), w);
}

void RenderManager::D2DManager::FillRect(hkRect2F& rect)
{
    D2D1_RECT_F d2dRect;
    memcpy(&d2dRect, &rect, sizeof(D2D1_RECT_F));

    m_renderTarget->FillRectangle(d2dRect, &m_brush.Get());
}

void RenderManager::D2DManager::DrawPoint(hkVector2F& position, int width)
{
    D2D1_ELLIPSE ellipse;
    ellipse.point.x = position.x;
    ellipse.point.y = position.y;
    ellipse.radiusX = width;
    ellipse.radiusY = width;

    m_renderTarget->FillEllipse(ellipse, &m_brush.Get());
}

void RenderManager::D2DManager::DrawBitmap(WPtr<ID2D1Bitmap> bitmap,
    const hkRect2F destRect,
    const hkRect2F srcRect,
    float opacity,
    D2D1_BITMAP_INTERPOLATION_MODE mode)
{
    if (bitmap == nullptr)
        return;

    D2D1_RECT_F d2dDestRect;
    memcpy(&d2dDestRect, &destRect, sizeof(D2D1_RECT_F));

    D2D1_RECT_F d2dSrcRect;
    memcpy(&d2dSrcRect, &srcRect, sizeof(D2D1_RECT_F));

    m_renderTarget->DrawBitmap(&bitmap.Get(), d2dDestRect,
        opacity, mode, d2dSrcRect);
}

void RenderManager::D2DManager::DrawBitmap(WPtr<ID2D1Bitmap> bitmap,
    const hkRect2F srcRect,
    float opacity,
    D2D1_BITMAP_INTERPOLATION_MODE mode)
{
    if (bitmap == nullptr)
        return;

    D2D1_RECT_F d2dSrcRect;
    memcpy(&d2dSrcRect, &srcRect, sizeof(D2D1_RECT_F));

    m_renderTarget->DrawBitmap(&bitmap.Get(), nullptr,
        opacity, mode, &d2dSrcRect);
}

void RenderManager::D2DManager::DrawFullBitmap(WPtr<ID2D1Bitmap> bitmap,
    const hkRect2F destRect,
    float opacity,
    D2D1_BITMAP_INTERPOLATION_MODE mode)
{
    if (bitmap == nullptr)
        return;

    D2D1_RECT_F d2dDestRect;
    memcpy(&d2dDestRect, &destRect, sizeof(D2D1_RECT_F));

    m_renderTarget->DrawBitmap(&bitmap.Get(), d2dDestRect,
        opacity, mode);
}

void RenderManager::D2DManager::DrawBitmap(WPtr<ID2D1Bitmap> bitmap,
    float opacity,
    D2D1_BITMAP_INTERPOLATION_MODE mode)
{
    if (bitmap == nullptr)
        return;

    m_renderTarget->DrawBitmap(&bitmap.Get(), nullptr, opacity, mode);
}

void RenderManager::D2DManager::DrawLine(hkVector2F& begin, hkVector2F& end, float strokeWidth)
{
    D2D_POINT_2F d2dBeginPoint;
    D2D_POINT_2F d2dEndPoint;

    memcpy(&d2dBeginPoint, &begin, sizeof(D2D_POINT_2F));
    memcpy(&d2dEndPoint, &end, sizeof(D2D_POINT_2F));

    m_renderTarget->DrawLine(d2dBeginPoint, d2dEndPoint, 
                             &m_brush.Get(), strokeWidth);
}

void RenderManager::D2DManager::SetDrawColor(hkRGBAColor color)
{
    if (m_drawColor != color)
    {
        m_drawColor = color;
        m_brush = m_deviceResource.GetBrush(m_drawColor);
    }
}

void RenderManager::D2DManager::ResetAllTransformMatrix() {
    uint numMatrix = m_matrixList.Size();
    for (uint index = 0; index < numMatrix; index++) 
    {
        m_matrixList[index].SetIdentity();
    }

    m_transformMatrix.SetIdentity();
    m_isTransformMatrixUpdate = true;
}

void RenderManager::D2DManager::SetTransformMatrix(uint index, hkMatrix3x3& matrix) {
    if (m_matrixList.Exist(index))
    {
        m_matrixList[index] = matrix;
        UpdateTransformMatrix();
    }
}

void RenderManager::D2DManager::ResetTransformMatrix(uint index) {
    if (m_matrixList.Exist(index))
    {
        m_matrixList[index].SetIdentity();
        UpdateTransformMatrix();
    }
}

hkMatrix3x3 RenderManager::D2DManager::GetTransformMatrix(uint index)
{
    if (m_matrixList.Exist(index))
        return m_matrixList[index];

    return hkMatrix3x3();
}

void RenderManager::D2DManager::UpdateTransformMatrix()
{
    m_transformMatrix.SetIdentity();
    m_isTransformMatrixUpdate = true;

    uint numMatrix = m_matrixList.Size();
    for (uint index = 0; index < numMatrix; index++)
    {
        m_transformMatrix *= m_matrixList[index];
    }
}

//=============================================================================
#pragma endregion // D2D Manager
//=============================================================================


//=============================================================================
#pragma region // D3D Manager
//=============================================================================

////////////
// STATIC
RenderManager::D3DManager* RenderManager::D3DManager::s_instance = nullptr;

bool RenderManager::D3DManager::CreateInstance(float screenWidth, float screenHeight, HWND hwnd)
{
    if (!s_instance)
    {
        s_instance = new D3DManager();
        if (!s_instance->Initialize(screenWidth, screenHeight, hwnd))
        {
            delete s_instance;
            s_instance = nullptr;

            return false;
        }
    }

    return true;
}

RenderManager::D3DManager* RenderManager::D3DManager::GetInstance()
{
    return s_instance;
}

void RenderManager::D3DManager::ReleasseInstance()
{
    if (s_instance)
    {
        delete s_instance;
    }

    s_instance = nullptr;
}



////////////
// MEMBER
RenderManager::D3DManager::D3DManager()
{
    m_deviceResource = nullptr;
}

RenderManager::D3DManager::D3DManager(const D3DManager&)
{

}

RenderManager::D3DManager::~D3DManager()
{
    Release();
}

bool RenderManager::D3DManager::Initialize(float screenWidth, float screenHeight, HWND hwnd)
{
    bool success;


    // Set up device resource
    success = RenderManager::D3DDeviceResources::CreateInstance(screenWidth, screenHeight, hwnd);
    if (!success)
        return false;

    m_deviceResource = RenderManager::D3DDeviceResources::GetInstance();

    return true;
}

void RenderManager::D3DManager::Release()
{
    if (m_deviceResource)
    {
        m_deviceResource->Release();
        delete m_deviceResource;
        m_deviceResource = nullptr;
    }
}

void RenderManager::D3DManager::Begin(hkRGBAColor clearColor)
{
    m_deviceResource->BeginRender(clearColor);
}

bool RenderManager::D3DManager::End()
{
    return m_deviceResource->EndRender();
}

//=============================================================================
#pragma endregion // D3D Manager
//=============================================================================
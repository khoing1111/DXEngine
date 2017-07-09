#pragma once


/////////////
// INCLUDE
#include <map>

#include <hkUtilLib\dataManage\LinkedList.h>
#include <hkUtilLib\core\Shape.h>
#include <hkUtilLib\core\Color.h>

#include "resources/D2DResources.h"
#include "resources/D3DResources.h"
#include "image/BitmapManager.h"


namespace RenderManager
{
    ///////////////////////
    // CLASS: D2DManager
    class D2DManager
    {
        ///////////////////
        // METHOD MEMBER
    public:
        D2DManager();
        D2DManager(HWND);
        D2DManager(const D2DManager&);
        ~D2DManager();

        bool Initialize(HWND);
        void IntitializeMatrixList(uint numMatrix);
        void Release();

        bool Begin(hkRGBAColor = hkRGBAColor::WHITE);
        bool End();

        void SetDrawColor(hkRGBAColor = hkRGBAColor::BLACK);

        //-------------------------------------------------------------------------------
        // DRAW RECTANGLE
        void DrawRect(hkRect2F&, float = 1.0f);
        void FillRect(hkRect2F&);

        //-------------------------------------------------------------------------------
        // DRAW POINT
        void DrawPoint(hkVector2F& position, int width = 1.0f);
        
        //-------------------------------------------------------------------------------
        // DRAW LINE
        void DrawLine(hkVector2F& begin, hkVector2F& end, float strokeWidth = 1);

        //-------------------------------------------------------------------------------
        // DRAW BITMAP
        void DrawBitmap(WPtr<ID2D1Bitmap>, 
            const hkRect2F destRect,
            const hkRect2F srcRect, 
            float opacity = 1.0f,
            D2D1_BITMAP_INTERPOLATION_MODE = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

        void DrawBitmap(WPtr<ID2D1Bitmap>,
            const hkRect2F srcRect,
            float opacity = 1.0f,
            D2D1_BITMAP_INTERPOLATION_MODE = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

        void DrawFullBitmap(WPtr<ID2D1Bitmap>,
            const hkRect2F destRect,
            float opacity = 1.0f,
            D2D1_BITMAP_INTERPOLATION_MODE = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

        void DrawBitmap(WPtr<ID2D1Bitmap>,
            float opacity = 1.0f,
            D2D1_BITMAP_INTERPOLATION_MODE = D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

        void ResetAllTransformMatrix();
        void SetTransformMatrix(uint index, hkMatrix3x3&);
        void ResetTransformMatrix(uint index);
        hkMatrix3x3 GetTransformMatrix(uint index);

    private:
        void UpdateTransformMatrix();

        //////////////
        // VARIABLE
    private:
        D2DDeviceResources m_deviceResource;
        BitmapManager m_bitmapManager;
        hkRGBAColor m_drawColor;
        WPtr<ID2D1SolidColorBrush> m_brush;

        WPtr<ID2D1RenderTarget> m_renderTarget;

        // Transform matrix
        ArrPtr<hkMatrix3x3> m_matrixList;
        hkMatrix3x3 m_transformMatrix;
        bool m_isTransformMatrixUpdate;
    };


    ///////////////////////
    // CLASS: D3DManager
    class D3DManager
    {
        ///////////////////
        // STATIC MEMBER
    private:
        static D3DManager* s_instance;

    public:
        static bool CreateInstance(float, float, HWND);
        static D3DManager* GetInstance();
        static void ReleasseInstance();


        ///////////////////
        // METHOD MEMBER
    public:
        D3DManager();
        D3DManager(const D3DManager&);
        ~D3DManager();

        bool Initialize(float, float, HWND);
        void Release();

        void Begin(hkRGBAColor = hkRGBAColor::WHITE);
        bool End();

        //////////////
        // VARIABLE
    private:
        D3DDeviceResources* m_deviceResource;
    };
}
#pragma once

/////////////
// INCLUDE
#include <d2d1.h>
#include <wincodec.h>
#include <string>

#include <hkUtilLib\dataManage\LinkedList.h>


//////////////////////////
// CLASS: BitmapPackage
struct BitmapPackage
{
public:
    BitmapPackage(hkString inUrl, SPtr<ID2D1Bitmap> inBitmap);
    ~BitmapPackage();

    void Release();

    bool operator==(const BitmapPackage& other);

public:
    hkString url;
    SPtr<ID2D1Bitmap> bitmap;
};


/////////////////////////
// CLASS: BitmapManger
class BitmapManager
{
    ///////////////////
    // MEMBER METHOD
public:
    BitmapManager();
    BitmapManager(const BitmapManager&);
    BitmapManager(WPtr<ID2D1RenderTarget> renderTarget);
    ~BitmapManager();

    bool Initialize(WPtr<ID2D1RenderTarget> renderTarget);
    void Release();

    WPtr<ID2D1Bitmap> GetBitmap(hkString url);
    void PreLoadBitmaps(LinkedList<hkString> urlList);

private:
    WPtr<ID2D1Bitmap> LoadBitmapFromURL(hkString url);


    //////////////
    // VARIABLE
private:
    WPtr<ID2D1RenderTarget> m_pRenderTarget;
    SPtr<IWICImagingFactory> m_pWICFactory;
    LinkedList<BitmapPackage> m_bitmapPackages;

    bool m_isValid;
};
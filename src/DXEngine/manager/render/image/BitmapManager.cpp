#include "BitmapManager.h"

//=============================================================================
#pragma region // BitmapPackage
//=============================================================================

BitmapPackage::BitmapPackage(hkString inUrl, SPtr<ID2D1Bitmap> inBitmap)
{
    url = inUrl;
    bitmap = inBitmap;
}

BitmapPackage::~BitmapPackage()
{
    Release();
}

void BitmapPackage::Release()
{
    if (bitmap != nullptr)
    {
        bitmap->Release();
        bitmap.Release();
    }
}

bool BitmapPackage::operator==(const BitmapPackage& other)
{
    return url == other.url;
}

//=============================================================================
#pragma endregion // BitmapPackage
//=============================================================================



//=============================================================================
#pragma region // BitmapManager
//=============================================================================

BitmapManager::BitmapManager()
{
    m_isValid = false;
}

BitmapManager::BitmapManager(const BitmapManager& clone)
{
    m_bitmapPackages = clone.m_bitmapPackages;
    m_pRenderTarget = clone.m_pRenderTarget;
    m_pWICFactory = clone.m_pWICFactory;

    m_isValid = clone.m_isValid;
}

BitmapManager::BitmapManager(WPtr<ID2D1RenderTarget> renderTarget)
{
    m_isValid = Initialize(renderTarget);
}

BitmapManager::~BitmapManager()
{
    Release();
}

bool BitmapManager::Initialize(WPtr<ID2D1RenderTarget> renderTarget)
{
    HRESULT hr = S_OK;

    if (renderTarget == nullptr)
        return false;

    // Set render target reference
    m_pRenderTarget = renderTarget;

    // Create WIC Image Factory
    IWICImagingFactory* newFactory;
    hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&newFactory));
    if (!SUCCEEDED(hr))
        return false;

    m_pWICFactory.Set(newFactory);

    return true;
}

void BitmapManager::Release()
{
    m_pRenderTarget.Release();

    // Release WIC factory
    if (m_pWICFactory != nullptr)
    {
        m_pWICFactory.Get().Release();
        m_pWICFactory.Release();
    }
    
    // Clear all bitmap
    m_bitmapPackages.Clear();
}

WPtr<ID2D1Bitmap> BitmapManager::GetBitmap(hkString url)
{
    if (!m_isValid)
        return WPtr<ID2D1Bitmap>();

    // Search in existing bitmap list
    SPtr<ListIterator<BitmapPackage>> iterator = m_bitmapPackages.GetIterator();

    if (!iterator->IsValid())
        return LoadBitmapFromURL(url);

    do
    {
        // Check package url
        if (iterator->Get()->url == url)
            return iterator->Get()->bitmap;
    } while (iterator->Next());

    // If there are no bitmap with appropriate url, load a new one
    return LoadBitmapFromURL(url);
}

WPtr<ID2D1Bitmap> BitmapManager::LoadBitmapFromURL(hkString url)
{
    if (!m_isValid)
        return WPtr<ID2D1Bitmap>();

    // If there are no WIC factory
    if (m_pWICFactory == nullptr)
        return SPtr<ID2D1Bitmap>();

    IWICBitmapDecoder* decoder;
    IWICBitmapFrameDecode* bitmapSource;
    IWICFormatConverter* converter;
    HRESULT hr = S_OK;

    std::string stdUrl = url.GetStdString();
    std::wstring wUrl = std::wstring(stdUrl.begin(), stdUrl.end());

    // Create decoder for image from url
    hr = m_pWICFactory.Get().CreateDecoderFromFilename(
        wUrl.c_str(), // name of the file
        nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
    if (!SUCCEEDED(hr))
        return SPtr<ID2D1Bitmap>();

    // Get first frame
    hr = decoder->GetFrame(0, &bitmapSource);
    if (!SUCCEEDED(hr))
        return SPtr<ID2D1Bitmap>();

    // Create converter to convert image to desired format
    hr = m_pWICFactory.Get().CreateFormatConverter(&converter);
    converter->Initialize(
        bitmapSource, GUID_WICPixelFormat32bppPRGBA,
        WICBitmapDitherTypeNone, nullptr, 0.f,
        WICBitmapPaletteTypeMedianCut);
    if (!SUCCEEDED(hr))
        return SPtr<ID2D1Bitmap>();

    // Create a Direct2D bitmap from the WIC bitmap.
    ID2D1Bitmap* newBitmap;
    hr = m_pRenderTarget.Get().CreateBitmapFromWicBitmap(converter, nullptr, &newBitmap);

    if (!SUCCEEDED(hr))
        return SPtr<ID2D1Bitmap>();

    SPtr<ID2D1Bitmap> result(newBitmap);
    SPtr<BitmapPackage> newBitmapPackage = new BitmapPackage(url, newBitmap);
    m_bitmapPackages.Add(newBitmapPackage);

    // Free memory
    if (converter)
    {
        converter->Release();
        converter = nullptr;
    }

    if (decoder)
    {
        decoder->Release();
        decoder = nullptr;
    }

    if (bitmapSource)
    {
        bitmapSource->Release();
        bitmapSource = nullptr;
    }

    return result;
}

void BitmapManager::PreLoadBitmaps(LinkedList<hkString> urlList)
{
    if (!m_isValid)
        return;

    // If there are no WIC factory
    if (m_pWICFactory == nullptr)
        return;

    SPtr<ListIterator<hkString>> iterator = urlList.GetIterator();

    if (iterator->IsValid())
        do LoadBitmapFromURL(*iterator->Get());
        while (iterator->Next());
}

//=============================================================================
#pragma endregion // BitmapManager
//=============================================================================
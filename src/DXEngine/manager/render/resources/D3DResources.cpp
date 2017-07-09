/////////////
// INCLUDE
#include <fstream>

#include "D3DResources.h"
#include "../shader/ShaderPipeLineResource.h"


//=============================================================================
#pragma region // Device's resource
//=============================================================================

////////////
// STATIC
RenderManager::D3DDeviceResources* RenderManager::D3DDeviceResources::s_instance = nullptr;

bool RenderManager::D3DDeviceResources::CreateInstance(float screenWidth, float screenHeight, HWND hwnd)
{
	if (!s_instance)
	{
		// Create new instance;
		s_instance = new D3DDeviceResources();

		if (!s_instance->Initialize(screenWidth, screenHeight, hwnd))
		{
			// If fail release instance
			delete s_instance;
			s_instance = nullptr;

			return false;
		}
	}

	return true;
}

bool RenderManager::D3DDeviceResources::CreateInstance(
	const D3D_FEATURE_LEVEL* featureLevels,
	const uint numFeature,
	float screenWidth,
	float screenHeight,
	HWND hwnd)
{
	if (!s_instance)
	{
		// Create new instance;
		s_instance = new D3DDeviceResources();

		if (!s_instance->Initialize(featureLevels, numFeature, screenWidth, screenHeight, hwnd))
		{
			// If fail release instance
			delete s_instance;
			s_instance = nullptr;

			return false;
		}
	}

	return true;
}

RenderManager::D3DDeviceResources* RenderManager::D3DDeviceResources::GetInstance()
{
	return s_instance;
}

void RenderManager::D3DDeviceResources::ReleaseInstance()
{
	if (s_instance)
	{
		delete s_instance;
		s_instance = nullptr;
	}
}

void RenderManager::D3DDeviceResources::LoadShaderFile(std::string File, ArrPtr<char>& output)
{
	// open the file
	std::ifstream VertexFile(File, std::ios::in | std::ios::binary | std::ios::ate);

	// if open was successful
	if (VertexFile.is_open())
	{
		// find the length of the file
		int size = (int)VertexFile.tellg();

		// collect the file data
		char* data = new char[size];
		VertexFile.seekg(0, std::ios::beg);
		VertexFile.read(data, size);
		VertexFile.close();

		output.Set(data, size);
	}
}



////////////
// MEMBER
RenderManager::D3DDeviceResources::D3DDeviceResources()
{
	m_isVsyncEnabled = false;
	m_videoCardMemory = 0;

	m_device = nullptr;
	m_deviceContext = nullptr;

	m_swapChain = nullptr;
	m_renderTargetView = nullptr;

	m_depthStencilState = nullptr;
	m_depthStencilView = nullptr;
	m_depthStencilBuffer = nullptr;

	m_vertexShader = nullptr;
	m_pixelShader = nullptr;
	m_inputLayout = nullptr;

	m_rasterState = nullptr;
}

RenderManager::D3DDeviceResources::D3DDeviceResources(
	const RenderManager::D3DDeviceResources& clone)
{

}

RenderManager::D3DDeviceResources::~D3DDeviceResources()
{
	Release();
}

void RenderManager::D3DDeviceResources::Release()
{
	// Before shutting down set to windowed mode or when you release the swap chain it will throw an exception.
	if (m_swapChain)
	{
		m_swapChain->SetFullscreenState(false, NULL);
	}

	if (m_rasterState)
	{
		m_rasterState->Release();
		m_rasterState = nullptr;
	}

	if (m_depthStencilView)
	{
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}

	if (m_depthStencilState)
	{
		m_depthStencilState->Release();
		m_depthStencilState = nullptr;
	}

	if (m_depthStencilBuffer)
	{
		m_depthStencilBuffer->Release();
		m_depthStencilBuffer = nullptr;
	}

	if (m_renderTargetView)
	{
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}

	if (m_deviceContext)
	{
		m_deviceContext->Release();
		m_deviceContext = nullptr;
	}

	if (m_device)
	{
		m_device->Release();
		m_device = nullptr;
	}

	if (m_swapChain)
	{
		m_swapChain->Release();
		m_swapChain = nullptr;
	}

	if (m_vertexShader)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}

	if (m_pixelShader)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	if (m_inputLayout)
	{
		m_inputLayout->Release();
		m_inputLayout = nullptr;
	}
}

bool RenderManager::D3DDeviceResources::Initialize(float screenWidth, float screenHeight, HWND hwnd)
{
	bool success;


	// Get Screensize
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_hwnd = hwnd;

	// Create device
	success = CreateDevice();
	if (!success)
		return false;

	// Initialize all resources
	success = InitializeResources();
	if (!success)
		return false;

	return true;
}

bool RenderManager::D3DDeviceResources::Initialize(
	const D3D_FEATURE_LEVEL* featureLevels, 
	const uint numFeature,
	float screenWidth,
	float screenHeight,
	HWND hwnd)
{
	bool success;


	// Get Screensize
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	m_hwnd = hwnd;

	// Create device
	success = CreateDevice(featureLevels, numFeature);
	if (!success)
		return false;

	// Initialize all resources
	success = InitializeResources();
	if (!success)
		return false;

	return true;
}

bool RenderManager::D3DDeviceResources::InitializeResources()
{
	bool success;


	// Create depth buffer
	CreateDepthStencilView();

	// Create swap chain
	success = CreateSwapchainAndRenderTarget(m_hwnd);
	if (!success)
		return false;

	// Create and setup view port
	CreateViewPort(m_screenWidth, m_screenHeight);

	// Create graphic's pipeline
	InitPipeline();

	return true;
}

bool RenderManager::D3DDeviceResources::CreateDevice()
{
	// Create device using a set of suitable level
	static const D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
	};

	uint numFeature = ARRAYSIZE(featureLevels);

	return CreateDevice(&featureLevels[0], numFeature);
}

bool RenderManager::D3DDeviceResources::CreateDevice(const D3D_FEATURE_LEVEL* featureLevels, const uint numFeature)
{
	HRESULT hr;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;

	// Remember that ID3D11Device stand for DirectX11.
	// ID3D11Device1 stand for DirectX11.1 and so on

	// Create the device and device context objects
	// Loop through all feature levels and create device
	for (unsigned int level = 0; level < numFeature; level++)
	{
		hr = D3D11CreateDevice(
			nullptr, //---------------> // See (IDXGIAdapter)
										// - Interface to and graphic card adapter
										// - Use this to find GPU information and 
										// choose the best card.

			D3D_DRIVER_TYPE_HARDWARE,	// See (D3D_DRIVER_TYPE)
										// - Driver type, to get detail of all type
										// head over to http://msdn.microsoft.com/en-us/library/windows/desktop/ff476328%28v=vs.85%29.aspx
										// - We will mostly care about type HARDWARE
										// which mean to use hardware accelerated
										// graphic (On hardware DirectX)

			nullptr, //---------------> // See (HMODULE)
										// - This parameter allow us to include our
										// own software engine.

			0, //---------------------> // UINT Flags. See (D3D11_CREATE_DEVICE_FLAG)
										// - These flags enable many function like:
										// debug, BGRA support...

			&featureLevels[level], //-> // See (D3D_FEATURE_LEVEL)
										// - Each major version of Direct3D has a 
										// series of video card features that are
										// required. Customers will have different
										// video cards, so this help you to create
										// a list of feature levels which you are
										// expecting your program to work with.

			1, //---------------------> // - This parameter indicates how many feature 
										// levels you had in the list

			D3D11_SDK_VERSION, //-----> // UINT SDKVersion 
										// - This always the same: D3D11_SDK_VERSION.
										// This tells the user's DirectX which version
										// we developed our game for.

			&device, //---------------> // See (ID3D11Device)
										// - This is a pointer to a pointer to the device
										// interface.

			nullptr, //---------------> // See (D3D_FEATURE_LEVEL)
										// - This is a pointer to feature level variable.
										// When this function id completed, the variable
										// will be filled with the flag of the highest 
										// feature level that was found.

			&deviceContext); //-----> // See (ID3D11DeviceContext)

		if (SUCCEEDED(hr))
		{
			m_device = (ID3D11Device1*)device;
			m_deviceContext = (ID3D11DeviceContext1*)deviceContext;
			return true;
		}
	}

	return false;
}


void RenderManager::D3DDeviceResources::CreateDepthStencilView()
{
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;


	// Create description of z-buffer texture. It will be 2D texture
	textureDesc = { 0 };

	textureDesc.Width = m_screenWidth;
	textureDesc.Height = m_screenHeight;
	textureDesc.ArraySize = 1;
	textureDesc.MipLevels = 1;	// - Mip-levels can be use to automatically create multiple
								// versions of the texture, where each successive texture
								// is half the size. (In z-buffer this is useless so we use 1)

	textureDesc.SampleDesc.Count = 1;
	textureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	// Create a texture to be use as depth stencil view (z-buffer)
	m_device->CreateTexture2D(&textureDesc, nullptr, &m_depthStencilBuffer);

	// Initialize the description of the stencil state.
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	// Set up the description of the stencil state.
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	// Stencil operations if pixel is front-facing.
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Stencil operations if pixel is back-facing.
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	// Create the depth stencil state.
	m_device->CreateDepthStencilState(&depthStencilDesc, &m_depthStencilState);

	// Set the depth stencil state.
	m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);

	// Create z-buffer
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));	// Clear all memory

	// Set z-buffer to 2D buffer
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;		// 24 bit of stencil and 8 bit for deapth
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	// Create z-buffer
	m_device->CreateDepthStencilView(m_depthStencilBuffer, &depthStencilViewDesc, &m_depthStencilView);
}

bool RenderManager::D3DDeviceResources::CreateSwapchainAndRenderTarget(HWND inHwnd)
{
	IDXGIDevice1* dxgiDevice;
	IDXGIAdapter* dxgiAdapter;
	IDXGIFactory2* dxgiFactory;
	DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
	HWND hwnd;
	HRESULT hr;
	bool success;


	// Get window handler
	hwnd = inHwnd;
	if (hwnd == nullptr)
		return false;

	// Cast device to dixgi device
	hr = m_device->QueryInterface(&dxgiDevice);
	if (!SUCCEEDED(hr))
		return false;

	// Second, use the IDXGIDevice1 interface to get access to the adapter
	hr = dxgiDevice->GetAdapter(&dxgiAdapter);
	if (!SUCCEEDED(hr))
		return false;

	// Third, use the IDXGIAdapter interface to get access to the factory
	hr = dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));
	if (!SUCCEEDED(hr))
		return false;

	// __uuidof is and COM-speak for the unique identifying code for an asociated COM object.
	// As above, we use it to tell GetParent method that we want to get an IDXGIFactory interface

	// Now, to create a swap chain, we need to create and fill in it's desciption
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// - This swap chain is use 
	// for redering output perpose.

	swapChainDesc.BufferCount = 2; //-----------------------------> // - We only need 2 buffer, front
	// and back buffer.
	swapChainDesc.BufferDesc.Width = m_screenWidth;
	swapChainDesc.BufferDesc.Height = m_screenHeight;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;

	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = true;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;	// - Flip buffer after use.

	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.SampleDesc.Count = 1; //------------------------> // - This number tell swap chain how
																	// to perform anti-aliased. (1 means
																	// no anti-alias)
																	// Other swap chain description include:
																	// swapChainDesc.Width and swapChainDesc.Height (set 0 to automatically sized)
																	// swapChainDesc.Scaling with DXGI_SCALING_STRETCH / DXGI_SCALING_NONE
																	// swapChainDesc.SampleDesc.Quality is the quality of the anti - aliasing(0 is default)
																	// swapChainDesc.AlphaMode sets how semi-transparency works
																	// swapChainDesc.Stereo enable use of 3D glasses
																	// swapChainDesc.Flags some even more advanced options

	// With all of the description fill, we can create swapchain now
	hr = dxgiFactory->CreateSwapChain(m_device, &swapChainDesc, &m_swapChain);
	if (!SUCCEEDED(hr))
		return false;

	// Create render target (where the render process draw into)
	success = CreateRenderTarget();
	if (!success)
		return false;

	return true;
}

bool RenderManager::D3DDeviceResources::CreateRenderTarget()
{
	ID3D11Texture2D* backbuffer;
	HRESULT hr;


	// A render target is a simple COM object that maintains a location in video memory for you to render into

	// Get a pointer directly to the back buffer
	hr = m_swapChain->GetBuffer(0,	// Number of back buffer we want to obtain. We only use one
		// back buffer and it is back buffer #0
		__uuidof(ID3D11Texture2D), (void**)&backbuffer);
	if (!SUCCEEDED(hr))
		return false;

	// Create a render target pointing to the back buffer
	hr = m_device->CreateRenderTargetView(backbuffer, nullptr, &m_renderTargetView);
	if (!SUCCEEDED(hr))
		return false;

	// We do not need Render target view description for this renderTarget because it is a default back-buffer
	// render target. Use by swap chain to render to user's monitor.

	return true;
}

void RenderManager::D3DDeviceResources::InitPipeline()
{
	VS_CreateVertexShader();
	PS_CreatePixelShader();

	m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void RenderManager::D3DDeviceResources::VS_CreateVertexShader()
{
    ArrPtr<char> VSFileData;
	LoadShaderFile(SPL_VERTEX_SHADER_FILE_NAME, VSFileData);

	m_device->CreateVertexShader(&VSFileData.Get(0), VSFileData.Size(), nullptr, &m_vertexShader);
	m_deviceContext->VSSetShader(m_vertexShader, nullptr, 0);

	IA_SetInputLayout(VSFileData);
}

void RenderManager::D3DDeviceResources::PS_CreatePixelShader()
{
    ArrPtr<char> PSFileData;
	LoadShaderFile(SPL_PIXEL_SHADER_FILE_NAME, PSFileData);

	m_device->CreatePixelShader(&PSFileData.Get(0), PSFileData.Size(), nullptr, &m_pixelShader);
	m_deviceContext->PSSetShader(m_pixelShader, nullptr, 0);
}

void RenderManager::D3DDeviceResources::IA_SetInputLayout(ArrPtr<char>& shaderSrc)
{
	D3D11_INPUT_ELEMENT_DESC inputDescription[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// create the input layout
	m_device->CreateInputLayout(inputDescription, ARRAYSIZE(inputDescription),
		&shaderSrc.Get(0), shaderSrc.Size(), &m_inputLayout);

	m_deviceContext->IASetInputLayout(m_inputLayout);
}

void RenderManager::D3DDeviceResources::CreateViewPort(float screenWidth, float screenHeight)
{
	m_viewPort.TopLeftX = 0;
	m_viewPort.TopLeftY = 0;
	m_viewPort.Width = screenWidth;
	m_viewPort.Height = screenHeight;
	m_viewPort.MinDepth = 0;    // the closest an object can be on the depth buffer is 0.0
	m_viewPort.MaxDepth = 1;    // the farthest an object can be on the depth buffer is 1.0

	m_deviceContext->RSSetViewports(1, &m_viewPort);
}

void RenderManager::D3DDeviceResources::BeginRender(hkRGBAColor clearColor)
{
	float color[4] = { clearColor.r, clearColor.g, clearColor.b, clearColor.a };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, color);
}

bool RenderManager::D3DDeviceResources::EndRender()
{
	HRESULT result;
	result = m_swapChain->Present(1, 0);

	if (SUCCEEDED(result))
		return true;

	return false;
}

//=============================================================================
#pragma endregion // Device's resource
//=============================================================================
#pragma once


/////////////
// INCLUDE

// Direct 3D library
#include <d3d11_2.h>		// 3D lib
#include <d3dcompiler.h>
#include <string>

// Custom library
#include "hkUtilLib\core\Color.h"


//////////////////
// LINK LIBRARY
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


///////////////////////////////
// CLASS: D3DDeviceResources
namespace RenderManager
{
	class D3DDeviceResources
	{
		////////////
		// STATIC
	private:
		static D3DDeviceResources* s_instance;
		void LoadShaderFile(std::string File, ArrPtr<char>& output);

	public:
		static bool CreateInstance(float, float, HWND);
		static bool CreateInstance(const D3D_FEATURE_LEVEL*, const uint, float , float, HWND);

		static D3DDeviceResources* GetInstance();
		static void ReleaseInstance();


		////////////
		// MEMBER
	public:
		D3DDeviceResources();
		D3DDeviceResources(const D3DDeviceResources&);
		~D3DDeviceResources();

		bool Initialize(float, float, HWND);
		bool Initialize(const D3D_FEATURE_LEVEL*, const uint, float, float, HWND);

		void Release();

		void BeginRender(hkRGBAColor clearColor = hkRGBAColor::WHITE);
		bool EndRender();

	private:
		bool InitializeResources();

		bool CreateDevice();
		bool CreateDevice(const D3D_FEATURE_LEVEL* featureLevels, const uint numFeature);

		void CreateDepthStencilView();
		bool CreateSwapchainAndRenderTarget(HWND hwnd);
		bool CreateRenderTarget();
		void CreateViewPort(float, float);

		void InitPipeline();
		void IA_SetInputLayout(ArrPtr<char>&);
		void VS_CreateVertexShader();
		void PS_CreatePixelShader();

	private:
		bool m_isVsyncEnabled;
		float m_screenWidth, m_screenHeight;

		int m_videoCardMemory;
		char m_videoCardDescription[128];

		HWND m_hwnd;

		ID3D11Device1* m_device;
		ID3D11DeviceContext1* m_deviceContext;

		IDXGISwapChain* m_swapChain;
		ID3D11RenderTargetView* m_renderTargetView;

		ID3D11DepthStencilState* m_depthStencilState;
		ID3D11DepthStencilView* m_depthStencilView;
		ID3D11Texture2D* m_depthStencilBuffer;

		ID3D11RasterizerState* m_rasterState;

		// Pipeline
		ID3D11VertexShader* m_vertexShader;
		ID3D11PixelShader* m_pixelShader;
		ID3D11InputLayout* m_inputLayout;

		D3D11_VIEWPORT m_viewPort;
	};
}

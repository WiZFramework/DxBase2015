
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct RenderState;
	//	用途: レンダリングステートクラス
	//--------------------------------------------------------------------------------------
	struct RenderState::Impl{
		//ブレンドステート
		ComPtr<ID3D11BlendState> m_OpaquePtr;
		ComPtr<ID3D11BlendState> m_AlphaBlendPtr;
		ComPtr<ID3D11BlendState> m_AlphaBlendExPtr;

		ComPtr<ID3D11BlendState> m_AdditivePtr;
		ComPtr<ID3D11BlendState> m_NonPremultipliedPtr;
		//デプスステンシルステート
		ComPtr<ID3D11DepthStencilState> m_DepthNonePtr;
		ComPtr<ID3D11DepthStencilState> m_DepthDefaultPtr;
		ComPtr<ID3D11DepthStencilState> m_DepthReadPtr;
		//ラスタライザステート
		ComPtr<ID3D11RasterizerState> m_CullNonePtr;
		ComPtr<ID3D11RasterizerState> m_CullNoneScissorPtr;
		ComPtr<ID3D11RasterizerState> m_CullFrontPtr;
		ComPtr<ID3D11RasterizerState> m_CullFrontScissorPtr;
		ComPtr<ID3D11RasterizerState> m_CullBackPtr;
		ComPtr<ID3D11RasterizerState> m_CullBackScissorPtr;
		ComPtr<ID3D11RasterizerState> m_WireframePtr;
		ComPtr<ID3D11RasterizerState> m_WireframeScissorPtr;
		//サンプラーステート
		ComPtr<ID3D11SamplerState> m_PointWrapPtr;
		ComPtr<ID3D11SamplerState> m_PointClampPtr;
		ComPtr<ID3D11SamplerState> m_LinearWrapPtr;
		ComPtr<ID3D11SamplerState> m_LinearClampPtr;
		ComPtr<ID3D11SamplerState> m_AnisotropicWrapPtr;
		ComPtr<ID3D11SamplerState> m_AnisotropicClampPtr;
		ComPtr<ID3D11SamplerState> m_ComparisonLinearPtr;
		//ミューテックス
		std::mutex Mutex;

		//--------------------------------------------------------------------------------------
		//	Impl(};
		//	用途: コンストラクタ
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		Impl(){}
		~Impl(){}
		//--------------------------------------------------------------------------------------
		//	void CreateBlendState(
		//	D3D11_BLEND srcBlend,		//ソースブレンド
		//	D3D11_BLEND destBlend,		//デストブレンド
		//	ID3D11BlendState** pResult	//受け取るインターフェイス
		//	);
		//	用途: ブレンドステートを作成するヘルパー関数
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend,
			ID3D11BlendState** pResult);
		//--------------------------------------------------------------------------------------
		//	void CreateDepthStencilState(
		//	bool enable,
		//	bool writeEnable,
		//	ID3D11DepthStencilState** pResult
		//	);
		//	用途: デプスステンシルステートを作成するヘルパー関数
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void CreateDepthStencilState(bool enable, bool writeEnable,
			ID3D11DepthStencilState** pResult);
		//--------------------------------------------------------------------------------------
		//	void CreateRasterizerState(
		//	D3D11_CULL_MODE cullMode,			//カリング
		//	D3D11_FILL_MODE fillMode,			//塗り
		//	ID3D11RasterizerState** pResult,		//受け取るインターフェイス
		//	bool Scissor = false				//画面分割するかどうか
		//	);
		//	用途: ラスタライザステートを作成するヘルパー関数
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
			ID3D11RasterizerState** pResult, bool Scissor = false);
		//--------------------------------------------------------------------------------------
		//	void CreateSamplerState(
		//	D3D11_FILTER filter,					//フィルター
		//	D3D11_TEXTURE_ADDRESS_MODE addressMode	//アドレスモード
		//	ID3D11SamplerState** pResult			//受け取るインターフェイス
		//	);
		//	用途: サンプラーステートを作成するヘルパー関数
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void CreateSamplerState(D3D11_FILTER filter,
			D3D11_TEXTURE_ADDRESS_MODE addressMode,
			ID3D11SamplerState** pResult);


	};

	//--------------------------------------------------------------------------------------
	//	void RenderState::Impl::CreateBlendState(
	//	D3D11_BLEND srcBlend,		//ソースブレンド
	//	D3D11_BLEND destBlend,		//デストブレンド
	//	ID3D11BlendState** pResult	//受け取るインターフェイス
	//	);
	//	用途: ブレンドステートを作成するヘルパー関数
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	void RenderState::Impl::CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend,
		ID3D11BlendState** pResult){
		try{
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();

			D3D11_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.RenderTarget[0].BlendEnable = (srcBlend != D3D11_BLEND_ONE) ||
				(destBlend != D3D11_BLEND_ZERO);

			desc.RenderTarget[0].SrcBlend = desc.RenderTarget[0].SrcBlendAlpha = srcBlend;
			desc.RenderTarget[0].DestBlend = desc.RenderTarget[0].DestBlendAlpha = destBlend;
			desc.RenderTarget[0].BlendOp = desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			HRESULT hr = pDx11Device->CreateBlendState(&desc, pResult);
			if (FAILED(hr)){
				// 初期化失敗
				throw BaseException(
					L"ブレンドステート作成に失敗しました。",
					L"if(FAILED(pDx11Device->CreateBlendState()))",
					L"RenderState::Impl::CreateBlendState()"
					);
			}
		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	void RenderState::Impl::CreateDepthStencilState(
	//	bool enable,
	//	bool writeEnable,
	//	ID3D11DepthStencilState** pResult
	//	);
	//	用途: デプスステンシルステートを作成するヘルパー関数
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	void RenderState::Impl::CreateDepthStencilState(bool enable, bool writeEnable,
		ID3D11DepthStencilState** pResult){
		try{
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();

			D3D11_DEPTH_STENCIL_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.DepthEnable = enable;
			desc.DepthWriteMask = writeEnable ? D3D11_DEPTH_WRITE_MASK_ALL : D3D11_DEPTH_WRITE_MASK_ZERO;
			desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

			desc.StencilEnable = false;
			desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

			desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
			desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

			desc.BackFace = desc.FrontFace;

			HRESULT hr = pDx11Device->CreateDepthStencilState(&desc, pResult);
			if (FAILED(hr)){
				// 初期化失敗
				throw BaseException(
					L"デプスステンシルステート作成に失敗しました。",
					L"if(FAILED(pDx11Device->CreateDepthStencilState()))",
					L"RenderState::Impl::CreateDepthStencilState()"
					);
			}
		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	void RenderState::Impl::CreateRasterizerState(
	//	D3D11_CULL_MODE cullMode,			//カリング
	//	D3D11_FILL_MODE fillMode,			//塗り
	//	ID3D11RasterizerState** pResult,		//受け取るインターフェイス
	//	bool Scissor = false				//画面分割するかどうか
	//	);
	//	用途: ラスタライザステートを作成するヘルパー関数
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	void RenderState::Impl::CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
		ID3D11RasterizerState** pResult, bool Scissor){
		try{
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();

			D3D11_RASTERIZER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.CullMode = cullMode;
			desc.FillMode = fillMode;
			desc.DepthClipEnable = true;
			desc.MultisampleEnable = true;
			desc.ScissorEnable = Scissor;

			HRESULT hr = pDx11Device->CreateRasterizerState(&desc, pResult);
			if (FAILED(hr)){
				// 初期化失敗
				throw BaseException(
					L"ラスタライザステート作成に失敗しました。",
					L"if(FAILED(pDx11Device->CreateRasterizerState()))",
					L"RenderState::Impl::CreateRasterizerState()"
					);
			}
		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	void RenderState::Impl::CreateSamplerState(
	//	D3D11_FILTER filter,					//フィルター
	//	D3D11_TEXTURE_ADDRESS_MODE addressMode	//アドレスモード
	//	ID3D11SamplerState** pResult			//受け取るインターフェイス
	//	);
	//	用途: サンプラーステートを作成するヘルパー関数
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	void RenderState::Impl::CreateSamplerState(D3D11_FILTER filter,
		D3D11_TEXTURE_ADDRESS_MODE addressMode,
		ID3D11SamplerState** pResult){
		try{

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();

			D3D11_SAMPLER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.Filter = filter;

			desc.AddressU = addressMode;
			desc.AddressV = addressMode;
			desc.AddressW = addressMode;

			desc.MaxAnisotropy = (pDx11Device->GetFeatureLevel() > D3D_FEATURE_LEVEL_9_1) ? 16 : 2;

			desc.MaxLOD = FLT_MAX;
			desc.ComparisonFunc = D3D11_COMPARISON_NEVER;

			HRESULT hr = pDx11Device->CreateSamplerState(&desc, pResult);

			if (FAILED(hr)){
				// 初期化失敗
				throw BaseException(
					L"サンプラーステート作成に失敗しました。",
					L"if(FAILED(pDx11Device->CreateSamplerState()))",
					L"RenderState::Impl::CreateSamplerState()"
					);
			}
		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	class RenderState;
	//	用途: レンダリングステートクラス
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//	RenderState::RenderState();
	//	用途: コンストラクタ
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	RenderState::RenderState() :
		pImpl(new Impl())
	{
	}

	RenderState::~RenderState(){
	}

	//--------------------------------------------------------------------------------------
	//	用途: ブレンドステートアクセッサ
	//	戻り値: ブレンドステートのポインタ
	//--------------------------------------------------------------------------------------
	ID3D11BlendState* RenderState::GetOpaque()const{
		return Util::DemandCreate(pImpl->m_OpaquePtr, pImpl->Mutex, [&](ID3D11BlendState** pResult)
		{
			pImpl->CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_ZERO, pResult);
		});

	}

	ID3D11BlendState* RenderState::GetAlphaBlend()const{
		return Util::DemandCreate(pImpl->m_AlphaBlendPtr, pImpl->Mutex, [&](ID3D11BlendState** pResult)
		{
			pImpl->CreateBlendState(D3D11_BLEND_ONE, D3D11_BLEND_INV_SRC_ALPHA, pResult);
		});
	}

	ID3D11BlendState* RenderState::GetAlphaBlendEx()const{
		if (!pImpl->m_AlphaBlendExPtr){
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();
			D3D11_BLEND_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.RenderTarget[0].BlendEnable = true;
			desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
			desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
			//		desc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA;
			desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
			desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
			desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

			HRESULT hr = pDx11Device->CreateBlendState(&desc, &pImpl->m_AlphaBlendExPtr);
			if (FAILED(hr)){
				// 初期化失敗
				throw BaseException(
					L"ブレンドステート作成に失敗しました。",
					L"if(FAILED(pDx11Device->CreateBlendState()))",
					L"BasicState::GetAlphaBlendEx()"
					);
			}
		}
		return pImpl->m_AlphaBlendExPtr.Get();
	}


	ID3D11BlendState* RenderState::GetAdditive()const{
		return Util::DemandCreate(pImpl->m_AdditivePtr, pImpl->Mutex, [&](ID3D11BlendState** pResult)
		{
			pImpl->CreateBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE, pResult);
		});
	}

	ID3D11BlendState*  RenderState::GetNonPremultiplied()const{
		return Util::DemandCreate(pImpl->m_NonPremultipliedPtr, pImpl->Mutex, [&](ID3D11BlendState** pResult)
		{
			pImpl->CreateBlendState(D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA, pResult);
		});
	}

	//--------------------------------------------------------------------------------------
	//	用途: デプスステンシルステートアクセッサ
	//	戻り値: デプスステンシルステートのポインタ
	//--------------------------------------------------------------------------------------
	ID3D11DepthStencilState* RenderState::GetDepthNone()const{
		return Util::DemandCreate(pImpl->m_DepthNonePtr, pImpl->Mutex, [&](ID3D11DepthStencilState** pResult)
		{
			pImpl->CreateDepthStencilState(false, false, pResult);
		});
	}

	ID3D11DepthStencilState* RenderState::GetDepthDefault()const{
		return Util::DemandCreate(pImpl->m_DepthDefaultPtr, pImpl->Mutex, [&](ID3D11DepthStencilState** pResult)
		{
			pImpl->CreateDepthStencilState(true, true, pResult);
		});
	}

	ID3D11DepthStencilState* RenderState::GetDepthRead()const{
		return Util::DemandCreate(pImpl->m_DepthReadPtr, pImpl->Mutex, [&](ID3D11DepthStencilState** pResult)
		{
			pImpl->CreateDepthStencilState(true, false, pResult);
		});
	}

	//--------------------------------------------------------------------------------------
	//	用途: ラスタライザステートアクセッサ
	//	戻り値: ラスタライザステートのポインタ
	//--------------------------------------------------------------------------------------
	ID3D11RasterizerState* RenderState::GetCullNone()const{
		return Util::DemandCreate(pImpl->m_CullNonePtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, pResult);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullNoneScissor()const{
		return Util::DemandCreate(pImpl->m_CullNoneScissorPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_NONE, D3D11_FILL_SOLID, pResult, true);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullFront()const{
		return Util::DemandCreate(pImpl->m_CullFrontPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID, pResult);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullFrontScissor()const{
		return Util::DemandCreate(pImpl->m_CullFrontScissorPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_FRONT, D3D11_FILL_SOLID, pResult, true);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullBack()const{
		return Util::DemandCreate(pImpl->m_CullBackPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, pResult);
		});
	}

	ID3D11RasterizerState* RenderState::GetCullBackScissor()const{
		return Util::DemandCreate(pImpl->m_CullBackScissorPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_SOLID, pResult, true);
		});
	}

	ID3D11RasterizerState* RenderState::GetWireframe()const{
		return Util::DemandCreate(pImpl->m_WireframePtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME, pResult);
		});
	}

	ID3D11RasterizerState* RenderState::GetWireframeScissor()const{
		return Util::DemandCreate(pImpl->m_WireframeScissorPtr, pImpl->Mutex, [&](ID3D11RasterizerState** pResult)
		{
			pImpl->CreateRasterizerState(D3D11_CULL_BACK, D3D11_FILL_WIREFRAME, pResult, true);
		});
	}

	//--------------------------------------------------------------------------------------
	//	用途: サンプラーステートアクセッサ
	//	戻り値: サンプラーステートのポインタ
	//--------------------------------------------------------------------------------------
	ID3D11SamplerState* RenderState::GetPointWrap()const{
		return Util::DemandCreate(pImpl->m_PointWrapPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_WRAP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetPointClamp()const{
		return Util::DemandCreate(pImpl->m_PointClampPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_POINT, D3D11_TEXTURE_ADDRESS_CLAMP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetLinearWrap()const{
		return Util::DemandCreate(pImpl->m_LinearWrapPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_WRAP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetLinearClamp()const{
		return Util::DemandCreate(pImpl->m_LinearClampPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_MIN_MAG_MIP_LINEAR, D3D11_TEXTURE_ADDRESS_CLAMP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetAnisotropicWrap()const{
		return Util::DemandCreate(pImpl->m_AnisotropicWrapPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetAnisotropicClamp()const{
		return Util::DemandCreate(pImpl->m_AnisotropicClampPtr, pImpl->Mutex, [&](ID3D11SamplerState** pResult)
		{
			pImpl->CreateSamplerState(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_CLAMP, pResult);
		});
	}

	ID3D11SamplerState* RenderState::GetComparisonLinear()const{
		if (!pImpl->m_ComparisonLinearPtr){
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			D3D11_SAMPLER_DESC comparisonSamplerDesc;
			ZeroMemory(&comparisonSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
			comparisonSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
			comparisonSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
			comparisonSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
			comparisonSamplerDesc.BorderColor[0] = 1.0f;
			comparisonSamplerDesc.BorderColor[1] = 1.0f;
			comparisonSamplerDesc.BorderColor[2] = 1.0f;
			comparisonSamplerDesc.BorderColor[3] = 1.0f;
			comparisonSamplerDesc.MinLOD = 0.f;
			comparisonSamplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			comparisonSamplerDesc.MipLODBias = 0.f;
			comparisonSamplerDesc.MaxAnisotropy = 0;
			comparisonSamplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
			comparisonSamplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
			ThrowIfFailed(
				pDx11Device->CreateSamplerState(&comparisonSamplerDesc, &pImpl->m_ComparisonLinearPtr),
				L"比較用サンプラーの作成に失敗しました",
				L"pDx11Device->CreateSamplerState(&comparisonSamplerDesc, &pImpl->m_ComparisonSampler_linear)",
				L"RenderState::GetComparisonLinear()"
				);
		}
		return pImpl->m_ComparisonLinearPtr.Get();
	}



	//--------------------------------------------------------------------------------------
	//	struct DefaultRenderTarget::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct DefaultRenderTarget::Impl{
		//ビュー関連
		ComPtr<ID3D11RenderTargetView> m_D3D11RenderTargetView;	//レンダリングターゲットレビュー
		ComPtr<ID3D11Texture2D>		m_DepthStencil;		//深度ステンシルバッファ
		ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;	//深度ステンシルビュー
		ComPtr<ID2D1Bitmap1>		m_d2dTargetBitmap;
		Impl()
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class DefaultRenderTarget : public RenderTarget;
	//	用途: デフォルトのレンダーターゲット
	//	＊デフォルトのレンダラー
	//--------------------------------------------------------------------------------------
	//構築
	DefaultRenderTarget::DefaultRenderTarget() :
		pImpl(new Impl())
	{
		try{

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11Device = Dev->GetD3DDevice();
			auto pSwapChain = Dev->GetSwapChain();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto pD2D11DeviceContext = Dev->GetD2DDeviceContext();


			//レンダリングターゲットビューの作成
			ComPtr<ID3D11Texture2D> pBackBuffer;
			//まずバックバッファのポインタを得る
			ThrowIfFailed(
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer),
				L"スワップチェーンからバックバッファの取得に失敗しました。",
				L"pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
				);
			//バックバッファからレンダリングターゲットのビューを作成する
			ThrowIfFailed(
				pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pImpl->m_D3D11RenderTargetView),
				L"DX11バックバッファからのレンダリングターゲットビューを作成に失敗しました。",
				L"pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_D3D11RenderTargetView)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
				);

			//深度テクスチャの作成
			D3D11_TEXTURE2D_DESC descDepth;
			ZeroMemory(&descDepth, sizeof(descDepth));
			descDepth.Width = App::GetApp()->GetGameWidth();
			descDepth.Height = App::GetApp()->GetGameHeight();
			descDepth.MipLevels = 1;
			descDepth.ArraySize = 1;
			descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDepth.SampleDesc.Count = 1;
			descDepth.SampleDesc.Quality = 0;
			descDepth.Usage = D3D11_USAGE_DEFAULT;
			descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
			descDepth.CPUAccessFlags = 0;
			descDepth.MiscFlags = 0;

			ThrowIfFailed(
				pD3D11Device->CreateTexture2D(&descDepth, nullptr, &pImpl->m_DepthStencil),
				L"DX11深度テクスチャの作成失敗の作成に失敗しました。",
				L"pD3D11Device->CreateTexture2D(&descDepth, nullptr, &m_DepthStencil)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
				);

			//深度ステンシルビューの作成
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;

			ThrowIfFailed(
				pD3D11Device->CreateDepthStencilView(pImpl->m_DepthStencil.Get(), &descDSV, &pImpl->m_DepthStencilView),
				L"DX11深度ステンシルビューの作成に失敗しました。",
				L"pD3D11Device->CreateDepthStencilView(m_DepthStencil.Get(), &descDSV, &m_DepthStencilView)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
				);

			ComPtr<IDXGISurface2> dxgiBackBuffer;
			ThrowIfFailed(
				pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)),
				L"2dデバイスコンテキスト作成に失敗しました。",
				L"m_d2dDevice->CreateDeviceContext()",
				L"DeviceResources::Impl::CreateDeviceResources()"
				);


			ThrowIfFailed(
				pD2D11DeviceContext->CreateBitmapFromDxgiSurface(
				dxgiBackBuffer.Get(),
				nullptr,	//デフォルト設定
				&pImpl->m_d2dTargetBitmap
				),
				L"2dビットマップ作成に失敗しました。",
				L"pD2D11DeviceContext->CreateBitmapFromDxgiSurface()",
				L"DefaultRenderTarget::DefaultRenderTarget()"
				);

			pD2D11DeviceContext->SetTarget(pImpl->m_d2dTargetBitmap.Get());
			//グレースケール テキストのアンチエイリアシング
			pD2D11DeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

		}
		catch (...){
			throw;
		}
	}
	DefaultRenderTarget::~DefaultRenderTarget(){}


	//アクセサ
	ID3D11RenderTargetView* DefaultRenderTarget::GetRenderTargetView() const { return pImpl->m_D3D11RenderTargetView.Get(); }
	ID3D11Texture2D* DefaultRenderTarget::GetDepthStencil() const{ return pImpl->m_DepthStencil.Get(); }
	ID3D11DepthStencilView*	DefaultRenderTarget::GetDepthStencilView() const{ return pImpl->m_DepthStencilView.Get(); }
	ID2D1Bitmap1*			DefaultRenderTarget::GetD2DTargetBitmap() const	{ return pImpl->m_d2dTargetBitmap.Get(); }


	//操作
	//スクリーン全体を指定の色でクリアする
	void DefaultRenderTarget::ClearViews(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//バッファのクリア
		float Color[4] = { 0, 0, 0,1.0f };
		D3D11_VIEWPORT ViewPort;
		//ビューポートのセットアップ
		ZeroMemory(&ViewPort, sizeof(ViewPort));
		ViewPort.Width = (float)App::GetApp()->GetGameWidth();
		ViewPort.Height = (float)App::GetApp()->GetGameHeight();
		ViewPort.MinDepth = 0.0f;
		ViewPort.MaxDepth = 1.0f;
		ViewPort.TopLeftX = 0;
		ViewPort.TopLeftY = 0;
		pD3D11DeviceContext->RSSetViewports(1, &ViewPort);
		//レンダリングターゲットのクリア
		pD3D11DeviceContext->ClearRenderTargetView(pImpl->m_D3D11RenderTargetView.Get(), Color);
		//通常の深度バッファとステンシルバッファのクリア
		pD3D11DeviceContext->ClearDepthStencilView(pImpl->m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	//レンダリングターゲットを開始する
	void DefaultRenderTarget::StartRenderTarget(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		ID3D11RenderTargetView* pV = pImpl->m_D3D11RenderTargetView.Get();
		//レンダリングターゲットとステンシルを設定
		pD3D11DeviceContext->OMSetRenderTargets(1, &pV, pImpl->m_DepthStencilView.Get());
		//シェーダーリソースビューのクリア
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
		pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
		//シェーダーは指定しない
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//ブレンドは指定しない
		pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	}
	//レンダリングターゲットを終了する
	void DefaultRenderTarget::EndRenderTarget(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		//auto RenderStatePtr = App::GetApp()->GetRenderState();
		//シェーダーリソースビューのクリア
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
		pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
		//シェーダーは指定しない
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//ブレンドは指定しない
		pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}




}
//end basedx11

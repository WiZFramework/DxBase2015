#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct RenderState;
	//	�p�r: �����_�����O�X�e�[�g�N���X
	//--------------------------------------------------------------------------------------
	struct RenderState::Impl{
		//�u�����h�X�e�[�g
		ComPtr<ID3D11BlendState> m_OpaquePtr;
		ComPtr<ID3D11BlendState> m_AlphaBlendPtr;
		ComPtr<ID3D11BlendState> m_AlphaBlendExPtr;

		ComPtr<ID3D11BlendState> m_AdditivePtr;
		ComPtr<ID3D11BlendState> m_NonPremultipliedPtr;
		//�f�v�X�X�e���V���X�e�[�g
		ComPtr<ID3D11DepthStencilState> m_DepthNonePtr;
		ComPtr<ID3D11DepthStencilState> m_DepthDefaultPtr;
		ComPtr<ID3D11DepthStencilState> m_DepthReadPtr;
		//���X�^���C�U�X�e�[�g
		ComPtr<ID3D11RasterizerState> m_CullNonePtr;
		ComPtr<ID3D11RasterizerState> m_CullNoneScissorPtr;
		ComPtr<ID3D11RasterizerState> m_CullFrontPtr;
		ComPtr<ID3D11RasterizerState> m_CullFrontScissorPtr;
		ComPtr<ID3D11RasterizerState> m_CullBackPtr;
		ComPtr<ID3D11RasterizerState> m_CullBackScissorPtr;
		ComPtr<ID3D11RasterizerState> m_WireframePtr;
		ComPtr<ID3D11RasterizerState> m_WireframeScissorPtr;
		//�T���v���[�X�e�[�g
		ComPtr<ID3D11SamplerState> m_PointWrapPtr;
		ComPtr<ID3D11SamplerState> m_PointClampPtr;
		ComPtr<ID3D11SamplerState> m_LinearWrapPtr;
		ComPtr<ID3D11SamplerState> m_LinearClampPtr;
		ComPtr<ID3D11SamplerState> m_AnisotropicWrapPtr;
		ComPtr<ID3D11SamplerState> m_AnisotropicClampPtr;
		ComPtr<ID3D11SamplerState> m_ComparisonLinearPtr;
		//�~���[�e�b�N�X
		std::mutex Mutex;

		//--------------------------------------------------------------------------------------
		//	Impl(};
		//	�p�r: �R���X�g���N�^
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		Impl(){}
		~Impl(){}
		//--------------------------------------------------------------------------------------
		//	void CreateBlendState(
		//	D3D11_BLEND srcBlend,		//�\�[�X�u�����h
		//	D3D11_BLEND destBlend,		//�f�X�g�u�����h
		//	ID3D11BlendState** pResult	//�󂯎��C���^�[�t�F�C�X
		//	);
		//	�p�r: �u�����h�X�e�[�g���쐬����w���p�[�֐�
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void CreateBlendState(D3D11_BLEND srcBlend, D3D11_BLEND destBlend,
			ID3D11BlendState** pResult);
		//--------------------------------------------------------------------------------------
		//	void CreateDepthStencilState(
		//	bool enable,
		//	bool writeEnable,
		//	ID3D11DepthStencilState** pResult
		//	);
		//	�p�r: �f�v�X�X�e���V���X�e�[�g���쐬����w���p�[�֐�
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void CreateDepthStencilState(bool enable, bool writeEnable,
			ID3D11DepthStencilState** pResult);
		//--------------------------------------------------------------------------------------
		//	void CreateRasterizerState(
		//	D3D11_CULL_MODE cullMode,			//�J�����O
		//	D3D11_FILL_MODE fillMode,			//�h��
		//	ID3D11RasterizerState** pResult,		//�󂯎��C���^�[�t�F�C�X
		//	bool Scissor = false				//��ʕ������邩�ǂ���
		//	);
		//	�p�r: ���X�^���C�U�X�e�[�g���쐬����w���p�[�֐�
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void CreateRasterizerState(D3D11_CULL_MODE cullMode, D3D11_FILL_MODE fillMode,
			ID3D11RasterizerState** pResult, bool Scissor = false);
		//--------------------------------------------------------------------------------------
		//	void CreateSamplerState(
		//	D3D11_FILTER filter,					//�t�B���^�[
		//	D3D11_TEXTURE_ADDRESS_MODE addressMode	//�A�h���X���[�h
		//	ID3D11SamplerState** pResult			//�󂯎��C���^�[�t�F�C�X
		//	);
		//	�p�r: �T���v���[�X�e�[�g���쐬����w���p�[�֐�
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void CreateSamplerState(D3D11_FILTER filter,
			D3D11_TEXTURE_ADDRESS_MODE addressMode,
			ID3D11SamplerState** pResult);


	};

	//--------------------------------------------------------------------------------------
	//	void RenderState::Impl::CreateBlendState(
	//	D3D11_BLEND srcBlend,		//�\�[�X�u�����h
	//	D3D11_BLEND destBlend,		//�f�X�g�u�����h
	//	ID3D11BlendState** pResult	//�󂯎��C���^�[�t�F�C�X
	//	);
	//	�p�r: �u�����h�X�e�[�g���쐬����w���p�[�֐�
	//	�߂�l: �Ȃ�
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
				// ���������s
				throw BaseException(
					L"�u�����h�X�e�[�g�쐬�Ɏ��s���܂����B",
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
	//	�p�r: �f�v�X�X�e���V���X�e�[�g���쐬����w���p�[�֐�
	//	�߂�l: �Ȃ�
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
				// ���������s
				throw BaseException(
					L"�f�v�X�X�e���V���X�e�[�g�쐬�Ɏ��s���܂����B",
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
	//	D3D11_CULL_MODE cullMode,			//�J�����O
	//	D3D11_FILL_MODE fillMode,			//�h��
	//	ID3D11RasterizerState** pResult,		//�󂯎��C���^�[�t�F�C�X
	//	bool Scissor = false				//��ʕ������邩�ǂ���
	//	);
	//	�p�r: ���X�^���C�U�X�e�[�g���쐬����w���p�[�֐�
	//	�߂�l: �Ȃ�
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
				// ���������s
				throw BaseException(
					L"���X�^���C�U�X�e�[�g�쐬�Ɏ��s���܂����B",
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
	//	D3D11_FILTER filter,					//�t�B���^�[
	//	D3D11_TEXTURE_ADDRESS_MODE addressMode	//�A�h���X���[�h
	//	ID3D11SamplerState** pResult			//�󂯎��C���^�[�t�F�C�X
	//	);
	//	�p�r: �T���v���[�X�e�[�g���쐬����w���p�[�֐�
	//	�߂�l: �Ȃ�
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
				// ���������s
				throw BaseException(
					L"�T���v���[�X�e�[�g�쐬�Ɏ��s���܂����B",
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
	//	�p�r: �����_�����O�X�e�[�g�N���X
	//--------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------
	//	RenderState::RenderState();
	//	�p�r: �R���X�g���N�^
	//	�߂�l: �Ȃ�
	//--------------------------------------------------------------------------------------
	RenderState::RenderState() :
		pImpl(new Impl())
	{
	}

	RenderState::~RenderState(){
	}

	//--------------------------------------------------------------------------------------
	//	�p�r: �u�����h�X�e�[�g�A�N�Z�b�T
	//	�߂�l: �u�����h�X�e�[�g�̃|�C���^
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
				// ���������s
				throw BaseException(
					L"�u�����h�X�e�[�g�쐬�Ɏ��s���܂����B",
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
	//	�p�r: �f�v�X�X�e���V���X�e�[�g�A�N�Z�b�T
	//	�߂�l: �f�v�X�X�e���V���X�e�[�g�̃|�C���^
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
	//	�p�r: ���X�^���C�U�X�e�[�g�A�N�Z�b�T
	//	�߂�l: ���X�^���C�U�X�e�[�g�̃|�C���^
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
	//	�p�r: �T���v���[�X�e�[�g�A�N�Z�b�T
	//	�߂�l: �T���v���[�X�e�[�g�̃|�C���^
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
				L"��r�p�T���v���[�̍쐬�Ɏ��s���܂���",
				L"pDx11Device->CreateSamplerState(&comparisonSamplerDesc, &pImpl->m_ComparisonSampler_linear)",
				L"RenderState::GetComparisonLinear()"
				);
		}
		return pImpl->m_ComparisonLinearPtr.Get();
	}


	//--------------------------------------------------------------------------------------
	//	struct ShadowMapRenderTarget::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct ShadowMapRenderTarget::Impl{
		const float m_ShadowMapDimension;	//�V���h�E�}�b�v�̑傫��
		ComPtr<ID3D11ShaderResourceView>	m_ShaderResourceView;	//�V�F�[�_���\�[�X�r���[
		ComPtr<ID3D11RasterizerState> m_ShadowRenderState;	//�V���h�E�}�b�v�̃��X�^���C�U�X�e�[�g
		ComPtr<ID3D11Texture2D>		m_DepthStencil;		//�[�x�X�e���V���o�b�t�@
		ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;	//�[�x�X�e���V���r���[
		D3D11_VIEWPORT m_ViewPort;	//�r���[�|�[�g
		Impl():
			m_ShadowMapDimension(2048.0f)
		{}
		~Impl(){}
	};



	//--------------------------------------------------------------------------------------
	//	class ShadowMapRenderTarget: public RenderTarget;
	//	�p�r: �V���h�E�}�b�v�̃����_�����O�^�[�Q�b�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	ShadowMapRenderTarget::ShadowMapRenderTarget() :
		pImpl(new Impl())
	{
		try{
			//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11Device = Dev->GetD3DDevice();
			auto pSwapChain = Dev->GetSwapChain();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

			//�V���h�E�}�b�v�e�N�X�`���̍쐬
			D3D11_TEXTURE2D_DESC shadowMapDesc;
			ZeroMemory(&shadowMapDesc, sizeof(D3D11_TEXTURE2D_DESC));
			shadowMapDesc.Width = static_cast<UINT>(pImpl->m_ShadowMapDimension);
			shadowMapDesc.Height = static_cast<UINT>(pImpl->m_ShadowMapDimension);
			shadowMapDesc.MipLevels = 1;
			shadowMapDesc.ArraySize = 1;
			shadowMapDesc.SampleDesc.Count = 1;
			shadowMapDesc.Usage = D3D11_USAGE_DEFAULT;
			shadowMapDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			shadowMapDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

			ThrowIfFailed(
				pD3D11Device->CreateTexture2D(&shadowMapDesc, nullptr, &pImpl->m_DepthStencil),
				L"�V���h�E�}�b�v�e�N�X�`���̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateTexture2D(&shadowMapDesc, nullptr, &m_DepthStencil)",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
				);

			//�[�x�X�e���V���r���[�쐬
			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
			ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
			depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;

			ThrowIfFailed(
				pD3D11Device->CreateDepthStencilView(pImpl->m_DepthStencil.Get(), &depthStencilViewDesc, &pImpl->m_DepthStencilView),
				L"�[�x�X�e���V���r���[�̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateDepthStencilView(&m_DepthStencil, &depthStencilViewDesc, &m_DepthStencilView)",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
				);

			//�V�F�[�_�[���\�[�X�r���[�쐬
			D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
			ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
			shaderResourceViewDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
			shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			shaderResourceViewDesc.Texture2D.MipLevels = 1;

			ThrowIfFailed(
				pD3D11Device->CreateShaderResourceView(pImpl->m_DepthStencil.Get(), &shaderResourceViewDesc, &pImpl->m_ShaderResourceView),
				L"�V�F�[�_�[���\�[�X�r���[�̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateShaderResourceView(m_DepthStencil.Get(), &shaderResourceViewDesc, &m_ShaderResourceView)",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
				);


			D3D11_RASTERIZER_DESC shadowRenderStateDesc;
			ZeroMemory(&shadowRenderStateDesc, sizeof(D3D11_RASTERIZER_DESC));
			shadowRenderStateDesc.CullMode = D3D11_CULL_FRONT;
			shadowRenderStateDesc.FillMode = D3D11_FILL_SOLID;
			shadowRenderStateDesc.DepthClipEnable = true;

			ThrowIfFailed(pD3D11Device->CreateRasterizerState(&shadowRenderStateDesc,&pImpl->m_ShadowRenderState),
				L"�V���h�E�}�b�v�̃��X�^���C�U�X�e�[�g�̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateRasterizerState(&shadowRenderStateDesc,&pImpl->m_ShadowRenderState)",
				L"ShadowMapRenderTarget::ShadowMapRenderTarget()"
				);


			//�r���[�|�[�g�̍쐬
			//�V���h�E�����_�����O�r���[�|�[�g
			ZeroMemory(&pImpl->m_ViewPort, sizeof(D3D11_VIEWPORT));
			pImpl->m_ViewPort.Height = pImpl->m_ShadowMapDimension;
			pImpl->m_ViewPort.Width = pImpl->m_ShadowMapDimension;
			pImpl->m_ViewPort.MinDepth = 0.f;
			pImpl->m_ViewPort.MaxDepth = 1.f;
		}
		catch (...){
			throw;
		}
	}

	ShadowMapRenderTarget::~ShadowMapRenderTarget(){}

	//�A�N�Z�T
	float ShadowMapRenderTarget::GetShadowMapDimension() const { return pImpl->m_ShadowMapDimension; }
	ID3D11ShaderResourceView* ShadowMapRenderTarget::GetShaderResourceView() const{ return pImpl->m_ShaderResourceView.Get(); }
	ID3D11Texture2D* ShadowMapRenderTarget::GetDepthStencil() const		{ return pImpl->m_DepthStencil.Get(); }
	ID3D11DepthStencilView*	ShadowMapRenderTarget::GetDepthStencilView() const	{ return pImpl->m_DepthStencilView.Get(); }


	//�����_�����O�^�[�Q�b�g���N���A����
	void ShadowMapRenderTarget::ClearViews(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�V���h�E�}�b�v�N���A
		pD3D11DeviceContext->ClearDepthStencilView(pImpl->m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0, 0);
	}
	//�����_�����O�^�[�Q�b�g���J�n����
	void ShadowMapRenderTarget::StartRenderTarget(){
		//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�����_�����O�^�[�Q�b�g�͐[�x�X�e���V���r���[�̂ݎw��
		ID3D11RenderTargetView* pnullView = nullptr;

		pD3D11DeviceContext->OMSetRenderTargets(1, &pnullView, pImpl->m_DepthStencilView.Get());
		//�r���[�|�[�g�̐ݒ�
		pD3D11DeviceContext->RSSetViewports(1, &pImpl->m_ViewPort);
		//���X�^���C�U�X�e�[�g�̐ݒ�(�ݒ����)
		pD3D11DeviceContext->RSSetState(nullptr);
		//�V�F�[�_�[�͎w�肵�Ȃ�
		//���_�V�F�[�_�̐ݒ�(�����ł͎w�肵�Ȃ�)
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		//�s�N�Z���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		//�W�I���g���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);


	}
	//�����_�����O�^�[�Q�b�g���I������
	void ShadowMapRenderTarget::EndRenderTarget(){
		//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�����_�����O�^�[�Q�b�g�͐[�x�X�e���V���r���[�̂ݎw��
		ID3D11RenderTargetView* pnullView = nullptr;
		pD3D11DeviceContext->OMSetRenderTargets(1, &pnullView, nullptr);
		//�r���[�|�[�g�̐ݒ�
		pD3D11DeviceContext->RSSetViewports(1, &pImpl->m_ViewPort);
		//���X�^���C�U�X�e�[�g�̐ݒ�(�ݒ����)
		pD3D11DeviceContext->RSSetState(nullptr);
		//�V�F�[�_�[�͎w�肵�Ȃ�
		//���_�V�F�[�_�̐ݒ�(�����ł͎w�肵�Ȃ�)
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		//�s�N�Z���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		//�W�I���g���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
	}

	//--------------------------------------------------------------------------------------
	//	struct DefaultRenderTarget::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct DefaultRenderTarget::Impl{
		weak_ptr<Stage> m_Stage;
		//�r���[�֘A
		ComPtr<ID3D11RenderTargetView> m_D3D11RenderTargetView;	//�����_�����O�^�[�Q�b�g���r���[
		ComPtr<ID3D11Texture2D>		m_DepthStencil;		//�[�x�X�e���V���o�b�t�@
		ComPtr<ID3D11DepthStencilView>	m_DepthStencilView;	//�[�x�X�e���V���r���[

		ComPtr<ID2D1Bitmap1>		m_d2dTargetBitmap;

		Impl(const shared_ptr<Stage>& stage):
			m_Stage(stage)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class DefaultRenderTarget : public RenderTarget;
	//	�p�r: �f�t�H���g�̃����_�[�^�[�Q�b�g
	//	���f�t�H���g�̃����_���[
	//--------------------------------------------------------------------------------------
	//�\�z
	DefaultRenderTarget::DefaultRenderTarget(const shared_ptr<Stage>& stage):
		pImpl(new Impl(stage))
	{
		try{
			if (pImpl->m_Stage.expired()){
				throw BaseException(
					L"�X�e�[�W�������ł�",
					L"if (pImpl->m_Stage.expired())",
					L"DefaultRenderTarget::DefaultRenderTarget()"
					);
			}

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pD3D11Device = Dev->GetD3DDevice();
			auto pSwapChain = Dev->GetSwapChain();
			auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
			auto pD2D11DeviceContext = Dev->GetD2DDeviceContext();


			//�����_�����O�^�[�Q�b�g�r���[�̍쐬
			ComPtr<ID3D11Texture2D> pBackBuffer;
			//�܂��o�b�N�o�b�t�@�̃|�C���^�𓾂�
			ThrowIfFailed(
				pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer),
				L"�X���b�v�`�F�[������o�b�N�o�b�t�@�̎擾�Ɏ��s���܂����B",
				L"pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
				);
			//�o�b�N�o�b�t�@���烌���_�����O�^�[�Q�b�g�̃r���[���쐬����
			ThrowIfFailed(
				pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pImpl->m_D3D11RenderTargetView),
				L"DX11�o�b�N�o�b�t�@����̃����_�����O�^�[�Q�b�g�r���[���쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &m_D3D11RenderTargetView)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
				);

			//�[�x�e�N�X�`���̍쐬
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
				L"DX11�[�x�e�N�X�`���̍쐬���s�̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateTexture2D(&descDepth, nullptr, &m_DepthStencil)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
				);

			//�[�x�X�e���V���r���[�̍쐬
			D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
			ZeroMemory(&descDSV, sizeof(descDSV));
			descDSV.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			descDSV.Texture2D.MipSlice = 0;

			ThrowIfFailed(
				pD3D11Device->CreateDepthStencilView(pImpl->m_DepthStencil.Get(), &descDSV, &pImpl->m_DepthStencilView),
				L"DX11�[�x�X�e���V���r���[�̍쐬�Ɏ��s���܂����B",
				L"pD3D11Device->CreateDepthStencilView(m_DepthStencil.Get(), &descDSV, &m_DepthStencilView)",
				L"DefaultRenderTarget::DefaultRenderTarget()"
				);

			ComPtr<IDXGISurface2> dxgiBackBuffer;
			ThrowIfFailed(
				pSwapChain->GetBuffer(0, IID_PPV_ARGS(&dxgiBackBuffer)),
				L"2d�f�o�C�X�R���e�L�X�g�쐬�Ɏ��s���܂����B",
				L"m_d2dDevice->CreateDeviceContext()",
				L"DeviceResources::Impl::CreateDeviceResources()"
				);


			ThrowIfFailed(
				pD2D11DeviceContext->CreateBitmapFromDxgiSurface(
				dxgiBackBuffer.Get(),
				nullptr,	//�f�t�H���g�ݒ�
				&pImpl->m_d2dTargetBitmap
				),
				L"2d�r�b�g�}�b�v�쐬�Ɏ��s���܂����B",
				L"pD2D11DeviceContext->CreateBitmapFromDxgiSurface()",
				L"DefaultRenderTarget::DefaultRenderTarget()"
				);

			pD2D11DeviceContext->SetTarget(pImpl->m_d2dTargetBitmap.Get());
			//�O���[�X�P�[�� �e�L�X�g�̃A���`�G�C���A�V���O
			pD2D11DeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);

		}
		catch (...){
			throw;
		}
	}
	DefaultRenderTarget::~DefaultRenderTarget(){}


	//�A�N�Z�T
	ID3D11RenderTargetView* DefaultRenderTarget::GetRenderTargetView() const { return pImpl->m_D3D11RenderTargetView.Get(); }
	ID3D11Texture2D* DefaultRenderTarget::GetDepthStencil() const{ return pImpl->m_DepthStencil.Get(); }
	ID3D11DepthStencilView*	DefaultRenderTarget::GetDepthStencilView() const{ return pImpl->m_DepthStencilView.Get(); }
	ID2D1Bitmap1*			DefaultRenderTarget::GetD2DTargetBitmap() const	{ return pImpl->m_d2dTargetBitmap.Get(); }


	//����
	//�X�N���[���S�̂��w��̐F�ŃN���A����
	void DefaultRenderTarget::ClearViews(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�o�b�t�@�̃N���A
		float Color[4] = { 0, 0, 0,1.0f };
		D3D11_VIEWPORT ViewPort;
		//�r���[�|�[�g�̃Z�b�g�A�b�v
		ZeroMemory(&ViewPort, sizeof(ViewPort));
		ViewPort.Width = (float)App::GetApp()->GetGameWidth();
		ViewPort.Height = (float)App::GetApp()->GetGameHeight();
		ViewPort.MinDepth = 0.0f;
		ViewPort.MaxDepth = 1.0f;
		ViewPort.TopLeftX = 0;
		ViewPort.TopLeftY = 0;
		pD3D11DeviceContext->RSSetViewports(1, &ViewPort);
		//�����_�����O�^�[�Q�b�g�̃N���A
		pD3D11DeviceContext->ClearRenderTargetView(pImpl->m_D3D11RenderTargetView.Get(), Color);
		//�ʏ�̐[�x�o�b�t�@�ƃX�e���V���o�b�t�@�̃N���A
		pD3D11DeviceContext->ClearDepthStencilView(pImpl->m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	//�����_�����O�^�[�Q�b�g���J�n����
	void DefaultRenderTarget::StartRenderTarget(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		ID3D11RenderTargetView* pV = pImpl->m_D3D11RenderTargetView.Get();
		//�����_�����O�^�[�Q�b�g�ƃX�e���V����ݒ�
		pD3D11DeviceContext->OMSetRenderTargets(1, &pV, pImpl->m_DepthStencilView.Get());
		//�r���[�|�[�g�̐ݒ�
		if (pImpl->m_Stage.expired()){
			throw BaseException(
				L"�X�e�[�W�������ł�",
				L"if (pImpl->m_Stage.expired())",
				L"DefaultRenderTarget::StartRenderTarget()"
				);
		}
		auto StagePtr = pImpl->m_Stage.lock();

		pD3D11DeviceContext->RSSetViewports(1, StagePtr->GetTargetViewPortRealPtr());

		//�V�F�[�_�[���\�[�X�r���[�̃N���A
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
		pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
		//�V�F�[�_�[�͎w�肵�Ȃ�
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//�u�����h�͎w�肵�Ȃ�
		pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);

	}
	//�����_�����O�^�[�Q�b�g���I������
	void DefaultRenderTarget::EndRenderTarget(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11Device = Dev->GetD3DDevice();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�X�e�[�^�X�̃|�C���^
		//auto RenderStatePtr = App::GetApp()->GetRenderState();
		//�V�F�[�_�[���\�[�X�r���[�̃N���A
		ID3D11ShaderResourceView* pNull[1] = { nullptr };
		pD3D11DeviceContext->PSSetShaderResources(0, _countof(pNull), pNull);
		pD3D11DeviceContext->PSSetShaderResources(1, _countof(pNull), pNull);
		//�V�F�[�_�[�͎w�肵�Ȃ�
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		//�u�����h�͎w�肵�Ȃ�
		pD3D11DeviceContext->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	}




}
//end basedx11

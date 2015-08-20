#include "stdafx.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	struct DeviceResources::Impl;
	//	�p�r: Direct11�f�o�C�X�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct DeviceResources::Impl{
		D3D_DRIVER_TYPE	m_D3DDriverType;			//�h���C�o�̃^�C�v
		D3D_FEATURE_LEVEL m_D3DFeatureLevel;
		// Direct3D �I�u�W�F�N�g�B
		ComPtr<ID3D11Device2> m_D3D11Device;		//�f�o�C�X
		ComPtr<ID3D11DeviceContext2> m_D3D11Context;		//�R���e�L�X�g
		ComPtr<IDXGISwapChain1> m_D3D11SwapChain;	//�X���b�v�`�F�[��


		// Direct2D �`��R���|�[�l���g�B
		ComPtr<ID2D1Factory2>		m_d2dFactory;
		ComPtr<ID2D1Device1>		m_d2dDevice;
		ComPtr<ID2D1DeviceContext1>	m_d2dContext;
//		ComPtr<ID2D1Bitmap1>		m_d2dTargetBitmap;

		// DirectWrite �`��R���|�[�l���g�B
		ComPtr<IDWriteFactory2>		m_dwriteFactory;
		ComPtr<IWICImagingFactory2>	m_wicFactory;

		float m_dpi;


		//�\�z�Ɣj��
		Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
		~Impl(){}
		//���\�[�X�̍\�z
		void CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height);
	};
	//�\�z
	DeviceResources::Impl::Impl(HWND hWnd, bool isFullScreen, UINT Width, UINT Height):
		m_dpi(96.0f)
	{
		try{
			//���\�[�X�̍\�z
			CreateDeviceResources(hWnd, isFullScreen, Width, Height);
		}
		catch (...){
			throw;
		}
	}
	//���\�[�X�̍\�z
	void DeviceResources::Impl::CreateDeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height){

		// Direct2D ���\�[�X�����������܂��B
		D2D1_FACTORY_OPTIONS options;
		ZeroMemory(&options, sizeof(D2D1_FACTORY_OPTIONS));

#if defined(_DEBUG)
		// �v���W�F�N�g���f�o�b�O �r���h�Ɋ܂܂�Ă���ꍇ�́ADirect2D �f�o�b�O�� SDK ���C���[����ėL���ɂ��܂��B
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#endif

		// Direct2D �t�@�N�g�������������܂��B
		//DX::ThrowIfFailed(
		//	D2D1CreateFactory(
		//	D2D1_FACTORY_TYPE_SINGLE_THREADED,
		//	__uuidof(ID2D1Factory2),
		//	&options,
		//	&m_d2dFactory
		//	)
		//	);

		ThrowIfFailed(
			D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED,
			__uuidof(ID2D1Factory2),
			&options,
			&m_d2dFactory
			),
			L"Factory�쐬�Ɏ��s���܂����B",
			L"D2D1CreateFactory()",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);


		// DirectWrite �t�@�N�g�������������܂��B
		//DX::ThrowIfFailed(
		//	DWriteCreateFactory(
		//	DWRITE_FACTORY_TYPE_SHARED,
		//	__uuidof(IDWriteFactory2),
		//	&m_dwriteFactory
		//	)
		//	);

		ThrowIfFailed(
			DWriteCreateFactory(
			DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory2),
			&m_dwriteFactory
			),
			L"DirectWrite �t�@�N�g���쐬�Ɏ��s���܂����B",
			L"DWriteCreateFactory()",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);

		ThrowIfFailed(
			CoCreateInstance(
			CLSID_WICImagingFactory2,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&m_wicFactory)
			),
			L"(WIC) �t�@�N�g���쐬�Ɏ��s���܂����B",
			L"CoCreateInstance()",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);


		HRESULT hr;	//API�̖߂�l�p
		//�f�o�C�X�쐬�p�t���O
		UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		//�f�o�b�O��
#ifdef _DEBUG
//		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
		//�h���C�o�̃^�C�v��z��
		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		//�I���ł���h���C�o�[�̐�
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		//�@�\�Z�b�g�̔z��
		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,	//Dx11�@�\�I�����[
		};
		//�I���ł���@�\�̐�
		UINT numFeatureLevels = ARRAYSIZE(featureLevels);

		//�X���b�v�`�F�[���̃f�X�N�v���^�̐ݒ�
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 2;					//�o�b�t�@��(2�_�u���o�b�t�@)
		sd.BufferDesc.Width = Width;		//�o�b�t�@�̕�
		sd.BufferDesc.Height = Height;	//�o�b�t�@�̍���
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		//8 �r�b�g �A���t�@���T�|�[�g���� 4 �����A16 �r�b�g�����Ȃ����K�������t�H�[�}�b�g
		sd.BufferDesc.RefreshRate.Numerator = 60;	//���t���b�V�����[�g�ő�l
		sd.BufferDesc.RefreshRate.Denominator = 1;	//���t���b�V�����[�g�ŏ��l
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�o�͗p�Ƃ��Ē�`
		sd.OutputWindow = hWnd;		//�E�C���h�E�̃n���h��
		sd.SampleDesc.Count = 1;		//�}���`�T���v�����O ����1
		sd.SampleDesc.Quality = 0;		//�}���`�T���v�����O�N�I���e�B��0�i�ő�j
		//�ȉ��A�X�g�A�A�v���͐ݒ�B�f�X�N�g�b�v�͎w�肵�Ȃ��i�w�肷��ƃt���X�N���[���ɂȂ�Ȃ��j
//		sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;

		sd.Windowed = TRUE;			//�E�C���h�E���[�h�ō쐬���Ă��Ƃ���t��s�N���[���ɂ���

		ComPtr<ID3D11Device> temp_device;
		ComPtr<ID3D11DeviceContext>	temp_context;
		ComPtr<IDXGISwapChain>	temp_swapChain;


		//�f�o�C�X�ƃX���b�v�`�F�[���̍쐬
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++){
			//�h���C�o�^�C�v��z�񂩂�擾
			m_D3DDriverType = driverTypes[driverTypeIndex];
			//�f�o�C�X�ƃX���b�v�`�F�[���̍쐬
			hr = D3D11CreateDeviceAndSwapChain(
				nullptr,
				m_D3DDriverType,
				nullptr,
				createDeviceFlags,
				featureLevels,
				numFeatureLevels,
				D3D11_SDK_VERSION,
				&sd,
				&temp_swapChain,
				&temp_device,
				&m_D3DFeatureLevel,
				&temp_context
				);
			//���������炻�̃h���C�o���g��
			if (SUCCEEDED(hr))
				break;
		}
		ThrowIfFailed(
			hr, 
			L"DX11�f�o�C�X�ƃX���b�v�`�F�[���̍쐬�Ɏ��s���܂����B", 
			L"D3D11CreateDeviceAndSwapChain()", 
			L"DeviceResources::Impl::CreateDeviceResources()"
		);

		//���\�[�X���o�[�W�����A�b�v����
		ThrowIfFailed(
			temp_device.As(&m_D3D11Device),
			L"DX11�f�o�C�X�̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"temp_device.As(&m_D3D11Device)",
			L"DeviceResources::Impl::CreateDeviceResources()"
		);
		ThrowIfFailed(
			temp_context.As(&m_D3D11Context),
			L"DX11�R���e�N�X�g�̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"temp_context.As(&m_D3D11Context)",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);
		ThrowIfFailed(
			temp_swapChain.As(&m_D3D11SwapChain),
			L"DX11�X���b�v�`�F�[���̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"temp_swapChain.As(&m_D3D11SwapChain)",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);

		// Direct2D �f�o�C�X �I�u�W�F�N�g�ƁA�Ή�����R���e�L�X�g���쐬���܂��B

		ComPtr<IDXGIDevice3> dxgiDevice;

		ThrowIfFailed(
			m_D3D11Device.As(&dxgiDevice),
			L"dxgiDevice�̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"m_d3dDevice.As(&dxgiDevice)",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);

		ThrowIfFailed(
			m_d2dFactory->CreateDevice(dxgiDevice.Get(), &m_d2dDevice),
			L"dxgiDevice�̃o�[�W�����A�b�v�Ɏ��s���܂����B",
			L"m_d2dFactory->CreateDevice()",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);

		ThrowIfFailed(
			m_d2dDevice->CreateDeviceContext(
			D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
			&m_d2dContext
			),
			L"2d�f�o�C�X�R���e�L�X�g�쐬�Ɏ��s���܂����B",
			L"m_d2dDevice->CreateDeviceContext()",
			L"DeviceResources::Impl::CreateDeviceResources()"
			);
		if (isFullScreen){
			ThrowIfFailed(
				m_D3D11SwapChain->SetFullscreenState(true, NULL),
				L"�t���X�N���[���ڍs�Ɏ��s���܂����B",
				L"m_D3D11SwapChain->SetFullscreenState(true, NULL)",
				L"DeviceResources::Impl::CreateDeviceResources()"
				);

		}

	}

	//--------------------------------------------------------------------------------------
	//	class DeviceResources;
	//	�p�r: Direct11�f�o�C�X�ȂǃN���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	DeviceResources::DeviceResources(HWND hWnd, bool isFullScreen, UINT Width, UINT Height):
		pImpl(new Impl(hWnd,isFullScreen,Width,Height))
	{
	}
	DeviceResources::~DeviceResources(){}
	//�A�N�Z�T
	ID3D11Device2*	DeviceResources::GetD3DDevice() const { return pImpl->m_D3D11Device.Get(); }
	ID3D11DeviceContext2* DeviceResources::GetD3DDeviceContext() const { return pImpl->m_D3D11Context.Get(); }
	IDXGISwapChain1* DeviceResources::GetSwapChain() const { return pImpl->m_D3D11SwapChain.Get(); }
	D3D_FEATURE_LEVEL DeviceResources::GetFeatureLevel() const { return pImpl->m_D3DFeatureLevel; }

	// D2D �A�N�Z�T�[�B
	ID2D1Factory2*			DeviceResources::GetD2DFactory() const					{ return pImpl->m_d2dFactory.Get(); }
	ID2D1Device1*			DeviceResources::GetD2DDevice() const					{ return pImpl->m_d2dDevice.Get(); }
	ID2D1DeviceContext1*	DeviceResources::GetD2DDeviceContext() const				{ return pImpl->m_d2dContext.Get(); }
	IDWriteFactory2*		DeviceResources::GetDWriteFactory() const				{ return pImpl->m_dwriteFactory.Get(); }
	IWICImagingFactory2*	DeviceResources::GetWicImagingFactory() const			{ return pImpl->m_wicFactory.Get(); }

	void DeviceResources::InitializeStates(const shared_ptr<RenderState>& StatePtr){
		ID3D11ShaderResourceView* pNull[D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT] = { nullptr };
		ID3D11SamplerState* pNullSR[D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT] = { nullptr };
		ID3D11Buffer* pNullConstantBuffer[D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT] = { nullptr };

		auto pID3D11DeviceContext = GetD3DDeviceContext();
		//��n��
		//�R���X�^���g�o�b�t�@�N���A
		pID3D11DeviceContext->VSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->GSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		pID3D11DeviceContext->CSSetConstantBuffers(0, D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, pNullConstantBuffer);
		//�V�F�[�_�[���\�[�X���N���A
		pID3D11DeviceContext->PSSetShaderResources(0, D3D11_COMMONSHADER_INPUT_RESOURCE_SLOT_COUNT, pNull);
		//�T���v���[���N���A
		pID3D11DeviceContext->PSSetSamplers(0, D3D11_COMMONSHADER_SAMPLER_SLOT_COUNT, pNullSR);
		//�V�F�[�_�̃N���A
		pID3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
		pID3D11DeviceContext->CSSetShader(nullptr, nullptr, 0);
		//�C���v�b�g���C�A�E�g�̃N���A
		pID3D11DeviceContext->IASetInputLayout(nullptr);
		//�f�t�H���g�̐ݒ�
		//���j�A�T���v���[��ݒ肵�Ă���
		ID3D11SamplerState* samplerState = StatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		//�u�����h�h��Ԃ�
		pID3D11DeviceContext->OMSetBlendState(StatePtr->GetOpaque(), nullptr, 0xffffffff);
		//�����_�[�X�e�[�g�O�ʂ̂ݕ`��
		pID3D11DeviceContext->RSSetState(StatePtr->GetCullBack());
		//�f�v�X�X�e���V���g�p
		pID3D11DeviceContext->OMSetDepthStencilState(StatePtr->GetDepthDefault(), 0);

	}

}



//end basedx11

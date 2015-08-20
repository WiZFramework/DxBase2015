#include "stdafx.h"


namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct ShaderResource::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct ShaderResource::Impl{
		Impl(){}
		~Impl(){}
		//�~���[�e�b�N�X
		std::mutex Mutex;
		//�R���X�^���g�o�b�t�@�A�N�Z�b�T
		ID3D11Buffer* GetConstantBufferBase(UINT BuffSize, ComPtr<ID3D11Buffer>& Buffer){
			return Util::DemandCreate(Buffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				ShaderResource::CreateConstantBuffer(BuffSize, pResult);
			});
		}
		//���_�V�F�[�_�A�N�Z�b�T
		ID3D11VertexShader* GetVertexShaderBase(const wstring& Filename, ComPtr<ID3D11VertexShader>& VSPtr){
			//�����_�����p
			return Util::DemandCreate(VSPtr, Mutex, [&](ID3D11VertexShader** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//�V�F�[�_�̓ǂݍ���
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateVertexShader(data, cso_sz, pResult);
			});
		}
		//�C���v�b�g���C�A�E�g�A�N�Z�b�T
		ID3D11InputLayout* GetInputLayoutBase2(const wstring& Filename, ComPtr<ID3D11InputLayout>& ILPtr,
			const D3D11_INPUT_ELEMENT_DESC* pElem, UINT NumElements){
			return Util::DemandCreate(ILPtr, Mutex, [&](ID3D11InputLayout** pResult)
			{
				std::unique_ptr<uint8_t[]> data;
				//�V�F�[�_�̓ǂݍ���
				size_t cso_sz = 0;
				ShaderResource::ReadBinaryFile(Filename, data, cso_sz);
				ShaderResource::CreateInputLayout(
					data,
					cso_sz,
					pElem,
					NumElements,
					pResult);
			});
		}


	};



	ShaderResource::ShaderResource() :pImpl(new Impl()){}
	ShaderResource::~ShaderResource(){}


	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::ReadBinaryFile(
	//		const wstring& fileName,		//�t�@�C����
	//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^�̖߂�
	//		size_t& CsoSz					//�T�C�Y�̖߂�
	//	);
	//	�p�r: �o�C�i���t�@�C����ǂݍ���
	//--------------------------------------------------------------------------------------
	void ShaderResource::ReadBinaryFile(const wstring& fileName, unique_ptr<uint8_t[]>& Data, size_t& CsoSz){
		try{
			if (fileName == L""){
				throw BaseException(
					L"�t�@�C�����w�肳��Ă��܂���",
					L"if(fileName == L\"\")",
					L"ShaderResource::ReadBinaryFile()"
					);
			}
			DWORD RetCode;
			RetCode = GetFileAttributes(fileName.c_str());
			if (RetCode == -1){
				throw BaseException(
					L"�t�@�C�������݂��܂���",
					fileName,
					L"ShaderResource::ReadBinaryFile()"
					);
			}

			ThrowIfFailed(
				BinaryReader::ReadEntireFile(fileName, Data, &CsoSz),
				L"�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B",
				fileName,
				L"ShaderResource::ReadBinaryFile()"
				);

		}
		catch (...){
			throw;
		}
	}


	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateVertexShader(
	//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^
	//		size_t CsoSz,					//�T�C�Y
	//		ID3D11VertexShader** pResult			//�󂯎��V�F�[�_
	//	);
	//	�p�r: �o�C�i���f�[�^���璸�_�V�F�[�_���쐬����
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateVertexShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11VertexShader** pResult){
		try{
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//���_�V�F�[�_�[�̍쐬
			ThrowIfFailed(
				pDx11Device->CreateVertexShader(Data.get(),CsoSz,nullptr,pResult),
				L"���_�V�F�[�_�̍쐬�Ɏ��s���܂����B",
				L"if( FAILED( pDx11Device->CreateVertexShader() ) )",
				L"ShaderResource::CreateVertexShader()"
				);

		}
		catch (...){
			throw;
		}
	}
	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateInputLayout(
	//		unique_ptr<uint8_t[]>& Data,	//���_�V�F�[�_�̃o�C�i���f�[�^
	//		size_t CsoSz,					//�T�C�Y
	//		const D3D11_INPUT_ELEMENT_DESC* pElement,	//���_��`
	//		UINT NumElement,							//���_��`�̐�
	//		ID3D11InputLayout** pResult			//�󂯎�郌�C�A�E�g
	//	);
	//	�p�r: �o�C�i���f�[�^����C���v�b�g���C�A�E�g���쐬����
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateInputLayout(unique_ptr<uint8_t[]>& Data, size_t CsoSz,
		const D3D11_INPUT_ELEMENT_DESC* pElement, UINT NumElement, ID3D11InputLayout** pResult){
		try{
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//�C���v�b�g���C�A�E�g�̍쐬
			HRESULT hr = pDx11Device->CreateInputLayout(
				pElement,	//���_��`
				NumElement,		//���_��`�̐�
				Data.get(),
				CsoSz,
				pResult
				);
			//���s������
			if (FAILED(hr))
			{
				throw basedx11::BaseException(
					L"���_�V�F�[�_�̃C���v�b�g���C�A�E�g�̍쐬�Ɏ��s���܂����B",
					L"if( FAILED( pDx11Device->CreateInputLayout() ) )",
					L"ShaderResource::CreateInputLayout()"
					);
			}
		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreatePixelShader(
	//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^
	//		size_t CsoSz,					//�T�C�Y
	//		ID3D11PixelShader** pResult			//�󂯎��V�F�[�_
	//	);
	//	�p�r: �o�C�i���f�[�^����s�N�Z���V�F�[�_���쐬����
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreatePixelShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11PixelShader** pResult){
		try{
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//�s�N�Z���V�F�[�_�쐬
			HRESULT hr = pDx11Device->CreatePixelShader(
				Data.get(),
				CsoSz,
				nullptr,
				pResult);
			if (FAILED(hr)){
				throw basedx11::BaseException(
					L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s���܂����B",
					L"f( FAILED( pDx11Device->CreatePixelShader() ) )",
					L"ShaderResource::CreatePixelShader()"
					);
			}
		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateGeometryShader(
	//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^
	//		size_t CsoSz,					//�T�C�Y
	//		ID3D11GeometryShader** pResult			//�󂯎��V�F�[�_
	//	);
	//	�p�r: �o�C�i���f�[�^����W�I���g���V�F�[�_���쐬����
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11GeometryShader** pResult){
		try{
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//�W�I���g���V�F�[�_�쐬
			HRESULT hr = pDx11Device->CreateGeometryShader(
				Data.get(),
				CsoSz,
				nullptr,
				pResult);
			if (FAILED(hr)){
				throw basedx11::BaseException(
					L"�W�I���g���V�F�[�_�̍쐬�Ɏ��s���܂����B",
					L"f( FAILED( pDx11Device->CreateGeometryShader() ) )",
					L"ShaderResource::CreateGeometryShader()"
					);
			}
		}
		catch (...){
			throw;
		}
	}

	void ShaderResource::CreateGeometryShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz,
		const vector<D3D11_SO_DECLARATION_ENTRY>& SOEntries, UINT Stride, UINT NumStride, ID3D11GeometryShader** pResult){
		try{
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			auto pDx11Device = Dev->GetD3DDevice();
			//�W�I���g���V�F�[�_�쐬
			UINT StridBuff[1] = { Stride };
			HRESULT hr = pDx11Device->CreateGeometryShaderWithStreamOutput(
				Data.get(),
				CsoSz,
				SOEntries.data(),
				SOEntries.size(),
				StridBuff,
				NumStride,
				0,
				nullptr,
				pResult);
			if (FAILED(hr)){
				throw basedx11::BaseException(
					L"�W�I���g���V�F�[�_�̍쐬�Ɏ��s���܂����B",
					L"f( FAILED( pDx11Device->CreateGeometryShader() ) )",
					L"ShaderResource::CreateGeometryShader()"
					);
			}
		}
		catch (...){
			throw;
		}
	}

	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateComputeShader(
	//		unique_ptr<uint8_t[]>& Data,	//�o�C�i���f�[�^
	//		size_t CsoSz,					//�T�C�Y
	//		ID3D11ComputeShader** pResult			//�󂯎��V�F�[�_
	//	);
	//	�p�r: �o�C�i���f�[�^����R���s���[�g�V�F�[�_���쐬����
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateComputeShader(unique_ptr<uint8_t[]>& Data, size_t CsoSz, ID3D11ComputeShader** pResult){
		try{
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//�s�N�Z���V�F�[�_�쐬
			HRESULT hr = pDx11Device->CreateComputeShader(
				Data.get(),
				CsoSz,
				nullptr,
				pResult);
			if (FAILED(hr)){
				throw basedx11::BaseException(
					L"�R���s���[�g�V�F�[�_�̍쐬�Ɏ��s���܂����B",
					L"f( FAILED( pDx11Device->CreateComputeShader() ) )",
					L"ShaderResource::CreateComputeShader()"
					);
			}
		}
		catch (...){
			throw;
		}
	}






	//--------------------------------------------------------------------------------------
	//	static void ShaderResource::CreateConstantBuffer(
	//		UINT BuffSize,					//�T�C�Y
	//		ID3D11Buffer** pResult			//�󂯎��V�F�[�_
	//	);
	//	�p�r: �R���X�^���g�o�b�t�@���쐬����
	//--------------------------------------------------------------------------------------
	void ShaderResource::CreateConstantBuffer(UINT BuffSize, ID3D11Buffer** pResult){
		try{
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//�R���X�^���g�o�b�t�@�̍쐬
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = BuffSize;
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			HRESULT hr = pDx11Device->CreateBuffer(&bd, NULL, pResult);
			if (FAILED(hr)){
				// ���������s
				throw BaseException(
					L"�R���X�^���g�o�b�t�@�쐬�Ɏ��s���܂����B",
					L"if( FAILED( hr ) )",
					L"ShaderResource::CreateConstantBuffer()"
					);
			}
		}
		catch (...){
			throw;
		}
	}


	//�R���X�^���g�o�b�t�@�A�N�Z�b�T
	ID3D11Buffer* ShaderResource::GetConstantBufferBase(UINT BuffSize, ComPtr<ID3D11Buffer>& Buffer){
		return pImpl->GetConstantBufferBase(BuffSize, Buffer);
	}
	//���_�V�F�[�_�A�N�Z�b�T
	ID3D11VertexShader* ShaderResource::GetVertexShaderBase(const wstring& Filename, ComPtr<ID3D11VertexShader>& VSPtr){
		return pImpl->GetVertexShaderBase(Filename,VSPtr);
	}
	//�C���v�b�g���C�A�E�g�A�N�Z�b�T
	ID3D11InputLayout* ShaderResource::GetInputLayoutBase2(const wstring& Filename, ComPtr<ID3D11InputLayout>& ILPtr,
		const D3D11_INPUT_ELEMENT_DESC* pElem, UINT NumElements){
		return pImpl->GetInputLayoutBase2(Filename, ILPtr,pElem, NumElements);
	}


}
//endof  basedx11

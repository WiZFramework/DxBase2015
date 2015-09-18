#include "stdafx.h"


namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	struct TextureResource::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct TextureResource::Impl{
		ComPtr<ID3D11ShaderResourceView> m_ShaderResView;	//���\�[�X�r���[
		wstring m_FileName;		//�e�N�X�`���t�@�C���ւ̃p�X

		Impl(const wstring& FileName, const wstring& TexType = L"WIC");
		~Impl(){}
		//�~���[�e�b�N�X
		std::mutex Mutex;

	};

	TextureResource::Impl::Impl(const wstring& FileName, const wstring& TexType) :
		m_FileName(FileName){
		try{
			if (FileName == L""){
				throw BaseException(
					L"�t�@�C�����w�肳��Ă��܂���",
					L"if(FileName == L\"\")",
					L"Texture::Impl::Impl()"
					);
			}
			DWORD RetCode;
			RetCode = GetFileAttributes(m_FileName.c_str());
			if (RetCode == -1){
				wstring patherr = m_FileName;
				throw BaseException(
					L"�t�@�C�������݂��܂���",
					patherr.c_str(),
					L"Texture::Impl::Impl()"
					);
			}
			HRESULT hr;
			//�e�N�X�`���쐬
			DirectX::TexMetadata metadata;
			DirectX::ScratchImage image;

			if (TexType == L"WIC"){
				//�t�@�C���g���q�̒���
				wchar_t Drivebuff[_MAX_DRIVE];
				wchar_t Dirbuff[_MAX_DIR];
				wchar_t FileNamebuff[_MAX_FNAME];
				wchar_t Extbuff[_MAX_EXT];

				::ZeroMemory(Drivebuff, sizeof(Drivebuff));
				::ZeroMemory(Dirbuff, sizeof(Dirbuff));
				::ZeroMemory(FileNamebuff, sizeof(FileNamebuff));
				::ZeroMemory(Extbuff, sizeof(Extbuff));

				_wsplitpath_s(m_FileName.c_str(),
					Drivebuff, _MAX_DRIVE,
					Dirbuff, _MAX_DIR,
					FileNamebuff, _MAX_FNAME,
					Extbuff, _MAX_EXT);

				wstring ExtStr = Extbuff;

				if (ExtStr == L".dds" || ExtStr == L".DDS"){
					ThrowIfFailed(
						DirectX::LoadFromDDSFile(m_FileName.c_str(), 0, &metadata, image),
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						m_FileName,
						L"Texture::Impl::Impl()"
						);
				}
				else if (ExtStr == L".tga" || ExtStr == L".TGA"){
					ThrowIfFailed(
						DirectX::LoadFromTGAFile(m_FileName.c_str(), &metadata, image),
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						m_FileName,
						L"Texture::Impl::Impl()"
						);
				}
				else{
					ThrowIfFailed(
						DirectX::LoadFromWICFile(m_FileName.c_str(), 0, &metadata, image),
						L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
						m_FileName,
						L"Texture::Impl::Impl()"
						);
				}
			}
			else if (TexType == L"DDS"){
				ThrowIfFailed(
					DirectX::LoadFromDDSFile(m_FileName.c_str(), 0, &metadata, image),
					L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
					m_FileName,
					L"Texture::Impl::Impl()"
					);
			}
			else if (TexType == L"TGA"){
				ThrowIfFailed(
					DirectX::LoadFromTGAFile(m_FileName.c_str(), &metadata, image),
					L"�e�N�X�`���̓ǂݍ��݂Ɏ��s���܂���",
					m_FileName,
					L"Texture::Impl::Impl()"
					);
			}
			else{
				throw BaseException(
					L"���̃^�C�v�͑Ή����Ă��܂���",
					TexType.c_str(),
					L"Texture::Impl::Impl()"
					);
			}

			//�f�o�C�X�ƃR���e�L�X�g�C���^�[�t�F�C�X�̎擾
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			ID3D11DeviceContext* pID3D11DeviceContex = Dev->GetD3DDeviceContext();

			Util::DemandCreate(m_ShaderResView, Mutex, [&](ID3D11ShaderResourceView** pResult) -> HRESULT
			{
				// �摜����V�F�[�_���\�[�XView�̍쐬
				hr = ThrowIfFailed(CreateShaderResourceView(pDx11Device, image.GetImages(), image.GetImageCount(), metadata, pResult),
					L"�V�F�[�_�[���\�[�X�r���[���쐬�ł��܂���",
					L"if( FAILED( CreateShaderResourceView() ) )",
					L"Texture::Impl::Impl()"
					);
				return hr;
			});


		}
		catch (...){
			throw;
		}
	}



	//--------------------------------------------------------------------------------------
	//	class TextureResource : public BaseResource;
	//	�p�r: �e�N�X�`���̃��b�s���O�N���X
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//	TextureResource(
	//	const wstring& FileName	//�t�@�C����
	//	);
	//	�p�r: �R���X�g���N�^
	//	�߂�l: �Ȃ�
	//--------------------------------------------------------------------------------------
	TextureResource::TextureResource(const wstring& FileName, const wstring& TexType) :
		BaseResource(),
		pImpl(new Impl(FileName, TexType))
	{
	}
	TextureResource::~TextureResource(){
	}

	//�A�N�Z�b�T
	ComPtr<ID3D11ShaderResourceView>& TextureResource::GetShaderResourceView() const{
		return pImpl->m_ShaderResView;
	}
	const wstring& TextureResource::GetTextureFileName() const{
		return pImpl->m_FileName;
	}


	//--------------------------------------------------------------------------------------
	//	struct AudioResource::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct AudioResource::Impl{
		WAVEFORMATEX m_WaveFormat;	//�E�F�u�t�H�[�}�b�g
		vector<byte> m_SoundData;	//�f�[�^
		wstring m_FileName;		//�t�@�C���ւ̃p�X
		Impl(const wstring& FileName) :
			m_FileName(FileName){}
		~Impl(){}
		//�~���[�e�b�N�X
		std::mutex Mutex;

	};


	//--------------------------------------------------------------------------------------
	//	class AudioResource : public BaseResource;
	//	�p�r: �I�[�f�B�I���\�[�X�iwav�Ȃǁj�̃��b�s���O�N���X
	//--------------------------------------------------------------------------------------
	AudioResource::AudioResource(const wstring& FileName):
		BaseResource(),
		pImpl(new Impl(FileName))
	{
		try{

			ThrowIfFailed(
				MFStartup(MF_VERSION),
				L"MediaFoundation�̏������Ɏ��s���܂���",
				L"MFStartup(MF_VERSION)",
				L"AudioResource::AudioResource()"
				);

			ComPtr<IMFSourceReader> reader;

			ThrowIfFailed(
				MFCreateSourceReaderFromURL(FileName.c_str(),nullptr,&reader),
				L"�T�E���h���[�_�[�̍쐬�Ɏ��s���܂���",
				L"MFCreateSourceReaderFromURL(FileName.c_str(),nullptr,&reader)",
				L"AudioResource::AudioResource()"
				);

			// Set the decoded output format as PCM.
			// XAudio2 on Windows can process PCM and ADPCM-encoded buffers.
			// When using MediaFoundation, this sample always decodes into PCM.
			Microsoft::WRL::ComPtr<IMFMediaType> mediaType;

			ThrowIfFailed(
				MFCreateMediaType(&mediaType),
				L"���f�B�A�^�C�v�̍쐬�Ɏ��s���܂���",
				L"MFCreateMediaType(&mediaType)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio),
				L"���f�B�AGUID�̐ݒ�Ɏ��s���܂���",
				L"mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM),
				L"���f�B�A�T�uGUID�̐ݒ�Ɏ��s���܂���",
				L"mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get()),
				L"���[�_�[�ւ̃��f�B�A�^�C�v�̐ݒ�Ɏ��s���܂���",
				L"reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get())",
				L"AudioResource::AudioResource()"
				);


			// Get the complete WAVEFORMAT from the Media Type.
			Microsoft::WRL::ComPtr<IMFMediaType> outputMediaType;

			ThrowIfFailed(
				reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType),
				L"�o�͗p�̃��f�B�A�^�C�v�̐ݒ�Ɏ��s���܂���",
				L"reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType)",
				L"AudioResource::AudioResource()"
				);

			UINT32 size = 0;
			WAVEFORMATEX* waveFormat;

			ThrowIfFailed(
				MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size),
				L"�E�F�u�t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���",
				L"MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size)",
				L"AudioResource::AudioResource()"
				);


			CopyMemory(&pImpl->m_WaveFormat, waveFormat, sizeof(pImpl->m_WaveFormat));
			CoTaskMemFree(waveFormat);

			PROPVARIANT propVariant;

			ThrowIfFailed(
				reader->GetPresentationAttribute(static_cast<uint32>(MF_SOURCE_READER_MEDIASOURCE), MF_PD_DURATION, &propVariant),
				L"�A�g���r���[�g�̐ݒ�Ɏ��s���܂���",
				L"reader->GetPresentationAttribute(static_cast<uint32>(MF_SOURCE_READER_MEDIASOURCE), MF_PD_DURATION, &propVariant)",
				L"AudioResource::AudioResource()"
				);

			// 'duration' is in 100ns units; convert to seconds, and round up
			// to the nearest whole byte.
			LONGLONG duration = propVariant.uhVal.QuadPart;
			unsigned int maxStreamLengthInBytes =
				static_cast<unsigned int>(
				((duration * static_cast<ULONGLONG>(pImpl->m_WaveFormat.nAvgBytesPerSec)) + 10000000) /
				10000000
				);

			pImpl->m_SoundData.resize(maxStreamLengthInBytes);

			ComPtr<IMFSample> sample;
			ComPtr<IMFMediaBuffer> mediaBuffer;
			DWORD flags = 0;

			int positionInData = 0;
			bool done = false;
			while (!done)
			{

				ThrowIfFailed(
					reader->ReadSample(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &flags, nullptr, &sample),
					L"�T���v���[�̓ǂݍ��݂Ɏ��s���܂���",
					L"reader->ReadSample(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &flags, nullptr, &sample)",
					L"AudioResource::AudioResource()"
					);

				if (sample != nullptr)
				{
					ThrowIfFailed(
						sample->ConvertToContiguousBuffer(&mediaBuffer),
						L"�T���v���[�̃R���o�[�g�Ɏ��s���܂���",
						L"sample->ConvertToContiguousBuffer(&mediaBuffer)",
						L"AudioResource::AudioResource()"
						);

					BYTE *audioData = nullptr;
					DWORD sampleBufferLength = 0;

					ThrowIfFailed(
						mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength),
						L"�o�b�t�@��Lock�Ɏ��s���܂���",
						L"mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength)",
						L"AudioResource::AudioResource()"
						);

					for (DWORD i = 0; i < sampleBufferLength; i++)
					{
						pImpl->m_SoundData[positionInData++] = audioData[i];
					}
				}
				if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
				{
					done = true;
				}
			}
		}
		catch (...){
			throw;
		}
	}
	AudioResource::~AudioResource(){}

	const vector<byte>& AudioResource::GetSoundData()const{
		return pImpl->m_SoundData;
	}

	WAVEFORMATEX*  AudioResource::GetOutputWaveFormatEx()const{
		return &pImpl->m_WaveFormat;
	}



	//--------------------------------------------------------------------------------------
	//	struct MeshResource::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct MeshResource::Impl{
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//���_�o�b�t�@
		ComPtr<ID3D11Buffer> m_IndexBuffer;	//�C���f�b�N�X�o�b�t�@
		UINT m_NumVertices;				//���_�̐�
		UINT m_NumIndicis;				//�C���f�b�N�X�̐�

	};

	//--------------------------------------------------------------------------------------
	//	class MeshResource : public BaseResource;
	//	�p�r: �v���~�e�B�u���b�V���N���X
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//	MeshResource();
	//	�p�r: �R���X�g���N�^
	//	�߂�l: �Ȃ�
	//--------------------------------------------------------------------------------------
	MeshResource::MeshResource():
		BaseResource(),
		pImpl(new Impl())
	{}
	MeshResource::~MeshResource(){}

	void MeshResource::SetVertexBuffer(ComPtr<ID3D11Buffer>& VertexBuffer){
		pImpl->m_VertexBuffer = VertexBuffer;
	}
	void MeshResource::SetIndexBuffer(ComPtr<ID3D11Buffer>& IndexBuffer){
		pImpl->m_IndexBuffer = IndexBuffer;
	}
	void MeshResource::SetNumVertices(UINT NumVertices){
		pImpl->m_NumVertices = NumVertices;
	}
	void MeshResource::SetNumIndicis(UINT NumIndicis){
		pImpl->m_NumIndicis = NumIndicis;
	}

	//�A�N�Z�b�T
	ComPtr<ID3D11Buffer>& MeshResource::GetVertexBuffer() const{
		return pImpl->m_VertexBuffer;
	}
	ComPtr<ID3D11Buffer>& MeshResource::GetIndexBuffer() const{
		return pImpl->m_IndexBuffer;

	}
	UINT MeshResource::GetNumVertices() const{
		return pImpl->m_NumVertices;
	}
	UINT MeshResource::GetNumIndicis() const{
		return pImpl->m_NumIndicis;
	}

	struct VertexParams{
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//���_�o�b�t�@
		ComPtr<ID3D11Buffer> m_IndexBuffer;	//�C���f�b�N�X�o�b�t�@
		UINT m_NumVertices;				//���_�̐�
		UINT m_NumIndicis;				//�C���f�b�N�X�̐�
	};

	//--------------------------------------------------------------------------------------
	//	struct CommonMeshResource::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct CommonMeshResource::Impl{
		vector<VertexPositionNormalTexture> m_BackupVertices;
		//�~���[�e�b�N�X
		std::mutex Mutex;
		//--------------------------------------------------------------------------------------
		//	void CreateBuffers(
		//	vector<VertexPositionNormalTexture>& vertices, //���_���쐬���邽�߂̔z��
		//	vector<uint16_t>& indices,	//�C���f�b�N�X���쐬���邽�߂̔z��
		//	bool AccessWrite		//��������OK���ǂ���
		//	VertexParams& RetParam
		//	);
		//	�p�r: �o�b�t�@���쐬����
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void CreateBuffers(const vector<VertexPositionNormalTexture>& vertices,const vector<uint16_t>& indices, bool AccessWrite, VertexParams& RetParam);
	};

	void CommonMeshResource::Impl::CreateBuffers(const vector<VertexPositionNormalTexture>& vertices,const vector<uint16_t>& indices, bool AccessWrite,
		VertexParams& RetParam){
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		//�o�b�t�@�̍쐬
		if (AccessWrite){
			Util::DemandCreate(RetParam.m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				//���_�o�b�t�@�̍쐬
				VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, vertices, pResult);
			});
		}
		else{
			Util::DemandCreate(RetParam.m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				//���_�o�b�t�@�̍쐬
				VertexUtil::CreatePrimitiveBuffer(pDx11Device, vertices, D3D11_BIND_VERTEX_BUFFER, pResult);
			});
		}
		//���_���̐ݒ�
		RetParam.m_NumVertices = static_cast<UINT>(vertices.size());
		Util::DemandCreate(RetParam.m_IndexBuffer, Mutex, [&](ID3D11Buffer** pResult)
		{
			//�C���f�b�N�X�o�b�t�@�̍쐬
			VertexUtil::CreatePrimitiveBuffer(pDx11Device, indices, D3D11_BIND_INDEX_BUFFER, pResult);
		});
		//�C���f�b�N�X���̐ݒ�
		RetParam.m_NumIndicis = static_cast<UINT>(indices.size());
	}





	//--------------------------------------------------------------------------------------
	//	class CommonMeshResource : public MeshResource;
	//	�p�r: �v���~�e�B�u���b�V���N���X
	//--------------------------------------------------------------------------------------
	//�\�z
	CommonMeshResource::CommonMeshResource():
		MeshResource(),
		pImpl(new Impl())
	{}
	//�j��
	CommonMeshResource::~CommonMeshResource(){}

	//���\�[�X�\�z
	shared_ptr<CommonMeshResource> CommonMeshResource::CreateCommonMeshResource(
		const vector<VertexPositionNormalTexture>& Vertices, 
		const vector<uint16_t>& Indices, 
		bool AccessWrite){
		try{
			shared_ptr<CommonMeshResource> Ptr(new CommonMeshResource());

			for (auto Virtex : Vertices){
				Ptr->pImpl->m_BackupVertices.push_back(Virtex);
			}

			VertexParams Params;

			//�z������Ƃɒ��_�ƃC���f�b�N�X���쐬
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, Indices, AccessWrite, Params);
			//�������ݕs�̏ꍇ�̓o�b�N�A�b�v���폜
			if (!AccessWrite){
				Ptr->pImpl->m_BackupVertices.clear();
			}
			Ptr->SetVertexBuffer(Params.m_VertexBuffer);
			Ptr->SetIndexBuffer(Params.m_IndexBuffer);
			Ptr->SetNumVertices(Params.m_NumVertices);
			Ptr->SetNumIndicis(Params.m_NumIndicis);

			return Ptr;
		}
		catch (...){
			throw;
		}
	}


	shared_ptr<CommonMeshResource> CommonMeshResource::CreateSquare(float Size, bool AccessWrite){
		try{
			shared_ptr<CommonMeshResource> Ptr(new CommonMeshResource());

			float HelfSize = Size / 2.0f;
			Ptr->pImpl->m_BackupVertices.push_back(VertexPositionNormalTexture(XMFLOAT3(-HelfSize, HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)));
			Ptr->pImpl->m_BackupVertices.push_back(VertexPositionNormalTexture(XMFLOAT3(HelfSize, HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)));
			Ptr->pImpl->m_BackupVertices.push_back(VertexPositionNormalTexture(XMFLOAT3(-HelfSize, -HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f)));
			Ptr->pImpl->m_BackupVertices.push_back(VertexPositionNormalTexture(XMFLOAT3(HelfSize, -HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)));

			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices{
				(uint16_t)0,
				(uint16_t)1,
				(uint16_t)2,
				(uint16_t)1,
				(uint16_t)3,
				(uint16_t)2,
			};

			VertexParams Params;

			//�z������Ƃɒ��_�ƃC���f�b�N�X���쐬
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//�������ݕs�̏ꍇ�̓o�b�N�A�b�v���폜
			if (!AccessWrite){
				Ptr->pImpl->m_BackupVertices.clear();
			}
			Ptr->SetVertexBuffer(Params.m_VertexBuffer);
			Ptr->SetIndexBuffer(Params.m_IndexBuffer);
			Ptr->SetNumVertices(Params.m_NumVertices);
			Ptr->SetNumIndicis(Params.m_NumIndicis);

			return Ptr;

		}
		catch (...){
			throw;
		}
	}
	shared_ptr<CommonMeshResource> CommonMeshResource::CreateCube(float Size, bool AccessWrite){
		try{
			shared_ptr<CommonMeshResource> Ptr(new CommonMeshResource());

			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Cube�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateCube(Size, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//�o�b�t�@���쐬
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//�������ݕs�̏ꍇ�̓o�b�N�A�b�v���폜
			if (!AccessWrite){
				Ptr->pImpl->m_BackupVertices.clear();
			}
			Ptr->SetVertexBuffer(Params.m_VertexBuffer);
			Ptr->SetIndexBuffer(Params.m_IndexBuffer);
			Ptr->SetNumVertices(Params.m_NumVertices);
			Ptr->SetNumIndicis(Params.m_NumIndicis);

			return Ptr;

		}
		catch (...){
			throw;
		}
	}
	shared_ptr<CommonMeshResource> CommonMeshResource::CreateSphere(float Diameter, size_t Tessellation, bool AccessWrite){
		try{
			shared_ptr<CommonMeshResource> Ptr(new CommonMeshResource());

			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Sphere�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateSphere(Diameter, Tessellation, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//�o�b�t�@���쐬
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//�������ݕs�̏ꍇ�̓o�b�N�A�b�v���폜
			if (!AccessWrite){
				Ptr->pImpl->m_BackupVertices.clear();
			}
			Ptr->SetVertexBuffer(Params.m_VertexBuffer);
			Ptr->SetIndexBuffer(Params.m_IndexBuffer);
			Ptr->SetNumVertices(Params.m_NumVertices);
			Ptr->SetNumIndicis(Params.m_NumIndicis);

			return Ptr;
		}
		catch (...){
			throw;
		}
	}

	shared_ptr<CommonMeshResource> CommonMeshResource::CreateCapsule(float Diameter, float Height, size_t Tessellation, bool AccessWrite){
		try{
			shared_ptr<CommonMeshResource> Ptr(new CommonMeshResource());

			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;

			Vector3 PointA(0, -Height / 2.0f, 0);
			Vector3 PointB(0, Height / 2.0f, 0);
			//Capsule�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateCapsule(Diameter, PointA, PointB, Tessellation, Ptr->pImpl->m_BackupVertices, indices);

			VertexParams Params;
			//�o�b�t�@���쐬
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//�������ݕs�̏ꍇ�̓o�b�N�A�b�v���폜
			if (!AccessWrite){
				Ptr->pImpl->m_BackupVertices.clear();
			}
			Ptr->SetVertexBuffer(Params.m_VertexBuffer);
			Ptr->SetIndexBuffer(Params.m_IndexBuffer);
			Ptr->SetNumVertices(Params.m_NumVertices);
			Ptr->SetNumIndicis(Params.m_NumIndicis);

			return Ptr;
		}
		catch (...){
			throw;
		}


	}



	shared_ptr<CommonMeshResource> CommonMeshResource::CreateCylinder(float Height, float Diameter, size_t Tessellation, bool AccessWrite){
		try{
			shared_ptr<CommonMeshResource> Ptr(new CommonMeshResource());

			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Cylinder�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateCylinder(Height,Diameter,Tessellation, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//�o�b�t�@���쐬
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//�������ݕs�̏ꍇ�̓o�b�N�A�b�v���폜
			if (!AccessWrite){
				Ptr->pImpl->m_BackupVertices.clear();
			}
			Ptr->SetVertexBuffer(Params.m_VertexBuffer);
			Ptr->SetIndexBuffer(Params.m_IndexBuffer);
			Ptr->SetNumVertices(Params.m_NumVertices);
			Ptr->SetNumIndicis(Params.m_NumIndicis);

			return Ptr;
		}
		catch (...){
			throw;
		}
	}

	shared_ptr<CommonMeshResource> CommonMeshResource::CreateCone(float Diameter, float Height, size_t Tessellation, bool AccessWrite){
		try{
			shared_ptr<CommonMeshResource> Ptr(new CommonMeshResource());

			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Cone�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateCone(Diameter, Height, Tessellation, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//�o�b�t�@���쐬
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//�������ݕs�̏ꍇ�̓o�b�N�A�b�v���폜
			if (!AccessWrite){
				Ptr->pImpl->m_BackupVertices.clear();
			}
			Ptr->SetVertexBuffer(Params.m_VertexBuffer);
			Ptr->SetIndexBuffer(Params.m_IndexBuffer);
			Ptr->SetNumVertices(Params.m_NumVertices);
			Ptr->SetNumIndicis(Params.m_NumIndicis);

			return Ptr;
		}
		catch (...){
			throw;
		}
	}

	shared_ptr<CommonMeshResource> CommonMeshResource::CreateTorus(float Diameter, float Thickness, size_t Tessellation, bool AccessWrite){
		try{
			shared_ptr<CommonMeshResource> Ptr(new CommonMeshResource());

			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Torus�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateTorus(Diameter, Thickness, Tessellation, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//�o�b�t�@���쐬
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//�������ݕs�̏ꍇ�̓o�b�N�A�b�v���폜
			if (!AccessWrite){
				Ptr->pImpl->m_BackupVertices.clear();
			}
			Ptr->SetVertexBuffer(Params.m_VertexBuffer);
			Ptr->SetIndexBuffer(Params.m_IndexBuffer);
			Ptr->SetNumVertices(Params.m_NumVertices);
			Ptr->SetNumIndicis(Params.m_NumIndicis);

			return Ptr;
		}
		catch (...){
			throw;
		}

	}

	shared_ptr<CommonMeshResource> CommonMeshResource::CreateTetrahedron(float Size, bool AccessWrite){
		try{
			shared_ptr<CommonMeshResource> Ptr(new CommonMeshResource());

			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Tetrahedron�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateTetrahedron(Size, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//�o�b�t�@���쐬
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//�������ݕs�̏ꍇ�̓o�b�N�A�b�v���폜
			if (!AccessWrite){
				Ptr->pImpl->m_BackupVertices.clear();
			}
			Ptr->SetVertexBuffer(Params.m_VertexBuffer);
			Ptr->SetIndexBuffer(Params.m_IndexBuffer);
			Ptr->SetNumVertices(Params.m_NumVertices);
			Ptr->SetNumIndicis(Params.m_NumIndicis);

			return Ptr;
		}
		catch (...){
			throw;
		}
	}

	//��8�ʑ�
	shared_ptr<CommonMeshResource> CommonMeshResource::CreateOctahedron(float Size, bool AccessWrite){
		try{
			shared_ptr<CommonMeshResource> Ptr(new CommonMeshResource());

			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Octahedron�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateOctahedron(Size, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//�o�b�t�@���쐬
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//�������ݕs�̏ꍇ�̓o�b�N�A�b�v���폜
			if (!AccessWrite){
				Ptr->pImpl->m_BackupVertices.clear();
			}
			Ptr->SetVertexBuffer(Params.m_VertexBuffer);
			Ptr->SetIndexBuffer(Params.m_IndexBuffer);
			Ptr->SetNumVertices(Params.m_NumVertices);
			Ptr->SetNumIndicis(Params.m_NumIndicis);

			return Ptr;
		}
		catch (...){
			throw;
		}
	}

	shared_ptr<CommonMeshResource> CommonMeshResource::CreateDodecahedron(float Size, bool AccessWrite){
		try{
			shared_ptr<CommonMeshResource> Ptr(new CommonMeshResource());

			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Dodecahedron�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateDodecahedron(Size, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//�o�b�t�@���쐬
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//�������ݕs�̏ꍇ�̓o�b�N�A�b�v���폜
			if (!AccessWrite){
				Ptr->pImpl->m_BackupVertices.clear();
			}
			Ptr->SetVertexBuffer(Params.m_VertexBuffer);
			Ptr->SetIndexBuffer(Params.m_IndexBuffer);
			Ptr->SetNumVertices(Params.m_NumVertices);
			Ptr->SetNumIndicis(Params.m_NumIndicis);

			return Ptr;
		}
		catch (...){
			throw;
		}
	}


	shared_ptr<CommonMeshResource> CommonMeshResource::CreateIcosahedron(float Size, bool AccessWrite){
		try{
			shared_ptr<CommonMeshResource> Ptr(new CommonMeshResource());

			//�C���f�b�N�X���쐬���邽�߂̔z��
			vector<uint16_t> indices;
			//Icosahedron�̍쐬(�w���p�[�֐��𗘗p)
			VertexUtil::CreateIcosahedron(Size, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//�o�b�t�@���쐬
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//�������ݕs�̏ꍇ�̓o�b�N�A�b�v���폜
			if (!AccessWrite){
				Ptr->pImpl->m_BackupVertices.clear();
			}
			Ptr->SetVertexBuffer(Params.m_VertexBuffer);
			Ptr->SetIndexBuffer(Params.m_IndexBuffer);
			Ptr->SetNumVertices(Params.m_NumVertices);
			Ptr->SetNumIndicis(Params.m_NumIndicis);

			return Ptr;
		}
		catch (...){
			throw;
		}
	}



	vector<VertexPositionNormalTexture>& CommonMeshResource::GetBackupVertices()const {
		return pImpl->m_BackupVertices;
	}

	void CommonMeshResource::UpdateVirtexBuffer(const vector<VertexPositionNormalTexture>& NewBuffer){
		if (NewBuffer.size() != pImpl->m_BackupVertices.size()){
			// Map���s
			throw BaseException(
				L"�ύX���钸�_�̐����o�b�N�A�b�v�ƈႢ�܂�",
				L"if (NewBuffer.size() != pImpl->m_BackupVertices.size())",
				L"CommonMeshResource::UpdateVirtexBuffer()"
				);
		}
		//���W��ύX����
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//���_�o�b�t�@�����\�[�X������o��
		auto pVertexBuffer = GetVertexBuffer().Get();

		//D3D11_MAP_WRITE_DISCARD�͏d�v�B���̏����ɂ��AGPU�Ɏז�����Ȃ�
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//���_�̃}�b�v
		if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))){
			// Map���s
			throw BaseException(
				L"���_��Map�Ɏ��s���܂����B",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"CommonMeshResource::UpdateVirtexBuffer()"
				);
		}
		//���_�̕ύX
		VertexPositionNormalTexture* vertices = (VertexPositionNormalTexture*)mappedBuffer.pData;
		for (size_t i = 0; i < NewBuffer.size(); i++){
			vertices[i] = NewBuffer[i];
		}
		//�A���}�b�v
		pID3D11DeviceContext->Unmap(pVertexBuffer, 0);

	}





	//--------------------------------------------------------------------------------------
	//	struct FbxMeshResource::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct FbxMeshResource::Impl{
		unique_ptr<FbxMesh, FbxMeshDeleter> m_FbxMesh;
		//FBX�V�[��
		weak_ptr<FbxSceneResource> m_FbxSceneResource;

		//FBX���i�󔒂̏ꍇ����j
		string m_FbxName;
		//�}�e���A�����̐�
		DWORD m_MaterialCount;
		//�}�e���A���̔z��
		vector<Material> m_MaterialVec;
		//�e�N�X�`���̔z��i�e�N�X�`���͂�����ō폜�j
		vector< shared_ptr<TextureResource> > m_TextureVec;
		//�ȉ��A�X�L�����b�V���p
		//�X�L�����b�V���̏ꍇnullptr�ȊO�ɂȂ�
		unique_ptr<FbxSkin, FbxSkinDeleter> m_FbxSkin;
		//�X�L�����b�V���ł������I�ɃX�^�e�B�b�N�œǂނ��ǂ���
		bool m_NeedStatic;
		//�P�t���[���̊�ƂȂ鎞��
		FbxTime m_timePeriod;
		//�X�L���A�j���[�V�����Ɏg�p����{�[���̐�
		UINT m_NumBones;
		//�X�L���A�j���[�V�����Ɏg�p����{�[��
		//�{�[���̔z��
		vector< Bone > m_vecBones;
		//�{�[���𖼑O�ŏƉ�邷��ۂɎg�p����C���f�b�N�X�e�[�u��
		map< string, UINT > m_mapBoneList;
		//�A�j���[�V�����𖼑O�ŏƉ�邷��ۂɎg�p����C���f�b�N�X�e�[�u��
		map< string, AnimationData > m_AnimationMap;
		//�~���[�e�b�N�X
		std::mutex Mutex;
		//--------------------------------------------------------------------------------------
		//	void CreateBuffers(
		//	vector<VertexPositionNormalTexture>& vertices, //���_���쐬���邽�߂̔z��
		//	vector<uint16_t>& indices,	//�C���f�b�N�X���쐬���邽�߂̔z��
		//	bool AccessWrite		//��������OK���ǂ���
		//	VertexParams& RetParam
		//	);
		//	�p�r: �o�b�t�@���쐬����
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		template<typename T>
		void CreateBuffers(vector<T>& vertices, vector<uint16_t>& indices, bool AccessWrite, VertexParams& RetParam){
			//�f�o�C�X�̎擾
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//�o�b�t�@�̍쐬
			if (AccessWrite){
				Util::DemandCreate(RetParam.m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					//���_�o�b�t�@�̍쐬
					VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, vertices, pResult);
				});
			}
			else{
				Util::DemandCreate(RetParam.m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					//���_�o�b�t�@�̍쐬
					VertexUtil::CreatePrimitiveBuffer(pDx11Device, vertices, D3D11_BIND_VERTEX_BUFFER, pResult);
				});
			}
			//���_���̐ݒ�
			RetParam.m_NumVertices = static_cast<UINT>(vertices.size());
			Util::DemandCreate(RetParam.m_IndexBuffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				//�C���f�b�N�X�o�b�t�@�̍쐬
				VertexUtil::CreatePrimitiveBuffer(pDx11Device, indices, D3D11_BIND_INDEX_BUFFER, pResult);
			});
			//�C���f�b�N�X���̐ݒ�
			RetParam.m_NumIndicis = static_cast<UINT>(indices.size());
		}


		Impl(shared_ptr<FbxSceneResource> FbxSceneResourcePtr, FbxMesh* pFbxMesh, bool NeedStatic) :
			m_FbxSceneResource(FbxSceneResourcePtr),
			m_MaterialCount(0),
			m_FbxSkin(nullptr),
			m_timePeriod(0),
			m_NumBones(0),
			m_FbxMesh(pFbxMesh),
			m_NeedStatic(NeedStatic)
		{}
		~Impl(){
		}
	};

	//--------------------------------------------------------------------------------------
	//	class FbxMeshResource : public BaseResource;
	//	�p�r: FBX���b�V�����\�[�X�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FbxMeshResource::FbxMeshResource(
		shared_ptr<FbxSceneResource> FbxSceneResourcePtr,
		FbxMesh* pFbxMesh,
		bool NeedStatic
		) :
		MeshResource(),
		pImpl(new Impl(FbxSceneResourcePtr, pFbxMesh, NeedStatic))
	{}
	FbxMeshResource::~FbxMeshResource(){}

	//������

	//--------------------------------------------------------------------------------------
	void FbxMeshResource::CreateInstanceFromStaticFbx(){
		VertexParams Params;
		//���b�V���P�̂̓ǂݍ���
		DWORD dwNumPolygons = 0;	//�|���S����
		//���_���Ȃ�	
		if ((Params.m_NumVertices = pImpl->m_FbxMesh->GetControlPointsCount()) <= 0) {
			//���s����
			throw BaseException(L"Fbx�ɒ��_������܂���",
				L"m_FbxMesh->GetControlPointsCount() <= 0",
				L"FbxMeshResource::Create()");
		}
		//����FBX�ɖ��O������΂����ێ�
		if (pImpl->m_FbxMesh->GetName()){
			pImpl->m_FbxName = pImpl->m_FbxMesh->GetName();
		}
		//�|���S�����̎擾
		dwNumPolygons = pImpl->m_FbxMesh->GetPolygonCount();

		//���_���쐬���邽�߂̔z��
		vector<VertexPositionNormalTexture> vertices(Params.m_NumVertices);

		FbxStringList sUVSetNames;
		pImpl->m_FbxMesh->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//���_���W�E�@���E�e�N�X�`�����W�̎擾
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//�|���S���̃T�C�Y�𓾂�i�ʏ�R�j
			const DWORD dwPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = pImpl->m_FbxMesh->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbx���璸�_�𓾂�
				vPos = pImpl->m_FbxMesh->GetControlPointAt(iIndex);
				//�@���𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexNormal(i, j, vNormal);
				//UV�l�𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				vertices[iIndex] =
					VertexPositionNormalTexture(
					//���_�̐ݒ�
					//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
					XMFLOAT3(static_cast< float >(vPos[0]), static_cast< float >(vPos[1]), -static_cast< float >(vPos[2])),
					//�@���̐ݒ�
					//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
					XMFLOAT3(static_cast< float >(vNormal[0]), static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
					//UV�l�̐ݒ�
					//V�̒l���A1.0����������l�ɂȂ�
					XMFLOAT2(static_cast< float >(vUV[0]), 1.0f - static_cast< float >(vUV[1]))
					);
			}
		}

		//�}�e���A���̐����擾����(��̃��b�V���ɑ΂��A�����̃}�e���A�������蓖�Ă��Ă��邱�Ƃ�����)
		pImpl->m_MaterialCount = pImpl->m_FbxMesh->GetNode()->GetMaterialCount();
		//�}�e���A���̓ǂݍ���
		CreateMaterial();


		//�C���f�b�N�X���쐬���邽�߂̔z��
		vector<uint16_t> indices;
		//�}�e���A���̃|�C���^���擾����
		const FbxLayerElementMaterial*	fbxMaterial = pImpl->m_FbxMesh->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < pImpl->m_MaterialCount; i++) {
			//���_�C���f�b�N�X���œK������(�����}�e���A�����g�p����|���S�����܂Ƃ߂ĕ`��ł���悤�ɕ��ׁA
			//�`�掞�Ƀ}�e���A���̐؂�ւ��񐔂����炷)
			for (DWORD j = 0; j < dwNumPolygons; j++) {
				DWORD	dwMaterialId = fbxMaterial->GetIndexArray().GetAt(j);
				if (dwMaterialId == i) {
					int iPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(j);
					for (int k = 0; k < iPolygonSize; k++) {
						indices.push_back(static_cast< uint16_t >(pImpl->m_FbxMesh->GetPolygonVertex(j, 2 - k)));
						pImpl->m_MaterialVec[i].m_IndexCount++;
					}
				}
			}
		}

		//�}�e���A���z��ɃX�^�[�g�n�_��ݒ�
		UINT StarIndex = 0;
		for (DWORD i = 0; i < pImpl->m_MaterialVec.size(); i++) {
			pImpl->m_MaterialVec[i].m_StartIndex = StarIndex;
			StarIndex += pImpl->m_MaterialVec[i].m_IndexCount;
		}

		//�z������Ƃɒ��_�ƃC���f�b�N�X���쐬
		pImpl->CreateBuffers(vertices, indices,false,Params);

		SetVertexBuffer(Params.m_VertexBuffer);
		SetIndexBuffer(Params.m_IndexBuffer);
		SetNumVertices(Params.m_NumVertices);
		SetNumIndicis(Params.m_NumIndicis);

	}

	//--------------------------------------------------------------------------------------
	void FbxMeshResource::CreateInstanceFromSkinFbx(){
		VertexParams Params;

		//���b�V���P�̂̓ǂݍ���
		DWORD dwNumPolygons = 0;	//�|���S����
		//���_���Ȃ�	
		if ((Params.m_NumVertices = pImpl->m_FbxMesh->GetControlPointsCount()) <= 0) {
			//���s����
			throw BaseException(L"Fbx�ɒ��_������܂���",
				L"m_pFbxMesh->GetControlPointsCount() <= 0",
				L"FbxMeshResource::CreateInstanceFromSkinFbx()");
		}
		//����FBX�ɖ��O������΂����ێ�
		if (pImpl->m_FbxMesh->GetName()){
			pImpl->m_FbxName = pImpl->m_FbxMesh->GetName();
		}
		//�|���S�����̎擾
		dwNumPolygons = pImpl->m_FbxMesh->GetPolygonCount();

		//���_���쐬���邽�߂̔z��
		//�u�����h�E�F�C�g�ƃu�����h�C���f�b�N�X������
		vector<VertexPositionNormalTextureSkinning> vertices(Params.m_NumVertices);

		FbxStringList sUVSetNames;
		pImpl->m_FbxMesh->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//���_���W�E�@���E�e�N�X�`�����W�̎擾
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//�|���S���̃T�C�Y�𓾂�i�ʏ�R�j
			const DWORD dwPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = pImpl->m_FbxMesh->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbx���璸�_�𓾂�
				vPos = pImpl->m_FbxMesh->GetControlPointAt(iIndex);
				//�@���𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexNormal(i, j, vNormal);
				//UV�l�𓾂�
				pImpl->m_FbxMesh->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				uint32_t temp[4] = { 0, 0, 0, 0 };
				float tempf[4] = { 0, 0, 0, 0 };
				vertices[iIndex] =
					VertexPositionNormalTextureSkinning(
					//���_�̐ݒ�
					//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
					XMFLOAT3(static_cast< float >(vPos[0]), static_cast< float >(vPos[1]), -static_cast< float >(vPos[2])),
					//�@���̐ݒ�
					//Z���W��Fbx�Ƃ͕������t�ɂȂ�iDirectX�͍�����W�n�j
					XMFLOAT3(static_cast< float >(vNormal[0]), static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
					//UV�l�̐ݒ�
					//V�̒l���A1.0����������l�ɂȂ�
					XMFLOAT2(static_cast< float >(vUV[0]), 1.0f - static_cast< float >(vUV[1])),
					//�u�����h�C���f�b�N�X�͂Ƃ肠����0
					temp,
					//�u�����h�E�G�C�g�͂Ƃ肠����0
					tempf
					);
			}
		}

		//�u�����h�E�F�C�g�ƃu�����h�C���f�b�N�X�̓ǂݍ���
		const int	iNumCluster = pImpl->m_FbxSkin->GetClusterCount();
		// �ϊ����� FbxSkin ���� �N���X�^�[(�{�[��)�̐����擾����

		for (int i = 0; i < iNumCluster; i++) {
			int		iNumBlendIndices = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointIndicesCount();	// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�̐����擾����
			int*	piBlendIndices = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointIndices();		// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�̔ԍ���z��Ŏ擾����
			double*	pdBlendWeights = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointWeights();		// i �Ԗڂ̃N���X�^�[�ɉe�����󂯂钸�_�ɑΉ������d�݃f�[�^��z��Ŏ擾����

			for (int j = 0; j < iNumBlendIndices; j++) {
				int	idx = piBlendIndices[j];

				//���_�ɓo�^����S�̃u�����h�E�F�C�g�̂����A�ŏ��̒l�����v�f����������iDirectX9�̌Œ�V�F�[�_�ł͂S�̃{�[�����炵���u�����h�ł��Ȃ��j
				int	iMinIndex = 0;
				for (int k = 0; k < 4 - 1; k++) {
					for (int l = k + 1; l < 4; l++) {
						if (vertices[idx].weights[k] < vertices[idx].weights[l]) {
							iMinIndex = k;
						}
						else {
							iMinIndex = l;
							k = l;
							break;
						}
					}
				}
				//���łɓo�^����Ă��钆�ōŏ��̃u�����h�E�F�C�g�����傫���l�����f�[�^��o�^����
				if (static_cast< float >(pdBlendWeights[j]) > vertices[idx].weights[iMinIndex]) {
					vertices[idx].indices[iMinIndex] = static_cast< BYTE >(i);
					vertices[idx].weights[iMinIndex] = static_cast< float >(pdBlendWeights[j]);
				}
			}
		}
		//�E�G�C�g�̃`�F�b�N
		//�e���_���ƂɃE�F�C�g�𑫂���1.0f�ɂȂ�Ȃ��ƃX�L�������܂��ł��Ȃ�
		for (size_t i = 0; i < vertices.size(); i++){
			float f = vertices[i].weights[0] + vertices[i].weights[1] + vertices[i].weights[2] + vertices[i].weights[3];
			if (f > 0 && f < 1.0f){
				float k = 1.0f / f;
				vertices[i].weights[0] *= k;
				vertices[i].weights[1] *= k;
				vertices[i].weights[2] *= k;
				vertices[i].weights[3] *= k;
			}
		}

		//��^�C�}�[�̐ݒ�
		FbxGlobalSettings&	globalTimeSettings = pImpl->m_FbxMesh->GetScene()->GetGlobalSettings();
		FbxTime::EMode timeMode = globalTimeSettings.GetTimeMode();
		pImpl->m_timePeriod.SetTime(0, 0, 0, 1, 0, timeMode);

		//�}�e���A���̐����擾����(��̃��b�V���ɑ΂��A�����̃}�e���A�������蓖�Ă��Ă��邱�Ƃ�����)
		pImpl->m_MaterialCount = pImpl->m_FbxMesh->GetNode()->GetMaterialCount();
		//�}�e���A���̓ǂݍ���
		CreateMaterial();

		//�C���f�b�N�X�o�b�t�@�̍쐬
		//�C���f�b�N�X���쐬���邽�߂̔z��
		vector<uint16_t> indices;
		//�}�e���A���̃|�C���^���擾����
		const FbxLayerElementMaterial*	fbxMaterial = pImpl->m_FbxMesh->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < pImpl->m_MaterialCount; i++) {
			//���_�C���f�b�N�X���œK������(�����}�e���A�����g�p����|���S�����܂Ƃ߂ĕ`��ł���悤�ɕ��ׁA
			//�`�掞�Ƀ}�e���A���̐؂�ւ��񐔂����炷)
			for (DWORD j = 0; j < dwNumPolygons; j++) {
				DWORD	dwMaterialId = fbxMaterial->GetIndexArray().GetAt(j);
				if (dwMaterialId == i) {
					int iPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(j);
					for (int k = 0; k < iPolygonSize; k++) {
						indices.push_back(static_cast< uint16_t >(pImpl->m_FbxMesh->GetPolygonVertex(j, 2 - k)));
						pImpl->m_MaterialVec[i].m_IndexCount++;
					}
				}
			}
		}

		//�}�e���A���z��ɃX�^�[�g�n�_��ݒ�
		UINT StarIndex = 0;
		for (DWORD i = 0; i < pImpl->m_MaterialVec.size(); i++) {
			pImpl->m_MaterialVec[i].m_StartIndex = StarIndex;
			StarIndex += pImpl->m_MaterialVec[i].m_IndexCount;
		}


		//�o�b�t�@�̍쐬
		pImpl->CreateBuffers(vertices, indices, false, Params);


		//�{�[�����𓾂�
		pImpl->m_NumBones = pImpl->m_FbxSkin->GetClusterCount();
		for (UINT i = 0; i < pImpl->m_NumBones; i++) {
			Bone	bone;

			FbxAMatrix	mBindPose, mCurrentPose;
			pImpl->m_FbxSkin->GetCluster(i)->GetTransformLinkMatrix(mBindPose);
			mCurrentPose = pImpl->m_FbxSkin->GetCluster(i)->GetLink()->EvaluateGlobalTransform(pImpl->m_timePeriod * 0);

			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					bone.m_BindPose(r, c) = static_cast< float >(mBindPose.Get(r, c));
					bone.m_CurrentPose(r, c) = static_cast< float >(mCurrentPose.Get(r, c));
				}
			}

			Matrix4X4	mMirror, mBindInverse;
			mMirror.Identity();
			mMirror(2, 2) = -1.0f;

			bone.m_BindPose *= mMirror;
			bone.m_CurrentPose *= mMirror;
			Vector4 temp4;
			mBindInverse = Matrix4X4EX::Inverse(&temp4, bone.m_BindPose);
			bone.m_ConbinedPose = mBindInverse * bone.m_CurrentPose;

			pImpl->m_vecBones.push_back(bone);
			//�}�b�v�̓o�^
			pImpl->m_mapBoneList[pImpl->m_FbxSkin->GetCluster(i)->GetName()] = i;
		}
		SetVertexBuffer(Params.m_VertexBuffer);
		SetIndexBuffer(Params.m_IndexBuffer);
		SetNumVertices(Params.m_NumVertices);
		SetNumIndicis(Params.m_NumIndicis);

	}

	//--------------------------------------------------------------------------------------
	void FbxMeshResource::CreateMaterial(){
		//�}�e���A���̐ݒ�
		//�e�N�X�`���t�@�C�����쐬�̂��߂̃��[�N�z��
		wchar_t Buff[MAX_PATH];
		setlocale(LC_CTYPE, "jpn");

		if (pImpl->m_FbxSceneResource.expired()){
			//���s����
			throw BaseException(
				L"FbxSceneResource�������ł�",
				L"if (pImpl->m_FbxSceneResource.expired())",
				L"FbxMeshResource::CreateMaterial()");

		}
		auto FbxSceneResourcePtr = pImpl->m_FbxSceneResource.lock();

		for (DWORD i = 0; i < pImpl->m_MaterialCount; i++) {
			//�}�e���A���擾
			Material material;
			::ZeroMemory(&material, sizeof(Material));

			FbxSurfaceMaterial*			pMaterial = pImpl->m_FbxMesh->GetNode()->GetMaterial(i);
			FbxSurfacePhong*			pPhong = (FbxSurfacePhong*)pMaterial;
			FbxPropertyT<FbxDouble3>	color;
			color = pPhong->Ambient;
			//�A���r�G���g�͎g�p���Ȃ�
			material.m_Ambient = Color4(0.5f, 0.5f, 0.5f, 1.0f);
			//			material.m_Ambient = Color4((float)color.Get()[ 0 ], (float)color.Get()[ 1 ], (float)color.Get()[ 2 ], 1.0f);
			color = pPhong->Diffuse;
			//�f�t�B�[�Y�̓e�N�X�`�����g���̂Ńt��
			//			material.m_Diffuse = Color4(1.0f,1.0f,1.0f,1.0f);
			material.m_Diffuse = Color4((float)color.Get()[0], (float)color.Get()[1], (float)color.Get()[2], 1.0f);
			//			material.m_Specular = Color4(0.0f,0.0f,0.0f,0.4f);
			material.m_Specular = Color4((float)color.Get()[0] * 0.5f, (float)color.Get()[1] * 0.5f, (float)color.Get()[2] * 0.5f, 0.4f);
			color = pPhong->Emissive;
			//�G�~�b�V�u��DirectXTK�f�t�H���g
			//			material.m_Emissive = Color4(0.05333332f,0.09882354f,0.1819608f,1.0f);
			material.m_Emissive = Color4((float)color.Get()[0], (float)color.Get()[1], (float)color.Get()[2], 1.0f);
			//�}�e���A���Ɋ֘A�t�����Ă���e�N�X�`����ǂݍ���
			const FbxProperty	fbxProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			//�e�N�X�`������t�@�C�����𔲂��o��
			FbxFileTexture*	pFbxFileTexture = fbxProperty.GetSrcObject< FbxFileTexture >(i);
			TextureResource* pTexture = 0;
			if (pFbxFileTexture) {
				//�e�N�X�`���t�@�C��������p�X��r�����t�@�C����+�g���q�Ƃ��č�������
				char szTextureFilename[256], szFileExt[8];
				_splitpath_s(pFbxFileTexture->GetFileName(), nullptr, 0, nullptr, 0, szTextureFilename, 256, szFileExt, 8);
				//�t�@�C�����̍���
				string sTextureFilename(szTextureFilename);
				sTextureFilename += szFileExt;
				//UNI�R�[�h�ɕϊ�
				size_t size = 0;
				mbstowcs_s(&size, Buff, sTextureFilename.c_str(), MAX_PATH - 1);
				wstring strWork = FbxSceneResourcePtr->GetDataDir() + Buff;
				//pFbxFileTexture���烉�b�v���[�h���擾���ăe�N�X�`�����쐬
				auto PtrTexture = make_shared<TextureResource>(strWork.c_str());
				pImpl->m_TextureVec.push_back(PtrTexture);
				material.m_Texture = PtrTexture;
			}
			else{
				if (pImpl->m_FbxSkin){
					throw BaseException(
						L"�X�L�����b�V���̃e�N�X�`��������܂���B\n�X�L�����b�V���̓e�N�X�`�������͑Ή����Ă܂���B",
						L"if ( !pFbxFileTexture )",
						L"FbxMeshResource::CreateMaterial()"
						);
				}
				//�e�N�X�`�����Ȃ��ꍇ��0��ݒ�
				material.m_Texture = nullptr;
			}
			//�}�e���A���z��ɒǉ�
			pImpl->m_MaterialVec.push_back(material);
		}
	}

	void FbxMeshResource::Create(){
		try{
			//�X�L�����b�V�����ǂ����𔻒f
			pImpl->m_FbxSkin.reset(FbxCast< FbxSkin >(pImpl->m_FbxMesh->GetDeformer(0, FbxDeformer::eSkin)));
			//�X�L�����b�V���̏ꍇ
			if (pImpl->m_FbxSkin && !pImpl->m_NeedStatic){
				CreateInstanceFromSkinFbx();
			}
			//�X�^�e�B�b�N���b�V���������͋����I�ɃX�^�e�B�b�N�œǂޏꍇ
			else{
				pImpl->m_FbxSkin = nullptr;
				CreateInstanceFromStaticFbx();
			}
		}
		catch (...){
			throw;
		}
	}

	//�A�N�Z�T
	//�X�L�j���O���邩�ǂ���
	bool FbxMeshResource::IsSkining() const{
		return (pImpl->m_FbxSkin && !pImpl->m_NeedStatic);
	}

	const AnimationData& FbxMeshResource::GetAnimationData(const string& AnimeName) const{
		auto it = pImpl->m_AnimationMap.find(AnimeName);
		if (it != pImpl->m_AnimationMap.end()){
			return it->second;
		}
		else{
			throw BaseMBException(
				"�w��̃A�j���[�V������������܂���",
				AnimeName,
				"AnimationData& FbxMeshResource::GetAnimationData()");
		}
	}



	const unique_ptr<FbxMesh, FbxMeshResource::FbxMeshDeleter>& FbxMeshResource::GetFbxMesh()const{
		return pImpl->m_FbxMesh;
	}
	const unique_ptr<FbxSkin, FbxMeshResource::FbxSkinDeleter>& FbxMeshResource::GetFbxSkin() const{
		return pImpl->m_FbxSkin;
	}


	UINT FbxMeshResource::GetNumBones() const{
		return pImpl->m_NumBones;
	}

	const vector<Material>& FbxMeshResource::GetMaterialVec()const{
		return pImpl->m_MaterialVec;
	}

	const vector<Bone>& FbxMeshResource::GetBonesVec() const{
		return pImpl->m_vecBones;
	}


	//
	bool FbxMeshResource::GenerateCurrentPose(vector<Bone>& tgtBones, AnimationData& rAnimationData, float CurrentTime){
		//�o�ߎ��Ԃ���t���[�������߂�
		float fCurrentFrame = CurrentTime * rAnimationData.m_FramesParSecond;
		//�A�j���[�V�������Ō�܂ł����Ă邩�ǂ������m�F
		bool ret = false;
		if (!rAnimationData.m_IsLoop && ((UINT)fCurrentFrame >= rAnimationData.m_FrameLength)){
			//���[�v���Ȃ��ŁA�A�j�����Ō�ɓ��B������
			ret = true;
		}
		int iAnimationFrame = static_cast< int >(fCurrentFrame * 10000.0f);

		if (rAnimationData.m_FrameLength > 0) {
			if (rAnimationData.m_IsLoop){
				//���[�v����
				iAnimationFrame %= rAnimationData.m_FrameLength * 10000;
				iAnimationFrame += rAnimationData.m_StartFrame * 10000;
			}
			else{
				//���[�v���Ȃ�
				if (ret){
					//�Ō�̃t���[���Ƀ|�[�Y��ݒ肷��
					iAnimationFrame = (rAnimationData.m_StartFrame + rAnimationData.m_FrameLength) * 10000;
				}
				else{
					iAnimationFrame += rAnimationData.m_StartFrame * 10000;
				}
			}
		}
		FbxAMatrix	mCurrentPose;
		Matrix4X4	mMirror, mBindInverse;
		mMirror.Identity();
		mMirror(2, 2) = -1.0f;	//���W�n�̊֌W�Ń{�[�������]���Ă���̂ŁA�����DirectX�ɍ��킹�邽�߂Ɏg�p����s��

		//�J�����g�s��ƍ����s����Čv�Z����
		for (UINT i = 0; i < pImpl->m_NumBones; i++) {
			mCurrentPose = pImpl->m_FbxSkin->GetCluster(i)->GetLink()->EvaluateGlobalTransform((pImpl->m_timePeriod * iAnimationFrame) / 10000);
			for (int r = 0; r < 4; r++) {
				for (int c = 0; c < 4; c++) {
					tgtBones[i].m_CurrentPose(r, c) = static_cast< float >(mCurrentPose.Get(r, c));
				}
			}
			tgtBones[i].m_CurrentPose *= mMirror;

			Vector4 temp;
			mBindInverse = Matrix4X4EX::Inverse(&temp, tgtBones[i].m_BindPose);

			tgtBones[i].m_ConbinedPose = mBindInverse * tgtBones[i].m_CurrentPose;
		}
		rAnimationData.m_IsLoopEnd = ret;
		return ret;
	}

	void FbxMeshResource::AddAnimation(const char* Name, UINT StartFrame, UINT FrameLength, bool Loop,
		float FramesParSecond){
		if (pImpl->m_FbxSkin && pImpl->m_NumBones > 0){
			map< string, AnimationData >::iterator it
				= pImpl->m_AnimationMap.find(Name);
			if (it != pImpl->m_AnimationMap.end()) {
				//�w��̖��O����������
				//���̃f�[�^�ɍ����ւ�
				it->second.m_StartFrame = StartFrame;
				it->second.m_FrameLength = FrameLength;
				it->second.m_FramesParSecond = FramesParSecond;
				it->second.m_IsLoop = Loop;
			}
			else{
				//������Ȃ�
				//�A�j���[�V������`�̒ǉ�
				pImpl->m_AnimationMap[Name] = AnimationData(StartFrame, FrameLength,
					Loop, FramesParSecond);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct FbxSceneResource::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct FbxSceneResource::Impl{
		//�f�[�^�f�B���N�g����
		wstring m_DataDir;
		//FBX�t�@�C����
		wstring m_FileName;
		//FBX�V�[����
		string m_FbxSceneName;
		//FBX�V�[��
		shared_ptr<FbxScene> m_FbxScene;
		//�����I��static�œǂނ��ǂ���
		bool m_NeedStatic;
		//�P���FBX���b�V���̔z��
		vector< shared_ptr<FbxMeshResource> > m_FbxMeshResourceVec;

		Impl(const wstring& DataDir,
			const wstring& FileName, const string& SceneName, bool  NeedStatic) :
			m_DataDir(DataDir),
			m_FileName(FileName),
			m_FbxSceneName(SceneName),
			m_NeedStatic(NeedStatic)
		{}
		~Impl(){}

	};


	//--------------------------------------------------------------------------------------
	//	class FbxSceneResource : public BaseResource;
	//	�p�r: FBX�V�[�����\�[�X�N���X
	//	�����FBX�t�@�C���͂P��FbxSceneResource�Ƃ��ĊǗ�����
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FbxSceneResource::FbxSceneResource(const wstring& DataDir,
		const wstring& FileName, const string& SceneName, bool NeedStatic) :
		pImpl(new Impl(DataDir, FileName, SceneName, NeedStatic))
	{
		try{
			if (FileName == L""){
				//���s����
				throw BaseException(
					L"Fbx�t�@�C�����������ł�",
					L"if(FileName == \"\")",
					L"FbxSceneResource::FbxSceneResource()");
			}

			if (pImpl->m_FbxSceneName == ""){
				if (FileName == L""){
					//���s����
					throw BaseException(
						L"�t�@�C�����������ł�",
						L"if(FileName == \"\")",
						L"FbxSceneResource::FbxSceneResource()");
				}
				wstring strWork;
				strWork = pImpl->m_FileName;
				//�t�@�C�������}���`�o�C�g�ɕϊ����V�[�������쐬
				Util::WStoMB(strWork, pImpl->m_FbxSceneName);
				pImpl->m_FbxSceneName += ".scene";
			}
		}
		catch (...){
			throw;
		}
	}
	//static�\�z
	shared_ptr<FbxSceneResource> FbxSceneResource::CreateFbxScene(const wstring& DataDir,
		const wstring& FileName, const string& SceneName, bool NeedStatic){
		shared_ptr<FbxSceneResource> ScenePtr(new FbxSceneResource(DataDir, FileName, SceneName, NeedStatic));
		ScenePtr->Create();
		return ScenePtr;
	}
	FbxSceneResource::~FbxSceneResource(){}

	//�ċA�I�ɃV�[����ǂݍ���
	void FbxSceneResource::CreateFbxMeshVector(FbxNode* pFbxNode){
		//�m�[�h���L���łȂ���΃��^�[��
		if (!pFbxNode){
			return;
		}
		//FBX�V�[���̎擾
		FbxScene* pScene = pFbxNode->GetScene();
		//�m�[�h�̑������擾
		FbxNodeAttribute* pFbxNodeAttribute = pFbxNode->GetNodeAttribute();
		//�m�[�h�̑������L���ł���΁A�����̃^�C�v�𔻒肵�A�^�C�v�����b�V���ł����vector�ɓo�^����
		if (pFbxNodeAttribute){
			if (pFbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
				//�O�p����
				FbxGeometryConverter converter(pFbxNode->GetFbxManager());
				converter.Triangulate(pScene, true);
				//���b�V�������o��
				FbxMesh* pFbxMesh = pFbxNode->GetMesh();
				//�V�[���̃O���[�o���s����擾
				FbxAnimEvaluator* pFbxAnimEvaluator = pScene->GetAnimationEvaluator();
				FbxMatrix mGlobal = pFbxAnimEvaluator->GetNodeGlobalTransform(pFbxNode);
				//�O���[�o���s������ׂĂ̒��_�ɓK�p
				int	iNumControlPoints = pFbxMesh->GetControlPointsCount();
				FbxVector4*	v = pFbxMesh->GetControlPoints();
				for (int i = 0; i < iNumControlPoints; i++){
					v[i] = mGlobal.MultNormalize(v[i]);
				}
				//Fbx���b�V���̔z��ɓo�^
				auto FbxMeshPtr = make_shared<FbxMeshResource>(GetThis<FbxSceneResource>(), pFbxMesh,pImpl->m_NeedStatic);
				FbxMeshPtr->Create();
				pImpl->m_FbxMeshResourceVec.push_back(
					FbxMeshPtr
					);
			}
		}
		//�q�m�[�h�̐����擾����
		int iNumChild = pFbxNode->GetChildCount();
		for (int i = 0; i < iNumChild; i++) {
			//�q�m�[�h���ׂĂ��ċA�I�Ƀ��b�V����o�^����
			CreateFbxMeshVector(pFbxNode->GetChild(i));
		}
	}
	//������
	void FbxSceneResource::Create(){
		try{

			//�C���|�[�^�[���쐬����
			FbxImporter* lImporter = FbxImporter::Create(App::GetApp()->GetFbxManager().get(), "");
			//�t�@�C�������}���`�o�C�g�ɂ���
			wstring strWork;
			strWork = pImpl->m_DataDir + pImpl->m_FileName;
			//�}���`�o�C�g�ɕϊ�
			string sFileName;
			Util::WStoMB(strWork, sFileName);
			//�C���|�[�^�[�̏�����
			if (!lImporter->Initialize(sFileName.c_str(), -1, App::GetApp()->GetFbxManager().get()->GetIOSettings())) {
				//���s����
				throw BaseMBException(
					"�C���|�[�^�̏������Ɏ��s���܂���",
					lImporter->GetStatus().GetErrorString(),
					"FbxSceneResource::FbxSceneResource()");
			}
			//�V�[���̍쐬
			pImpl->m_FbxScene = shared_ptr<FbxScene>(
				FbxScene::Create(App::GetApp()->GetFbxManager().get(), pImpl->m_FbxSceneName.c_str()),	//���|�C���^
				FbxSceneDeleter()	//�J�X�^���f���[�^
				);
			//�C���|�[�^�ɂ��V�[���ւ̓ǂݍ���
			lImporter->Import(pImpl->m_FbxScene.get());
			//�C���|�[�^�͂����K�v�Ȃ�
			lImporter->Destroy();


			//�ċA�I�ɓǂݍ���
			CreateFbxMeshVector(pImpl->m_FbxScene->GetRootNode());


		}
		catch (...){
			throw;
		}
	}



	//�A�N�Z�T
	wstring FbxSceneResource::GetDataDir() const{
		return pImpl->m_DataDir;
	}
	wstring FbxSceneResource::GetFileName() const{
		return pImpl->m_FileName;
	}
	string FbxSceneResource::GetFbxSceneName() const{
		return pImpl->m_FbxSceneName;
	}
	shared_ptr<FbxMeshResource> FbxSceneResource::GetFbxMeshResource(size_t Index) const{
		if (Index >= pImpl->m_FbxMeshResourceVec.size()){
			auto str = Util::UintToWStr(Index);
			str += L" >= m_FbxMeshResourceVec.size()";
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				str,
				L"FbxSceneResource::GetMeshResource()"
				);
		}
		return pImpl->m_FbxMeshResourceVec[Index];
	}
	size_t FbxSceneResource::GetFbxMeshResourceSize() const{
		return pImpl->m_FbxMeshResourceVec.size();
	}
	vector< shared_ptr<FbxMeshResource> >& FbxSceneResource::GetFbxMeshResourceVec() const{
		return pImpl->m_FbxMeshResourceVec;
	}


}

//endof  basedx11

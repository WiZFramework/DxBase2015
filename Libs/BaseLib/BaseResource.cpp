#include "stdafx.h"


namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	struct TextureResource::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct TextureResource::Impl{
		ComPtr<ID3D11ShaderResourceView> m_ShaderResView;	//リソースビュー
		wstring m_FileName;		//テクスチャファイルへのパス

		Impl(const wstring& FileName, const wstring& TexType = L"WIC");
		~Impl(){}
		//ミューテックス
		std::mutex Mutex;

	};

	TextureResource::Impl::Impl(const wstring& FileName, const wstring& TexType) :
		m_FileName(FileName){
		try{
			if (FileName == L""){
				throw BaseException(
					L"ファイルが指定されていません",
					L"if(FileName == L\"\")",
					L"Texture::Impl::Impl()"
					);
			}
			DWORD RetCode;
			RetCode = GetFileAttributes(m_FileName.c_str());
			if (RetCode == -1){
				wstring patherr = m_FileName;
				throw BaseException(
					L"ファイルが存在しません",
					patherr.c_str(),
					L"Texture::Impl::Impl()"
					);
			}
			HRESULT hr;
			//テクスチャ作成
			DirectX::TexMetadata metadata;
			DirectX::ScratchImage image;

			if (TexType == L"WIC"){
				//ファイル拡張子の調査
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
						L"テクスチャの読み込みに失敗しました",
						m_FileName,
						L"Texture::Impl::Impl()"
						);
				}
				else if (ExtStr == L".tga" || ExtStr == L".TGA"){
					ThrowIfFailed(
						DirectX::LoadFromTGAFile(m_FileName.c_str(), &metadata, image),
						L"テクスチャの読み込みに失敗しました",
						m_FileName,
						L"Texture::Impl::Impl()"
						);
				}
				else{
					ThrowIfFailed(
						DirectX::LoadFromWICFile(m_FileName.c_str(), 0, &metadata, image),
						L"テクスチャの読み込みに失敗しました",
						m_FileName,
						L"Texture::Impl::Impl()"
						);
				}
			}
			else if (TexType == L"DDS"){
				ThrowIfFailed(
					DirectX::LoadFromDDSFile(m_FileName.c_str(), 0, &metadata, image),
					L"テクスチャの読み込みに失敗しました",
					m_FileName,
					L"Texture::Impl::Impl()"
					);
			}
			else if (TexType == L"TGA"){
				ThrowIfFailed(
					DirectX::LoadFromTGAFile(m_FileName.c_str(), &metadata, image),
					L"テクスチャの読み込みに失敗しました",
					m_FileName,
					L"Texture::Impl::Impl()"
					);
			}
			else{
				throw BaseException(
					L"そのタイプは対応していません",
					TexType.c_str(),
					L"Texture::Impl::Impl()"
					);
			}

			//デバイスとコンテキストインターフェイスの取得
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			ID3D11DeviceContext* pID3D11DeviceContex = Dev->GetD3DDeviceContext();

			Util::DemandCreate(m_ShaderResView, Mutex, [&](ID3D11ShaderResourceView** pResult) -> HRESULT
			{
				// 画像からシェーダリソースViewの作成
				hr = ThrowIfFailed(CreateShaderResourceView(pDx11Device, image.GetImages(), image.GetImageCount(), metadata, pResult),
					L"シェーダーリソースビューを作成できません",
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
	//	用途: テクスチャのラッピングクラス
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//	TextureResource(
	//	const wstring& FileName	//ファイル名
	//	);
	//	用途: コンストラクタ
	//	戻り値: なし
	//--------------------------------------------------------------------------------------
	TextureResource::TextureResource(const wstring& FileName, const wstring& TexType) :
		BaseResource(),
		pImpl(new Impl(FileName, TexType))
	{
	}
	TextureResource::~TextureResource(){
	}

	//アクセッサ
	ComPtr<ID3D11ShaderResourceView>& TextureResource::GetShaderResourceView() const{
		return pImpl->m_ShaderResView;
	}
	const wstring& TextureResource::GetTextureFileName() const{
		return pImpl->m_FileName;
	}


	//--------------------------------------------------------------------------------------
	//	struct AudioResource::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct AudioResource::Impl{
		WAVEFORMATEX m_WaveFormat;	//ウェブフォーマット
		vector<byte> m_SoundData;	//データ
		wstring m_FileName;		//ファイルへのパス
		Impl(const wstring& FileName) :
			m_FileName(FileName){}
		~Impl(){}
		//ミューテックス
		std::mutex Mutex;

	};


	//--------------------------------------------------------------------------------------
	//	class AudioResource : public BaseResource;
	//	用途: オーディオリソース（wavなど）のラッピングクラス
	//--------------------------------------------------------------------------------------
	AudioResource::AudioResource(const wstring& FileName):
		BaseResource(),
		pImpl(new Impl(FileName))
	{
		try{

			ThrowIfFailed(
				MFStartup(MF_VERSION),
				L"MediaFoundationの初期化に失敗しました",
				L"MFStartup(MF_VERSION)",
				L"AudioResource::AudioResource()"
				);

			ComPtr<IMFSourceReader> reader;

			ThrowIfFailed(
				MFCreateSourceReaderFromURL(FileName.c_str(),nullptr,&reader),
				L"サウンドリーダーの作成に失敗しました",
				L"MFCreateSourceReaderFromURL(FileName.c_str(),nullptr,&reader)",
				L"AudioResource::AudioResource()"
				);

			// Set the decoded output format as PCM.
			// XAudio2 on Windows can process PCM and ADPCM-encoded buffers.
			// When using MediaFoundation, this sample always decodes into PCM.
			Microsoft::WRL::ComPtr<IMFMediaType> mediaType;

			ThrowIfFailed(
				MFCreateMediaType(&mediaType),
				L"メディアタイプの作成に失敗しました",
				L"MFCreateMediaType(&mediaType)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio),
				L"メディアGUIDの設定に失敗しました",
				L"mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM),
				L"メディアサブGUIDの設定に失敗しました",
				L"mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get()),
				L"リーダーへのメディアタイプの設定に失敗しました",
				L"reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get())",
				L"AudioResource::AudioResource()"
				);


			// Get the complete WAVEFORMAT from the Media Type.
			Microsoft::WRL::ComPtr<IMFMediaType> outputMediaType;

			ThrowIfFailed(
				reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType),
				L"出力用のメディアタイプの設定に失敗しました",
				L"reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType)",
				L"AudioResource::AudioResource()"
				);

			UINT32 size = 0;
			WAVEFORMATEX* waveFormat;

			ThrowIfFailed(
				MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size),
				L"ウェブフォーマットの設定に失敗しました",
				L"MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size)",
				L"AudioResource::AudioResource()"
				);


			CopyMemory(&pImpl->m_WaveFormat, waveFormat, sizeof(pImpl->m_WaveFormat));
			CoTaskMemFree(waveFormat);

			PROPVARIANT propVariant;

			ThrowIfFailed(
				reader->GetPresentationAttribute(static_cast<uint32>(MF_SOURCE_READER_MEDIASOURCE), MF_PD_DURATION, &propVariant),
				L"アトリビュートの設定に失敗しました",
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
					L"サンプラーの読み込みに失敗しました",
					L"reader->ReadSample(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &flags, nullptr, &sample)",
					L"AudioResource::AudioResource()"
					);

				if (sample != nullptr)
				{
					ThrowIfFailed(
						sample->ConvertToContiguousBuffer(&mediaBuffer),
						L"サンプラーのコンバートに失敗しました",
						L"sample->ConvertToContiguousBuffer(&mediaBuffer)",
						L"AudioResource::AudioResource()"
						);

					BYTE *audioData = nullptr;
					DWORD sampleBufferLength = 0;

					ThrowIfFailed(
						mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength),
						L"バッファのLockに失敗しました",
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
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct MeshResource::Impl{
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//頂点バッファ
		ComPtr<ID3D11Buffer> m_IndexBuffer;	//インデックスバッファ
		UINT m_NumVertices;				//頂点の数
		UINT m_NumIndicis;				//インデックスの数

	};

	//--------------------------------------------------------------------------------------
	//	class MeshResource : public BaseResource;
	//	用途: プリミティブメッシュクラス
	//--------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------
	//	MeshResource();
	//	用途: コンストラクタ
	//	戻り値: なし
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

	//アクセッサ
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
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//頂点バッファ
		ComPtr<ID3D11Buffer> m_IndexBuffer;	//インデックスバッファ
		UINT m_NumVertices;				//頂点の数
		UINT m_NumIndicis;				//インデックスの数
	};

	//--------------------------------------------------------------------------------------
	//	struct CommonMeshResource::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct CommonMeshResource::Impl{
		vector<VertexPositionNormalTexture> m_BackupVertices;
		//ミューテックス
		std::mutex Mutex;
		//--------------------------------------------------------------------------------------
		//	void CreateBuffers(
		//	vector<VertexPositionNormalTexture>& vertices, //頂点を作成するための配列
		//	vector<uint16_t>& indices,	//インデックスを作成するための配列
		//	bool AccessWrite		//書き込みOKかどうか
		//	VertexParams& RetParam
		//	);
		//	用途: バッファを作成する
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		void CreateBuffers(const vector<VertexPositionNormalTexture>& vertices,const vector<uint16_t>& indices, bool AccessWrite, VertexParams& RetParam);
	};

	void CommonMeshResource::Impl::CreateBuffers(const vector<VertexPositionNormalTexture>& vertices,const vector<uint16_t>& indices, bool AccessWrite,
		VertexParams& RetParam){
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		//バッファの作成
		if (AccessWrite){
			Util::DemandCreate(RetParam.m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				//頂点バッファの作成
				VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, vertices, pResult);
			});
		}
		else{
			Util::DemandCreate(RetParam.m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				//頂点バッファの作成
				VertexUtil::CreatePrimitiveBuffer(pDx11Device, vertices, D3D11_BIND_VERTEX_BUFFER, pResult);
			});
		}
		//頂点数の設定
		RetParam.m_NumVertices = static_cast<UINT>(vertices.size());
		Util::DemandCreate(RetParam.m_IndexBuffer, Mutex, [&](ID3D11Buffer** pResult)
		{
			//インデックスバッファの作成
			VertexUtil::CreatePrimitiveBuffer(pDx11Device, indices, D3D11_BIND_INDEX_BUFFER, pResult);
		});
		//インデックス数の設定
		RetParam.m_NumIndicis = static_cast<UINT>(indices.size());
	}





	//--------------------------------------------------------------------------------------
	//	class CommonMeshResource : public MeshResource;
	//	用途: プリミティブメッシュクラス
	//--------------------------------------------------------------------------------------
	//構築
	CommonMeshResource::CommonMeshResource():
		MeshResource(),
		pImpl(new Impl())
	{}
	//破棄
	CommonMeshResource::~CommonMeshResource(){}

	//リソース構築
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

			//配列をもとに頂点とインデックスを作成
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, Indices, AccessWrite, Params);
			//書き込み不可の場合はバックアップを削除
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

			//インデックスを作成するための配列
			vector<uint16_t> indices{
				(uint16_t)0,
				(uint16_t)1,
				(uint16_t)2,
				(uint16_t)1,
				(uint16_t)3,
				(uint16_t)2,
			};

			VertexParams Params;

			//配列をもとに頂点とインデックスを作成
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//書き込み不可の場合はバックアップを削除
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

			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Cubeの作成(ヘルパー関数を利用)
			VertexUtil::CreateCube(Size, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//バッファを作成
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//書き込み不可の場合はバックアップを削除
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

			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Sphereの作成(ヘルパー関数を利用)
			VertexUtil::CreateSphere(Diameter, Tessellation, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//バッファを作成
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//書き込み不可の場合はバックアップを削除
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

			//インデックスを作成するための配列
			vector<uint16_t> indices;

			Vector3 PointA(0, -Height / 2.0f, 0);
			Vector3 PointB(0, Height / 2.0f, 0);
			//Capsuleの作成(ヘルパー関数を利用)
			VertexUtil::CreateCapsule(Diameter, PointA, PointB, Tessellation, Ptr->pImpl->m_BackupVertices, indices);

			VertexParams Params;
			//バッファを作成
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//書き込み不可の場合はバックアップを削除
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

			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Cylinderの作成(ヘルパー関数を利用)
			VertexUtil::CreateCylinder(Height,Diameter,Tessellation, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//バッファを作成
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//書き込み不可の場合はバックアップを削除
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

			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Coneの作成(ヘルパー関数を利用)
			VertexUtil::CreateCone(Diameter, Height, Tessellation, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//バッファを作成
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//書き込み不可の場合はバックアップを削除
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

			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Torusの作成(ヘルパー関数を利用)
			VertexUtil::CreateTorus(Diameter, Thickness, Tessellation, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//バッファを作成
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//書き込み不可の場合はバックアップを削除
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

			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Tetrahedronの作成(ヘルパー関数を利用)
			VertexUtil::CreateTetrahedron(Size, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//バッファを作成
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//書き込み不可の場合はバックアップを削除
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

	//正8面体
	shared_ptr<CommonMeshResource> CommonMeshResource::CreateOctahedron(float Size, bool AccessWrite){
		try{
			shared_ptr<CommonMeshResource> Ptr(new CommonMeshResource());

			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Octahedronの作成(ヘルパー関数を利用)
			VertexUtil::CreateOctahedron(Size, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//バッファを作成
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//書き込み不可の場合はバックアップを削除
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

			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Dodecahedronの作成(ヘルパー関数を利用)
			VertexUtil::CreateDodecahedron(Size, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//バッファを作成
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//書き込み不可の場合はバックアップを削除
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

			//インデックスを作成するための配列
			vector<uint16_t> indices;
			//Icosahedronの作成(ヘルパー関数を利用)
			VertexUtil::CreateIcosahedron(Size, Ptr->pImpl->m_BackupVertices, indices);
			VertexParams Params;
			//バッファを作成
			Ptr->pImpl->CreateBuffers(Ptr->pImpl->m_BackupVertices, indices, AccessWrite, Params);
			//書き込み不可の場合はバックアップを削除
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
			// Map失敗
			throw BaseException(
				L"変更する頂点の数がバックアップと違います",
				L"if (NewBuffer.size() != pImpl->m_BackupVertices.size())",
				L"CommonMeshResource::UpdateVirtexBuffer()"
				);
		}
		//座標を変更する
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//頂点バッファをリソースから取り出す
		auto pVertexBuffer = GetVertexBuffer().Get();

		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))){
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"CommonMeshResource::UpdateVirtexBuffer()"
				);
		}
		//頂点の変更
		VertexPositionNormalTexture* vertices = (VertexPositionNormalTexture*)mappedBuffer.pData;
		for (size_t i = 0; i < NewBuffer.size(); i++){
			vertices[i] = NewBuffer[i];
		}
		//アンマップ
		pID3D11DeviceContext->Unmap(pVertexBuffer, 0);

	}





	//--------------------------------------------------------------------------------------
	//	struct FbxMeshResource::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct FbxMeshResource::Impl{
		unique_ptr<FbxMesh, FbxMeshDeleter> m_FbxMesh;
		//FBXシーン
		weak_ptr<FbxSceneResource> m_FbxSceneResource;

		//FBX名（空白の場合あり）
		string m_FbxName;
		//マテリアル情報の数
		DWORD m_MaterialCount;
		//マテリアルの配列
		vector<Material> m_MaterialVec;
		//テクスチャの配列（テクスチャはこちらで削除）
		vector< shared_ptr<TextureResource> > m_TextureVec;
		//以下、スキンメッシュ用
		//スキンメッシュの場合nullptr以外になる
		unique_ptr<FbxSkin, FbxSkinDeleter> m_FbxSkin;
		//スキンメッシュでも強制的にスタティックで読むかどうか
		bool m_NeedStatic;
		//１フレームの基準となる時間
		FbxTime m_timePeriod;
		//スキンアニメーションに使用するボーンの数
		UINT m_NumBones;
		//スキンアニメーションに使用するボーン
		//ボーンの配列
		vector< Bone > m_vecBones;
		//ボーンを名前で照会するする際に使用するインデックステーブル
		map< string, UINT > m_mapBoneList;
		//アニメーションを名前で照会するする際に使用するインデックステーブル
		map< string, AnimationData > m_AnimationMap;
		//ミューテックス
		std::mutex Mutex;
		//--------------------------------------------------------------------------------------
		//	void CreateBuffers(
		//	vector<VertexPositionNormalTexture>& vertices, //頂点を作成するための配列
		//	vector<uint16_t>& indices,	//インデックスを作成するための配列
		//	bool AccessWrite		//書き込みOKかどうか
		//	VertexParams& RetParam
		//	);
		//	用途: バッファを作成する
		//	戻り値: なし
		//--------------------------------------------------------------------------------------
		template<typename T>
		void CreateBuffers(vector<T>& vertices, vector<uint16_t>& indices, bool AccessWrite, VertexParams& RetParam){
			//デバイスの取得
			auto Dev = App::GetApp()->GetDeviceResources();
			ID3D11Device* pDx11Device = Dev->GetD3DDevice();
			//バッファの作成
			if (AccessWrite){
				Util::DemandCreate(RetParam.m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					//頂点バッファの作成
					VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, vertices, pResult);
				});
			}
			else{
				Util::DemandCreate(RetParam.m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
				{
					//頂点バッファの作成
					VertexUtil::CreatePrimitiveBuffer(pDx11Device, vertices, D3D11_BIND_VERTEX_BUFFER, pResult);
				});
			}
			//頂点数の設定
			RetParam.m_NumVertices = static_cast<UINT>(vertices.size());
			Util::DemandCreate(RetParam.m_IndexBuffer, Mutex, [&](ID3D11Buffer** pResult)
			{
				//インデックスバッファの作成
				VertexUtil::CreatePrimitiveBuffer(pDx11Device, indices, D3D11_BIND_INDEX_BUFFER, pResult);
			});
			//インデックス数の設定
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
	//	用途: FBXメッシュリソースクラス
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FbxMeshResource::FbxMeshResource(
		shared_ptr<FbxSceneResource> FbxSceneResourcePtr,
		FbxMesh* pFbxMesh,
		bool NeedStatic
		) :
		MeshResource(),
		pImpl(new Impl(FbxSceneResourcePtr, pFbxMesh, NeedStatic))
	{}
	FbxMeshResource::~FbxMeshResource(){}

	//初期化

	//--------------------------------------------------------------------------------------
	void FbxMeshResource::CreateInstanceFromStaticFbx(){
		VertexParams Params;
		//メッシュ単体の読み込み
		DWORD dwNumPolygons = 0;	//ポリゴン数
		//頂点がない	
		if ((Params.m_NumVertices = pImpl->m_FbxMesh->GetControlPointsCount()) <= 0) {
			//失敗した
			throw BaseException(L"Fbxに頂点がありません",
				L"m_FbxMesh->GetControlPointsCount() <= 0",
				L"FbxMeshResource::Create()");
		}
		//このFBXに名前があればそれを保持
		if (pImpl->m_FbxMesh->GetName()){
			pImpl->m_FbxName = pImpl->m_FbxMesh->GetName();
		}
		//ポリゴン数の取得
		dwNumPolygons = pImpl->m_FbxMesh->GetPolygonCount();

		//頂点を作成するための配列
		vector<VertexPositionNormalTexture> vertices(Params.m_NumVertices);

		FbxStringList sUVSetNames;
		pImpl->m_FbxMesh->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//頂点座標・法線・テクスチャ座標の取得
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//ポリゴンのサイズを得る（通常３）
			const DWORD dwPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = pImpl->m_FbxMesh->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbxから頂点を得る
				vPos = pImpl->m_FbxMesh->GetControlPointAt(iIndex);
				//法線を得る
				pImpl->m_FbxMesh->GetPolygonVertexNormal(i, j, vNormal);
				//UV値を得る
				pImpl->m_FbxMesh->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				vertices[iIndex] =
					VertexPositionNormalTexture(
					//頂点の設定
					//Z座標がFbxとは符号が逆になる（DirectXは左手座標系）
					XMFLOAT3(static_cast< float >(vPos[0]), static_cast< float >(vPos[1]), -static_cast< float >(vPos[2])),
					//法線の設定
					//Z座標がFbxとは符号が逆になる（DirectXは左手座標系）
					XMFLOAT3(static_cast< float >(vNormal[0]), static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
					//UV値の設定
					//Vの値が、1.0から引いた値になる
					XMFLOAT2(static_cast< float >(vUV[0]), 1.0f - static_cast< float >(vUV[1]))
					);
			}
		}

		//マテリアルの数を取得する(一つのメッシュに対し、複数のマテリアルが割り当てられていることがある)
		pImpl->m_MaterialCount = pImpl->m_FbxMesh->GetNode()->GetMaterialCount();
		//マテリアルの読み込み
		CreateMaterial();


		//インデックスを作成するための配列
		vector<uint16_t> indices;
		//マテリアルのポインタを取得する
		const FbxLayerElementMaterial*	fbxMaterial = pImpl->m_FbxMesh->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < pImpl->m_MaterialCount; i++) {
			//頂点インデックスを最適化する(同じマテリアルを使用するポリゴンをまとめて描画できるように並べ、
			//描画時にマテリアルの切り替え回数を減らす)
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

		//マテリアル配列にスタート地点を設定
		UINT StarIndex = 0;
		for (DWORD i = 0; i < pImpl->m_MaterialVec.size(); i++) {
			pImpl->m_MaterialVec[i].m_StartIndex = StarIndex;
			StarIndex += pImpl->m_MaterialVec[i].m_IndexCount;
		}

		//配列をもとに頂点とインデックスを作成
		pImpl->CreateBuffers(vertices, indices,false,Params);

		SetVertexBuffer(Params.m_VertexBuffer);
		SetIndexBuffer(Params.m_IndexBuffer);
		SetNumVertices(Params.m_NumVertices);
		SetNumIndicis(Params.m_NumIndicis);

	}

	//--------------------------------------------------------------------------------------
	void FbxMeshResource::CreateInstanceFromSkinFbx(){
		VertexParams Params;

		//メッシュ単体の読み込み
		DWORD dwNumPolygons = 0;	//ポリゴン数
		//頂点がない	
		if ((Params.m_NumVertices = pImpl->m_FbxMesh->GetControlPointsCount()) <= 0) {
			//失敗した
			throw BaseException(L"Fbxに頂点がありません",
				L"m_pFbxMesh->GetControlPointsCount() <= 0",
				L"FbxMeshResource::CreateInstanceFromSkinFbx()");
		}
		//このFBXに名前があればそれを保持
		if (pImpl->m_FbxMesh->GetName()){
			pImpl->m_FbxName = pImpl->m_FbxMesh->GetName();
		}
		//ポリゴン数の取得
		dwNumPolygons = pImpl->m_FbxMesh->GetPolygonCount();

		//頂点を作成するための配列
		//ブレンドウェイトとブレンドインデックスがある
		vector<VertexPositionNormalTextureSkinning> vertices(Params.m_NumVertices);

		FbxStringList sUVSetNames;
		pImpl->m_FbxMesh->GetUVSetNames(sUVSetNames);
		FbxString sUVSetName = sUVSetNames.GetStringAt(0);
		bool bUnmapped = true;
		//頂点座標・法線・テクスチャ座標の取得
		for (DWORD i = 0; i < dwNumPolygons; i++) {
			//ポリゴンのサイズを得る（通常３）
			const DWORD dwPolygonSize = pImpl->m_FbxMesh->GetPolygonSize(i);
			for (DWORD j = 0; j < dwPolygonSize; j++) {
				const int	iIndex = pImpl->m_FbxMesh->GetPolygonVertex(i, j);
				FbxVector4	vPos, vNormal;
				FbxVector2	vUV;
				//Fbxから頂点を得る
				vPos = pImpl->m_FbxMesh->GetControlPointAt(iIndex);
				//法線を得る
				pImpl->m_FbxMesh->GetPolygonVertexNormal(i, j, vNormal);
				//UV値を得る
				pImpl->m_FbxMesh->GetPolygonVertexUV(i, j, sUVSetName, vUV, bUnmapped);
				uint32_t temp[4] = { 0, 0, 0, 0 };
				float tempf[4] = { 0, 0, 0, 0 };
				vertices[iIndex] =
					VertexPositionNormalTextureSkinning(
					//頂点の設定
					//Z座標がFbxとは符号が逆になる（DirectXは左手座標系）
					XMFLOAT3(static_cast< float >(vPos[0]), static_cast< float >(vPos[1]), -static_cast< float >(vPos[2])),
					//法線の設定
					//Z座標がFbxとは符号が逆になる（DirectXは左手座標系）
					XMFLOAT3(static_cast< float >(vNormal[0]), static_cast< float >(vNormal[1]), -static_cast< float >(vNormal[2])),
					//UV値の設定
					//Vの値が、1.0から引いた値になる
					XMFLOAT2(static_cast< float >(vUV[0]), 1.0f - static_cast< float >(vUV[1])),
					//ブレンドインデックスはとりあえず0
					temp,
					//ブレンドウエイトはとりあえず0
					tempf
					);
			}
		}

		//ブレンドウェイトとブレンドインデックスの読み込み
		const int	iNumCluster = pImpl->m_FbxSkin->GetClusterCount();
		// 変換した FbxSkin から クラスター(ボーン)の数を取得する

		for (int i = 0; i < iNumCluster; i++) {
			int		iNumBlendIndices = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointIndicesCount();	// i 番目のクラスターに影響を受ける頂点の数を取得する
			int*	piBlendIndices = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointIndices();		// i 番目のクラスターに影響を受ける頂点の番号を配列で取得する
			double*	pdBlendWeights = pImpl->m_FbxSkin->GetCluster(i)->GetControlPointWeights();		// i 番目のクラスターに影響を受ける頂点に対応した重みデータを配列で取得する

			for (int j = 0; j < iNumBlendIndices; j++) {
				int	idx = piBlendIndices[j];

				//頂点に登録する４つのブレンドウェイトのうち、最少の値をもつ要素を検索する（DirectX9の固定シェーダでは４つのボーンからしかブレンドできない）
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
				//すでに登録されている中で最小のブレンドウェイトよりも大きい値を持つデータを登録する
				if (static_cast< float >(pdBlendWeights[j]) > vertices[idx].weights[iMinIndex]) {
					vertices[idx].indices[iMinIndex] = static_cast< BYTE >(i);
					vertices[idx].weights[iMinIndex] = static_cast< float >(pdBlendWeights[j]);
				}
			}
		}
		//ウエイトのチェック
		//各頂点ごとにウェイトを足して1.0fにならないとスキンがうまくできない
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

		//基準タイマーの設定
		FbxGlobalSettings&	globalTimeSettings = pImpl->m_FbxMesh->GetScene()->GetGlobalSettings();
		FbxTime::EMode timeMode = globalTimeSettings.GetTimeMode();
		pImpl->m_timePeriod.SetTime(0, 0, 0, 1, 0, timeMode);

		//マテリアルの数を取得する(一つのメッシュに対し、複数のマテリアルが割り当てられていることがある)
		pImpl->m_MaterialCount = pImpl->m_FbxMesh->GetNode()->GetMaterialCount();
		//マテリアルの読み込み
		CreateMaterial();

		//インデックスバッファの作成
		//インデックスを作成するための配列
		vector<uint16_t> indices;
		//マテリアルのポインタを取得する
		const FbxLayerElementMaterial*	fbxMaterial = pImpl->m_FbxMesh->GetLayer(0)->GetMaterials();
		DWORD dwIndexCount = 0;
		for (DWORD i = 0; i < pImpl->m_MaterialCount; i++) {
			//頂点インデックスを最適化する(同じマテリアルを使用するポリゴンをまとめて描画できるように並べ、
			//描画時にマテリアルの切り替え回数を減らす)
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

		//マテリアル配列にスタート地点を設定
		UINT StarIndex = 0;
		for (DWORD i = 0; i < pImpl->m_MaterialVec.size(); i++) {
			pImpl->m_MaterialVec[i].m_StartIndex = StarIndex;
			StarIndex += pImpl->m_MaterialVec[i].m_IndexCount;
		}


		//バッファの作成
		pImpl->CreateBuffers(vertices, indices, false, Params);


		//ボーン数を得る
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
			//マップの登録
			pImpl->m_mapBoneList[pImpl->m_FbxSkin->GetCluster(i)->GetName()] = i;
		}
		SetVertexBuffer(Params.m_VertexBuffer);
		SetIndexBuffer(Params.m_IndexBuffer);
		SetNumVertices(Params.m_NumVertices);
		SetNumIndicis(Params.m_NumIndicis);

	}

	//--------------------------------------------------------------------------------------
	void FbxMeshResource::CreateMaterial(){
		//マテリアルの設定
		//テクスチャファイル名作成のためのワーク配列
		wchar_t Buff[MAX_PATH];
		setlocale(LC_CTYPE, "jpn");

		if (pImpl->m_FbxSceneResource.expired()){
			//失敗した
			throw BaseException(
				L"FbxSceneResourceが無効です",
				L"if (pImpl->m_FbxSceneResource.expired())",
				L"FbxMeshResource::CreateMaterial()");

		}
		auto FbxSceneResourcePtr = pImpl->m_FbxSceneResource.lock();

		for (DWORD i = 0; i < pImpl->m_MaterialCount; i++) {
			//マテリアル取得
			Material material;
			::ZeroMemory(&material, sizeof(Material));

			FbxSurfaceMaterial*			pMaterial = pImpl->m_FbxMesh->GetNode()->GetMaterial(i);
			FbxSurfacePhong*			pPhong = (FbxSurfacePhong*)pMaterial;
			FbxPropertyT<FbxDouble3>	color;
			color = pPhong->Ambient;
			//アンビエントは使用しない
			material.m_Ambient = Color4(0.5f, 0.5f, 0.5f, 1.0f);
			//			material.m_Ambient = Color4((float)color.Get()[ 0 ], (float)color.Get()[ 1 ], (float)color.Get()[ 2 ], 1.0f);
			color = pPhong->Diffuse;
			//デフィーズはテクスチャを使うのでフル
			//			material.m_Diffuse = Color4(1.0f,1.0f,1.0f,1.0f);
			material.m_Diffuse = Color4((float)color.Get()[0], (float)color.Get()[1], (float)color.Get()[2], 1.0f);
			//			material.m_Specular = Color4(0.0f,0.0f,0.0f,0.4f);
			material.m_Specular = Color4((float)color.Get()[0] * 0.5f, (float)color.Get()[1] * 0.5f, (float)color.Get()[2] * 0.5f, 0.4f);
			color = pPhong->Emissive;
			//エミッシブはDirectXTKデフォルト
			//			material.m_Emissive = Color4(0.05333332f,0.09882354f,0.1819608f,1.0f);
			material.m_Emissive = Color4((float)color.Get()[0], (float)color.Get()[1], (float)color.Get()[2], 1.0f);
			//マテリアルに関連付けられているテクスチャを読み込む
			const FbxProperty	fbxProperty = pMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
			//テクスチャからファイル情報を抜き出す
			FbxFileTexture*	pFbxFileTexture = fbxProperty.GetSrcObject< FbxFileTexture >(i);
			TextureResource* pTexture = 0;
			if (pFbxFileTexture) {
				//テクスチャファイル名からパスを排除しファイル名+拡張子として合成する
				char szTextureFilename[256], szFileExt[8];
				_splitpath_s(pFbxFileTexture->GetFileName(), nullptr, 0, nullptr, 0, szTextureFilename, 256, szFileExt, 8);
				//ファイル名の合成
				string sTextureFilename(szTextureFilename);
				sTextureFilename += szFileExt;
				//UNIコードに変換
				size_t size = 0;
				mbstowcs_s(&size, Buff, sTextureFilename.c_str(), MAX_PATH - 1);
				wstring strWork = FbxSceneResourcePtr->GetDataDir() + Buff;
				//pFbxFileTextureからラップモードを取得してテクスチャを作成
				auto PtrTexture = make_shared<TextureResource>(strWork.c_str());
				pImpl->m_TextureVec.push_back(PtrTexture);
				material.m_Texture = PtrTexture;
			}
			else{
				if (pImpl->m_FbxSkin){
					throw BaseException(
						L"スキンメッシュのテクスチャがありません。\nスキンメッシュはテクスチャ無しは対応してません。",
						L"if ( !pFbxFileTexture )",
						L"FbxMeshResource::CreateMaterial()"
						);
				}
				//テクスチャがない場合は0を設定
				material.m_Texture = nullptr;
			}
			//マテリアル配列に追加
			pImpl->m_MaterialVec.push_back(material);
		}
	}

	void FbxMeshResource::Create(){
		try{
			//スキンメッシュかどうかを判断
			pImpl->m_FbxSkin.reset(FbxCast< FbxSkin >(pImpl->m_FbxMesh->GetDeformer(0, FbxDeformer::eSkin)));
			//スキンメッシュの場合
			if (pImpl->m_FbxSkin && !pImpl->m_NeedStatic){
				CreateInstanceFromSkinFbx();
			}
			//スタティックメッシュもしくは強制的にスタティックで読む場合
			else{
				pImpl->m_FbxSkin = nullptr;
				CreateInstanceFromStaticFbx();
			}
		}
		catch (...){
			throw;
		}
	}

	//アクセサ
	//スキニングするかどうか
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
				"指定のアニメーションが見つかりません",
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
		//経過時間からフレームを求める
		float fCurrentFrame = CurrentTime * rAnimationData.m_FramesParSecond;
		//アニメーションが最後までいってるかどうかを確認
		bool ret = false;
		if (!rAnimationData.m_IsLoop && ((UINT)fCurrentFrame >= rAnimationData.m_FrameLength)){
			//ループしないで、アニメが最後に到達したら
			ret = true;
		}
		int iAnimationFrame = static_cast< int >(fCurrentFrame * 10000.0f);

		if (rAnimationData.m_FrameLength > 0) {
			if (rAnimationData.m_IsLoop){
				//ループする
				iAnimationFrame %= rAnimationData.m_FrameLength * 10000;
				iAnimationFrame += rAnimationData.m_StartFrame * 10000;
			}
			else{
				//ループしない
				if (ret){
					//最後のフレームにポーズを設定する
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
		mMirror(2, 2) = -1.0f;	//座標系の関係でボーンが反転しているので、それをDirectXに合わせるために使用する行列

		//カレント行列と合成行列を再計算する
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
				//指定の名前が見つかった
				//そのデータに差し替え
				it->second.m_StartFrame = StartFrame;
				it->second.m_FrameLength = FrameLength;
				it->second.m_FramesParSecond = FramesParSecond;
				it->second.m_IsLoop = Loop;
			}
			else{
				//見つからない
				//アニメーション定義の追加
				pImpl->m_AnimationMap[Name] = AnimationData(StartFrame, FrameLength,
					Loop, FramesParSecond);
			}
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct FbxSceneResource::Impl;
	//	用途: Implイディオム
	//--------------------------------------------------------------------------------------
	struct FbxSceneResource::Impl{
		//データディレクトリ名
		wstring m_DataDir;
		//FBXファイル名
		wstring m_FileName;
		//FBXシーン名
		string m_FbxSceneName;
		//FBXシーン
		shared_ptr<FbxScene> m_FbxScene;
		//強制的にstaticで読むかどうか
		bool m_NeedStatic;
		//単一のFBXメッシュの配列
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
	//	用途: FBXシーンリソースクラス
	//	＊一つのFBXファイルは１つのFbxSceneResourceとして管理する
	//--------------------------------------------------------------------------------------
	//構築と破棄
	FbxSceneResource::FbxSceneResource(const wstring& DataDir,
		const wstring& FileName, const string& SceneName, bool NeedStatic) :
		pImpl(new Impl(DataDir, FileName, SceneName, NeedStatic))
	{
		try{
			if (FileName == L""){
				//失敗した
				throw BaseException(
					L"Fbxファイル名が無効です",
					L"if(FileName == \"\")",
					L"FbxSceneResource::FbxSceneResource()");
			}

			if (pImpl->m_FbxSceneName == ""){
				if (FileName == L""){
					//失敗した
					throw BaseException(
						L"ファイル名が無効です",
						L"if(FileName == \"\")",
						L"FbxSceneResource::FbxSceneResource()");
				}
				wstring strWork;
				strWork = pImpl->m_FileName;
				//ファイル名をマルチバイトに変換しシーン名を作成
				Util::WStoMB(strWork, pImpl->m_FbxSceneName);
				pImpl->m_FbxSceneName += ".scene";
			}
		}
		catch (...){
			throw;
		}
	}
	//static構築
	shared_ptr<FbxSceneResource> FbxSceneResource::CreateFbxScene(const wstring& DataDir,
		const wstring& FileName, const string& SceneName, bool NeedStatic){
		shared_ptr<FbxSceneResource> ScenePtr(new FbxSceneResource(DataDir, FileName, SceneName, NeedStatic));
		ScenePtr->Create();
		return ScenePtr;
	}
	FbxSceneResource::~FbxSceneResource(){}

	//再帰的にシーンを読み込む
	void FbxSceneResource::CreateFbxMeshVector(FbxNode* pFbxNode){
		//ノードが有効でなければリターン
		if (!pFbxNode){
			return;
		}
		//FBXシーンの取得
		FbxScene* pScene = pFbxNode->GetScene();
		//ノードの属性を取得
		FbxNodeAttribute* pFbxNodeAttribute = pFbxNode->GetNodeAttribute();
		//ノードの属性が有効であれば、属性のタイプを判定し、タイプがメッシュであればvectorに登録する
		if (pFbxNodeAttribute){
			if (pFbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
				//三角分割
				FbxGeometryConverter converter(pFbxNode->GetFbxManager());
				converter.Triangulate(pScene, true);
				//メッシュを取り出す
				FbxMesh* pFbxMesh = pFbxNode->GetMesh();
				//シーンのグローバル行列を取得
				FbxAnimEvaluator* pFbxAnimEvaluator = pScene->GetAnimationEvaluator();
				FbxMatrix mGlobal = pFbxAnimEvaluator->GetNodeGlobalTransform(pFbxNode);
				//グローバル行列をすべての頂点に適用
				int	iNumControlPoints = pFbxMesh->GetControlPointsCount();
				FbxVector4*	v = pFbxMesh->GetControlPoints();
				for (int i = 0; i < iNumControlPoints; i++){
					v[i] = mGlobal.MultNormalize(v[i]);
				}
				//Fbxメッシュの配列に登録
				auto FbxMeshPtr = make_shared<FbxMeshResource>(GetThis<FbxSceneResource>(), pFbxMesh,pImpl->m_NeedStatic);
				FbxMeshPtr->Create();
				pImpl->m_FbxMeshResourceVec.push_back(
					FbxMeshPtr
					);
			}
		}
		//子ノードの数を取得する
		int iNumChild = pFbxNode->GetChildCount();
		for (int i = 0; i < iNumChild; i++) {
			//子ノードすべてを再帰的にメッシュを登録する
			CreateFbxMeshVector(pFbxNode->GetChild(i));
		}
	}
	//初期化
	void FbxSceneResource::Create(){
		try{

			//インポーターを作成する
			FbxImporter* lImporter = FbxImporter::Create(App::GetApp()->GetFbxManager().get(), "");
			//ファイル名をマルチバイトにする
			wstring strWork;
			strWork = pImpl->m_DataDir + pImpl->m_FileName;
			//マルチバイトに変換
			string sFileName;
			Util::WStoMB(strWork, sFileName);
			//インポーターの初期化
			if (!lImporter->Initialize(sFileName.c_str(), -1, App::GetApp()->GetFbxManager().get()->GetIOSettings())) {
				//失敗した
				throw BaseMBException(
					"インポータの初期化に失敗しました",
					lImporter->GetStatus().GetErrorString(),
					"FbxSceneResource::FbxSceneResource()");
			}
			//シーンの作成
			pImpl->m_FbxScene = shared_ptr<FbxScene>(
				FbxScene::Create(App::GetApp()->GetFbxManager().get(), pImpl->m_FbxSceneName.c_str()),	//生ポインタ
				FbxSceneDeleter()	//カスタムデリータ
				);
			//インポータによるシーンへの読み込み
			lImporter->Import(pImpl->m_FbxScene.get());
			//インポータはもう必要ない
			lImporter->Destroy();


			//再帰的に読み込む
			CreateFbxMeshVector(pImpl->m_FbxScene->GetRootNode());


		}
		catch (...){
			throw;
		}
	}



	//アクセサ
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
				L"インデックスが範囲外です",
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

#include "stdafx.h"
#include "resource.h"
#include "GameObjects.h"
namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CBTexture3D : public ConstantBuffer<CBTexture3D,Texture3DConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<CBTexture3D, CBTexture3D::Deleter> CBTexture3D::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSTexture3D : public VertexShader<VSTexture3D, VertexPositionNormalTexture>;
	//	用途:  VSTexture3D頂点シェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<VSTexture3D, VSTexture3D::Deleter> VSTexture3D::m_Ptr;
	//構築
	VSTexture3D::VSTexture3D() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSTexture3D.cso"){}

	//--------------------------------------------------------------------------------------
	//	class PSTexture3D : public PixelShader<PSTexture3D>;
	//	用途: PSTexture3Dピクセルシェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<PSTexture3D, PSTexture3D::Deleter> PSTexture3D::m_Ptr;
	//構築
	PSTexture3D::PSTexture3D() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSTexture3D.cso")
	{
	}

	GameObject::GameObject(const shared_ptr<GameStage>& GStage,
		const Vector3& StartScale,
		const Vector3& StartRotation,
		const Vector3& StartPos) :
		m_GameStgae(GStage),
		m_Scale(StartScale),
		m_Quaternion(),
		m_Position(StartPos)
	{
		m_Quaternion.RotationRollPitchYawFromVector(StartRotation);
	}

	struct VertexPositionNormalTexturePOD{
		float position[3];
		float normal[3];
		float textureCoordinate[2];
	};

	struct MaterialExPOD{
		//!開始インデックス
		UINT m_StartIndex;
		//!描画インデックスカウント
		UINT m_IndexCount;
		//! デフィーズ（物体の色）
		float m_Diffuse[4];
		//! スペキュラー（反射光）
		float m_Specular[4];
		//! アンビエント（環境色）
		float m_Ambient[4];
		//! エミッシブ（放射光）
		float m_Emissive[4];
	};


	//メッシュデータの読み込み
	void GameObject::ReadMesh(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices, vector<MaterialEx>& materials){
		auto MeshFileName = App::GetApp()->m_wstrRelativeDataPath + L"Chara_R.bin";
		BinaryReader Reader(MeshFileName);
		//ヘッダの読み込み
		auto pHeader = Reader.ReadArray<char>(16);
		string str = pHeader;
		if (str != "BDV1.0"){
			throw BaseException(
				L"データ形式が違います",
				MeshFileName,
				L"GameObject::ReadMesh()"
				);
		}
		//頂点の読み込み
		auto blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::Vertex){
			throw BaseException(
				L"頂点のヘッダが違います",
				MeshFileName,
				L"GameObject::ReadMesh()"
				);
		}
		auto VerTexSize = blockHeader.m_Size / sizeof(VertexPositionNormalTexturePOD);
		auto pVertex = Reader.ReadArray<VertexPositionNormalTexturePOD>((size_t)VerTexSize);
		for (UINT i = 0; i < VerTexSize; i++){
			VertexPositionNormalTexture v;
			v.position.x = pVertex[i].position[0];
			v.position.y = pVertex[i].position[1];
			v.position.z = pVertex[i].position[2];
			v.normal.x = pVertex[i].normal[0];
			v.normal.y = pVertex[i].normal[1];
			v.normal.z = pVertex[i].normal[2];
			v.textureCoordinate.x = pVertex[i].textureCoordinate[0];
			v.textureCoordinate.y = pVertex[i].textureCoordinate[1];
			vertices.push_back(v);
		}

		//インデックスの読み込み
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::Index){
			throw BaseException(
				L"インデックスのヘッダが違います",
				MeshFileName,
				L"GameObject::ReadMesh()"
				);
		}

		auto IndexSize = blockHeader.m_Size / sizeof(uint16_t);
		auto pIndex = Reader.ReadArray<uint16_t>((size_t)IndexSize);
		for (UINT i = 0; i < IndexSize; i++){
			indices.push_back(pIndex[i]);
		}

		//マテリアルの読み込み
		//マテリアル数の読み込み
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::MaterialCount){
			throw BaseException(
				L"マテリアル数のヘッダが違います",
				MeshFileName,
				L"GameObject::ReadMesh()"
				);
		}
		UINT MaterialCount = blockHeader.m_Size;
		for (UINT i = 0; i < MaterialCount; i++){
			//テクスチャファイル名が可変長なので注意。
			blockHeader = Reader.Read<BlockHeader>();
			if (blockHeader.m_Type != BlockType::Material){
				throw BaseException(
					L"マテリアルのヘッダが違います",
					MeshFileName,
					L"GameObject::ReadMesh()"
					);
			}
			UINT TextureFileNameSize = blockHeader.m_Size - sizeof(MaterialExPOD);
			auto rMaterial = Reader.Read<MaterialExPOD>();
			MaterialEx ToM;
			//!開始インデックス
			ToM.m_StartIndex = rMaterial.m_StartIndex;
			//!描画インデックスカウント
			ToM.m_IndexCount = rMaterial.m_IndexCount;
			//! デフィーズ（物体の色）
			ToM.m_Diffuse.x = rMaterial.m_Diffuse[0];
			ToM.m_Diffuse.y = rMaterial.m_Diffuse[1];
			ToM.m_Diffuse.z = rMaterial.m_Diffuse[2];
			ToM.m_Diffuse.w = rMaterial.m_Diffuse[3];
			//! スペキュラー（反射光）
			ToM.m_Specular.x = rMaterial.m_Specular[0];
			ToM.m_Specular.y = rMaterial.m_Specular[1];
			ToM.m_Specular.z = rMaterial.m_Specular[2];
			ToM.m_Specular.w = rMaterial.m_Specular[3];
			//! アンビエント（環境色）
			ToM.m_Ambient.x = rMaterial.m_Ambient[0];
			ToM.m_Ambient.y = rMaterial.m_Ambient[1];
			ToM.m_Ambient.z = rMaterial.m_Ambient[2];
			ToM.m_Ambient.w = rMaterial.m_Ambient[3];
			//! エミッシブ（放射光）
			ToM.m_Emissive.x = rMaterial.m_Emissive[0];
			ToM.m_Emissive.y = rMaterial.m_Emissive[1];
			ToM.m_Emissive.z = rMaterial.m_Emissive[2];
			ToM.m_Emissive.w = rMaterial.m_Emissive[3];
			auto pTexture = Reader.ReadArray<wchar_t>(TextureFileNameSize / sizeof(wchar_t));
			wstring TextureFileStr = pTexture;
			TextureFileStr = App::GetApp()->m_wstrRelativeDataPath + TextureFileStr;
			ToM.m_ShaderResView = CreateShaderResView(TextureFileStr);
			materials.push_back(ToM);
		}

		//Endの読み込み
		blockHeader = Reader.Read<BlockHeader>();
		if (blockHeader.m_Type != BlockType::End){
			throw BaseException(
				L"Endヘッダが違います",
				MeshFileName,
				L"GameObject::ReadMesh()"
				);
		}
	}


	//メッシュの作成
	void GameObject::CreateCustomMesh(){
		//頂点を作成するための配列
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		ReadMesh(vertices, indices, m_Materials);
		//頂点バッファの作成（頂点は変更できない）
		VertexUtil::CreateVertexBuffer(m_VertexBuffer, vertices);
		//頂点数の設定
		m_NumVertices = static_cast<UINT>(vertices.size());
		//インデックスバッファの作成
		VertexUtil::CreateIndexBuffer(m_IndexBuffer, indices);
		//インデックス数の設定
		m_NumIndicis = static_cast<UINT>(indices.size());
	}

	//シェーダリソースビューの作成
	ComPtr<ID3D11ShaderResourceView> GameObject::CreateShaderResView(const wstring& TextureFileName){
		//テクスチャ作成
		DirectX::TexMetadata metadata;
		DirectX::ScratchImage image;
		ThrowIfFailed(
			DirectX::LoadFromWICFile(TextureFileName.c_str(), 0, &metadata, image),
			L"テクスチャの読み込みに失敗しました",
			TextureFileName,
			L"GameObject::CreateShaderResView()"
			);
		//デバイスとコンテキストインターフェイスの取得
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContex = Dev->GetD3DDeviceContext();
		//ミューテックス
		std::mutex Mutex;
		ComPtr<ID3D11ShaderResourceView> ResView;
		Util::DemandCreate(ResView, Mutex, [&](ID3D11ShaderResourceView** pResult) -> HRESULT
		{
			// 画像からシェーダリソースViewの作成
			return ThrowIfFailed(CreateShaderResourceView(pDx11Device, image.GetImages(), image.GetImageCount(), metadata, pResult),
				L"シェーダーリソースビューを作成できません",
				L"if( FAILED( CreateShaderResourceView() ) )",
				L"Texture::Impl::Impl()"
				);
		});
		return ResView;
	}

	void GameObject::Create(){
		//メッシュの作成
		CreateCustomMesh();
		m_WorldMatrix.DefTransformation(m_Scale, m_Quaternion, m_Position);
	}

	void GameObject::Update(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		Quaternion SpanQt;
		SpanQt.RotationRollPitchYawFromVector(Vector3(0, ElapsedTime, 0));
		m_Quaternion *= SpanQt;
		m_WorldMatrix.DefTransformation(m_Scale, m_Quaternion, m_Position);
	}




	void GameObject::Draw(){
		//ゲームステージが無効ならリターン
		if (m_GameStgae.expired()){
			return;
		}
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		auto RenderStatePtr = Dev->GetRenderState();
		auto Stage = m_GameStgae.lock();
		auto ViewPtr = Stage->GetView();
		//ビューからカメラを取り出す
		auto PtrCamera = ViewPtr->GetCamera();
		//カメラの取得
		Matrix4X4 View, Proj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();

		//コンスタントバッファの設定
		Texture3DConstantBuffer cb1;
		//行列の設定(転置する)
		cb1.Model = Matrix4X4EX::Transpose(m_WorldMatrix);
		cb1.View = Matrix4X4EX::Transpose(View);
		cb1.Projection = Matrix4X4EX::Transpose(Proj);
		//ライトの設定
		//ステージから0番目のライトを取り出す
		auto PtrLight = ViewPtr->GetMultiLight()->GetLight(0);
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;

		//コンスタントバッファの更新
		pID3D11DeviceContext->UpdateSubresource(CBTexture3D::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//透明処理
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
		//デプスステンシルは使用する
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSTexture3D::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSTexture3D::GetPtr()->GetShader(), nullptr, 0);
		//リニアサンプラーを設定
		ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		for (auto& m : m_Materials){
			//テクスチャを設定
			pID3D11DeviceContext->PSSetShaderResources(0, 1, m.m_ShaderResView.GetAddressOf());
			//インプットレイアウトの設定
			pID3D11DeviceContext->IASetInputLayout(VSTexture3D::GetPtr()->GetInputLayout());
			//コンスタントバッファの設定
			ID3D11Buffer* pConstantBuffer = CBTexture3D::GetPtr()->GetBuffer();
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			//レンダリングステート
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
			//描画
			pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex, 0);
			//レンダリングステート
			pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
			//描画
			pID3D11DeviceContext->DrawIndexed(m.m_IndexCount, m.m_StartIndex,0);
		}
		//後始末
		Dev->InitializeStates(RenderStatePtr);
	}




	void GameStage::Create(){
		m_View = Object::CreateObject<View>();
		Rect2D<float> rect(0, 0, (float)App::GetApp()->GetGameWidth(), (float)App::GetApp()->GetGameHeight());
		m_View->ResetParamaters<Camera, MultiLight>(rect, Color4(0.0f, 0.0f, 0.0f, 1.0f), 1, 0.0f, 1.0f);
		auto PtrCamera = m_View->GetCamera();
		PtrCamera->SetEye(Vector3(0.0f, 2.0f, -5.0f));
		PtrCamera->SetAt(Vector3(0.0f, 0.0f, 2.0f));
		auto PtrLight = m_View->GetMultiLight();
		PtrLight->SetDefaultLighting();
		//デフォルトのレンダリングターゲットを作成
		m_DefaultRenderTarget = make_shared<DefaultRenderTarget>();
		
		auto Obj = Object::CreateObject<GameObject>(
			GetThis<GameStage>(),
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, -1.0f, 2.0f)
			);
		m_GameObjectVec.push_back(Obj);
		//文字列をつける
		m_StringSprite = Object::CreateObject<StringSprite>();
		m_StringSprite->SetText(L"");
		m_StringSprite->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

	}

	void GameStage::Update(){
		//ビューの変化
		m_View->Update();
		//オブジェクトの変化
		for (auto Obj : m_GameObjectVec){
			Obj->Update();
		}
		auto fps = App::GetApp()->GetStepTimer().GetFramesPerSecond();
		wstring FPS(L"FPS: ");
		FPS += Util::UintToWStr(fps);
		//文字列の変化
		m_StringSprite->SetText(FPS);
		m_StringSprite->Update();
	}

	void GameStage::Draw(){
		m_DefaultRenderTarget->ClearViews();
		m_DefaultRenderTarget->StartRenderTarget();
		for (auto Obj : m_GameObjectVec){
			Obj->Draw();
		}
		//文字列の描画
		m_StringSprite->Draw();
		m_DefaultRenderTarget->EndRenderTarget();
	}




}


//end basedx11

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


	//--------------------------------------------------------------------------------------
	//	class CubeResource : public ObjectResource;
	//	用途: 立方体リソース
	//--------------------------------------------------------------------------------------
	void CubeResource::Create(){
		//頂点を作成するための配列
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		//立方体の作成
		VertexUtil::CreateCube(1.0f, vertices, indices);
		//頂点バッファの作成（頂点は変更できない）
		VertexUtil::CreateVertexBuffer(m_VertexBuffer, vertices);
		//頂点数の設定
		m_NumVertices = static_cast<UINT>(vertices.size());
		//インデックスバッファの作成
		VertexUtil::CreateIndexBuffer(m_IndexBuffer, indices);
		//インデックス数の設定
		m_NumIndicis = static_cast<UINT>(indices.size());
	}


	//--------------------------------------------------------------------------------------
	//	class TextureResource : public Object, public SimpleInterface;
	//	用途: テクスチャリソース
	//--------------------------------------------------------------------------------------
	void TextureResource::CreateTextureBase(){
		//テクスチャ作成
		DirectX::TexMetadata metadata;
		DirectX::ScratchImage image;
		ThrowIfFailed(
			DirectX::LoadFromWICFile(m_TextureFileName.c_str(), 0, &metadata, image),
			L"テクスチャの読み込みに失敗しました",
			m_TextureFileName,
			L"TextureResource::CreateTextureBase()"
			);
		//デバイスとコンテキストインターフェイスの取得
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContex = Dev->GetD3DDeviceContext();
		//ミューテックス
		std::mutex Mutex;
		Util::DemandCreate(m_ShaderResView, Mutex, [&](ID3D11ShaderResourceView** pResult) -> HRESULT
		{
			// 画像からシェーダリソースViewの作成
			return ThrowIfFailed(CreateShaderResourceView(pDx11Device, image.GetImages(), image.GetImageCount(), metadata, pResult),
				L"シェーダーリソースビューを作成できません",
				L"if( FAILED( CreateShaderResourceView() ) )",
				L"TextureResource::CreateTextureBase()"
				);
		});
	}

	void TextureResource::Create(){
		if (m_TextureFileName != L""){
			CreateTextureBase();
		}
	}
	void TextureResource::CreateTexture(const wstring& str){
		if (str != L""){
			m_TextureFileName = str;
			CreateTextureBase();
		}
	}


	//--------------------------------------------------------------------------------------
	//	class CubeObject : public Object, public SimpleInterface;
	//	用途: 立方体オブジェクト
	//--------------------------------------------------------------------------------------
	CubeObject::CubeObject(const shared_ptr<GameStage>& GStage,
		const Vector3& StartScale,
		const Vector3& StartRotation,
		const Vector3& StartPos,
		float Span) :
		m_GameStgae(GStage),
		m_Scale(StartScale),
		m_Quaternion(),
		m_Position(StartPos),
		m_Span(Span),
		m_IsHit(false)
	{
		m_WorldMatrix.DefTransformation(m_Scale, m_Quaternion, m_Position);
	}

	void CubeObject::Create(){
	}
	void CubeObject::Update(){
		m_IsHit = false;
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_Position.x += m_Span * ElapsedTime;
		if (abs(m_Position.x) > 5.0f){
			m_Span *= -1.0f;
		}
		m_WorldMatrix.DefTransformation(m_Scale, m_Quaternion, m_Position);
	}
	void CubeObject::Update2(){
		//ゲームステージが無効ならリターン
		if (m_GameStgae.expired()){
			return;
		}
		if (m_IsHit){
			return;
		}
		auto GameStgaePtr = m_GameStgae.lock();
		//OBBを作成
		OBB obb(Vector3(1.0f, 1.0f, 1.0f), m_WorldMatrix);
		auto CubeVec = GameStgaePtr->GetCubeObjectVec();
		for (auto C : CubeVec){
			if (C != GetThis<CubeObject>()){
				OBB OtherObb = C->GetOBB();
				//トンネル現象を加味しないOBBとOBB判定
				if (HitTest::OBB_OBB(obb, OtherObb)){
					SetHit();
					C->SetHit();
				}
			}
		}
	}
	OBB CubeObject::GetOBB(){
		OBB obb(Vector3(1.0f, 1.0f, 1.0f), m_WorldMatrix);
		return obb;
	}

	void CubeObject::SetHit(){
		m_Span *= -1.0f;
		m_IsHit = true;
	}


	void CubeObject::Draw(){
		//ゲームステージが無効ならリターン
		if (m_GameStgae.expired()){
			return;
		}
		auto GameStgaePtr = m_GameStgae.lock();
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
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, 
			GameStgaePtr->GetCubeResource()->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(
			GameStgaePtr->GetCubeResource()->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//塗りつぶし処理（今回の画像はJPEGなので透明にしない）
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
		//デプスステンシルは使用する
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSTexture3D::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSTexture3D::GetPtr()->GetShader(), nullptr, 0);
		//リニアサンプラーを設定
		ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		//テクスチャを設定
		pID3D11DeviceContext->PSSetShaderResources(0, 1, 
			GameStgaePtr->GetTextureResource()->GetShaderResView().GetAddressOf());
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSTexture3D::GetPtr()->GetInputLayout());
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBTexture3D::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
		//描画
		pID3D11DeviceContext->DrawIndexed(
			GameStgaePtr->GetCubeResource()->GetNumIndicis(), 0, 0);
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
		//描画
		pID3D11DeviceContext->DrawIndexed(
			GameStgaePtr->GetCubeResource()->GetNumIndicis(), 0, 0);
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

		//立方体リソースの作成
		m_CubeResource = Object::CreateObject<CubeResource>();
		wstring TextureFile = App::GetApp()->m_wstrRelativeDataPath + L"sky.jpg";
		//テクスチャリソースの作成
		m_TextureResource = Object::CreateObject<TextureResource>(TextureFile);

		
		auto Obj = Object::CreateObject<CubeObject>(
			GetThis<GameStage>(),
			Vector3(1.0f,1.0f,1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(-2.0f, 0.0f, 2.0f),
			2.0f
			);
		m_CubeObjectVec.push_back(Obj);

		Obj = Object::CreateObject<CubeObject>(
			GetThis<GameStage>(),
			Vector3(1.0f, 1.0f, 1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(2.0f, 0.0f, 2.0f),
			-2.0f
			);
		m_CubeObjectVec.push_back(Obj);



		//文字列をつける
		m_StringSprite = Object::CreateObject<StringSprite>();
		m_StringSprite->SetText(L"");
		m_StringSprite->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));


	}

	void GameStage::Update(){
		//ビューの変化
		m_View->Update();
		//オブジェクトの変化
		for (auto Obj : m_CubeObjectVec){
			Obj->Update();
		}
		//衝突判定用のUpdate2
		for (auto Obj : m_CubeObjectVec){
			Obj->Update2();
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
		for (auto Obj : m_CubeObjectVec){
			Obj->Draw();
		}
		//文字列の描画
		m_StringSprite->Draw();
		m_DefaultRenderTarget->EndRenderTarget();
	}




}


//end basedx11

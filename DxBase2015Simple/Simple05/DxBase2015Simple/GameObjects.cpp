#include "stdafx.h"
#include "resource.h"
#include "GameObjects.h"
namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CBTextureColor3D : public ConstantBuffer<CBTextureColor3D,TextureColor3DConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<CBTextureColor3D, CBTextureColor3D::Deleter> CBTextureColor3D::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSTextureColor3D : public VertexShader< VSTextureColor3D, VertexPositionNormalColorTexture>;
	//	用途:  VSTextureColor3D頂点シェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<VSTextureColor3D, VSTextureColor3D::Deleter> VSTextureColor3D::m_Ptr;
	//構築
	VSTextureColor3D::VSTextureColor3D() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSTextureColor3D.cso"){}

	//--------------------------------------------------------------------------------------
	//	class PSTextureColor3D : public PixelShader<PSTextureColor3D>;
	//	用途: PSTextureColor3Dピクセルシェーダ
	//--------------------------------------------------------------------------------------
	unique_ptr<PSTextureColor3D, PSTextureColor3D::Deleter> PSTextureColor3D::m_Ptr;
	//構築
	PSTextureColor3D::PSTextureColor3D() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSTextureColor3D.cso")
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
		m_TextureFileName = App::GetApp()->m_wstrRelativeDataPath + L"trace.png";
	}

	//メッシュの作成
	void GameObject::CreateCustomMesh(){
		//頂点を作成するための配列
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		//正8面体の作成
		VertexUtil::CreateOctahedron(1.0f, vertices, indices);
		//頂点を変更するための配列
		vector<VertexPositionNormalColorTexture> set_vertices;
		for (auto V : vertices){
			Color4 Col;
			if (V.position.y > 0){
				Col = Color4(1.0f, 1.0f, 0.0f, 0.0f);
			}
			else if (V.position.y < 0){
				Col = Color4(0.0f, 1.0f, 1.0f, 0.0f);
			}
			else{
				if (V.position.x == 0.0f){
					Col = Color4(1.0f, 1.0f, 0.0f, 1.0f);
				}
				else if (V.position.x > 0.0f){
					Col = Color4(0.0f, 0.0f, 1.0f, 1.0f);
				}
				else{
					Col = Color4(1.0f, 0.0f, 0.0f, 1.0f);
				}
			}
			set_vertices.push_back(
				VertexPositionNormalColorTexture(V.position, V.normal, Col,V.textureCoordinate)
				);
		}
		//頂点バッファの作成（頂点は変更できない）
		VertexUtil::CreateVertexBuffer(m_VertexBuffer, set_vertices);

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
	void GameObject::CreateShaderResView(){
		//テクスチャ作成
		DirectX::TexMetadata metadata;
		DirectX::ScratchImage image;
		ThrowIfFailed(
			DirectX::LoadFromWICFile(m_TextureFileName.c_str(), 0, &metadata, image),
			L"テクスチャの読み込みに失敗しました",
			m_TextureFileName,
			L"GameObject::CreateShaderResView()"
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
				L"Texture::Impl::Impl()"
				);
		});
	}

	void GameObject::Create(){
		//メッシュの作成
		CreateCustomMesh();
		//シェーダーリソースビューの作成
		CreateShaderResView();
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
		TextureColor3DConstantBuffer cb1;
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
		pID3D11DeviceContext->UpdateSubresource(CBTextureColor3D::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalColorTexture);
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
		pID3D11DeviceContext->VSSetShader(VSTextureColor3D::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSTextureColor3D::GetPtr()->GetShader(), nullptr, 0);
		//リニアサンプラーを設定
		ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		//テクスチャを設定
		pID3D11DeviceContext->PSSetShaderResources(0, 1, m_ShaderResView.GetAddressOf());
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSTextureColor3D::GetPtr()->GetInputLayout());
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBTextureColor3D::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
		//描画
		pID3D11DeviceContext->DrawIndexed(m_NumIndicis, 0, 0);
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
		//描画
		pID3D11DeviceContext->DrawIndexed(m_NumIndicis, 0, 0);
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
			Vector3(1.0f,1.0f,1.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, 0.0f, 2.0f)
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

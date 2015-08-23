#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CBCustomShadowmap : public ConstantBuffer<CBCustomShadowmap,CustomShadowmapConstantBuffer>;
	//	用途: カスタムコンスタントバッファ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<CBCustomShadowmap, CBCustomShadowmap::Deleter> CBCustomShadowmap::m_Ptr;
	//--------------------------------------------------------------------------------------
	//	class VSCustomShadowmap : public VertexShader<VSCustomShadowmap, VertexPositionNormalColor>;
	//	用途: VSCustomShadowmap頂点シェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<VSCustomShadowmap, VSCustomShadowmap::Deleter> VSCustomShadowmap::m_Ptr;
	//構築
	VSCustomShadowmap::VSCustomShadowmap() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSCustomShadowmap.cso")
	{}




	//--------------------------------------------------------------------------------------
	//	class CustomShadowmap : public Shadowmap;
	//	用途: カスタムシャドウマップコンポーネント
	//--------------------------------------------------------------------------------------
	CustomShadowmap::CustomShadowmap(const shared_ptr<GameObject>& GameObjectPtr) :
		Shadowmap(GameObjectPtr){}
	CustomShadowmap::~CustomShadowmap(){}

	//操作
	void CustomShadowmap::Draw(){
		//m_GameObjectがnullならDrawできない
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrStage = PtrGameObject->GetStage();
			if (!PtrStage){
				return;
			}
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			//ステージから0番目のライトを取り出す
			auto PtrLight = PtrStage->GetTargetLight(0);
			//ステージからカメラを取り出す
			auto PtrCamera = PtrStage->GetTargetCamera();

			if (PtrT && PtrLight && PtrCamera){

				Matrix4X4 RealWorldMatrix = GetMeshToTransform() * PtrT->GetWorldMatrix();


				auto Dev = App::GetApp()->GetDeviceResources();
				auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
				//ステータスのポインタ
				auto RenderStatePtr = PtrStage->GetRenderState();


				//ライトの取得
				Matrix4X4 LightView, LightProj;

				Vector3 LightDir = -1.0 * PtrLight->GetDirectional();
				Vector3 LightAt = PtrCamera->GetAt();
				Vector3 LightEye = LightAt + (LightDir * GetLightHeight());

				//ライトのビューと射影を計算
				LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
				LightProj.OrthographicLH(GetViewWidth(), GetViewHeight(), GetLightNear(), GetLightFar());

				CustomShadowmapConstantBuffer Cb;

				Cb.mWorld = Matrix4X4EX::Transpose(RealWorldMatrix);
				Cb.mView = Matrix4X4EX::Transpose(LightView);
				Cb.mProj = Matrix4X4EX::Transpose(LightProj);

				//これより描画処理
				//コンスタントバッファの更新
				pID3D11DeviceContext->UpdateSubresource(CBCustomShadowmap::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);


				//インプットレイアウトのセット
				pID3D11DeviceContext->IASetInputLayout(VSCustomShadowmap::GetPtr()->GetInputLayout());
				//ストライドとオフセット
				UINT stride = sizeof(VertexPositionNormalColor);
				UINT offset = 0;

				auto GameObj = dynamic_pointer_cast<CustomDrawOctahedron>(GetGameObject());
				//頂点バッファをセット

				pID3D11DeviceContext->IASetVertexBuffers(0, 1, GameObj->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				//頂点シェーダーのセット
				pID3D11DeviceContext->VSSetShader(VSCustomShadowmap::GetPtr()->GetShader(), nullptr, 0);

				//インデックスバッファのセット
				pID3D11DeviceContext->IASetIndexBuffer(GameObj->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//描画方法（3角形）
				pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//ピクセルシェーダはセットしない！
				pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
				//ジオメトリシェーダの設定（使用しない）
				pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);

				//コンスタントバッファの設定
				ID3D11Buffer* pConstantBuffer = CBCustomShadowmap::GetPtr()->GetBuffer();
				pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				//コンスタントバッファをピクセルシェーダにセット
				pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
				//描画
				pID3D11DeviceContext->DrawIndexed(GameObj->GetNumIndicis(), 0, 0);
				//後始末
				Dev->InitializeStates(RenderStatePtr);

			}

		}
	}


	//--------------------------------------------------------------------------------------
	//	class CBCustomDraw : public ConstantBuffer<CBCustomDraw,CustomDrawConstantBuffer>;
	//	用途: カスタムコンスタントバッファ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<CBCustomDraw, CBCustomDraw::Deleter> CBCustomDraw::m_Ptr;
	//--------------------------------------------------------------------------------------
	//	class VSCustomDraw : public VertexShader<VSCustomDraw, VertexPositionNormalColor>;
	//	用途: VSCustomDraw頂点シェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<VSCustomDraw, VSCustomDraw::Deleter> VSCustomDraw::m_Ptr;
	//構築
	VSCustomDraw::VSCustomDraw() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSSimpleBase.cso")
	{}
	//--------------------------------------------------------------------------------------
	//	class PSCustomDraw : public PixelShader<PSCustomDraw>;
	//	用途: PSCustomDrawピクセルシェーダ
	//--------------------------------------------------------------------------------------
	unique_ptr<PSCustomDraw, PSCustomDraw::Deleter> PSCustomDraw::m_Ptr;
	//構築
	PSCustomDraw::PSCustomDraw() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSSimpleBase.cso")
	{}





	//--------------------------------------------------------------------------------------
	//	class CustomDrawOctahedron : public GameObject;
	//	独自のシェーダで描画する8面体
	//--------------------------------------------------------------------------------------
	//メッシュの作成
	void CustomDrawOctahedron::CreateCustomMesh(){
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		//正8面体の作成
		VertexUtil::CreateOctahedron(1.0f, vertices, indices);
		m_BackupVirtex.clear();
		for (auto V : vertices){
			Color4 Col;
			if (V.position.y > 0){
				switch (m_Pattern){
				case 0:
					Col = Color4(1.0f, 0.0f, 0.0f, 0.0f);
					break;
				case 1:
					Col = Color4(0.0f, 1.0f, 0.0f, 0.0f);
					break;
				case 2:
					Col = Color4(0.0f, 0.0f, 1.0f, 0.0f);
					break;
				default:
					Col = Color4(1.0f, 1.0f, 1.0f, 1.0f);
					break;
				}
			}
			else if (V.position.y < 0){
				switch (m_Pattern){
				case 0:
					Col = Color4(0.0f, 1.0f, 0.0f, 0.0f);
					break;
				case 1:
					Col = Color4(0.0f, 0.0f, 1.0f, 0.0f);
					break;
				case 2:
					Col = Color4(1.0f, 0.0f, 0.0f, 0.0f);
					break;
				default:
					Col = Color4(1.0f, 1.0f, 1.0f, 1.0f);
					break;
				}
			}
			else{
				Col = Color4(1.0f, 1.0f, 1.0f, 1.0f);
			}
			m_BackupVirtex.push_back(
				VertexPositionNormalColor(V.position, V.normal, Col)
			);
		}

		//ミューテックス
		std::mutex Mutex;
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		//バッファの作成
		Util::DemandCreate(m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
		{
			//頂点バッファの作成(頂点変更可能)
			VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, m_BackupVirtex, pResult);
		});
		//頂点数の設定
		m_NumVertices = static_cast<UINT>(vertices.size());
		Util::DemandCreate(m_IndexBuffer, Mutex, [&](ID3D11Buffer** pResult)
		{
			//インデックスバッファの作成
			VertexUtil::CreatePrimitiveBuffer(pDx11Device, indices, D3D11_BIND_INDEX_BUFFER, pResult);
		});
		//インデックス数の設定
		m_NumIndicis = static_cast<UINT>(indices.size());
	}

	//頂点の変更
	void CustomDrawOctahedron::UpdateCustomMesh(){

		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		//座標を変更する
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pID3D11DeviceContext->Map(m_VertexBuffer.Get(), 0, mapType, 0, &mappedBuffer))){
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"CustomDrawOctahedron::UpdateCustomMesh()"
				);
		}
		//頂点の変更
		auto* vertices = (VertexPositionNormalColor*)mappedBuffer.pData;

		for (size_t i = 0; i < m_NumVertices; i++){
			Vector3 Pos = m_BackupVirtex[i].position;
			if (Pos.y > 0){
				//サインを使っていったり来たりするようにする
				Pos.y += sin(m_TotalTime);
			}
			vertices[i] = VertexPositionNormalColor(
				Pos,
				m_BackupVirtex[i].normal,
				m_BackupVirtex[i].color
				);
		}
		//アンマップ
		pID3D11DeviceContext->Unmap(m_VertexBuffer.Get(), 0);
	}



	//構築と破棄
	CustomDrawOctahedron::CustomDrawOctahedron(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos,
		int Pattern) :
		GameObject(StagePtr),
		m_StartScale(StartScale),
		m_StartRotation(StartRotation),
		m_StartPos(StartPos),
		m_Pattern(Pattern),
		m_TotalTime(0)
	{}
	CustomDrawOctahedron::~CustomDrawOctahedron(){
	}
	//初期化
	void CustomDrawOctahedron::Create(){
		//Transformだけは追加しなくても取得できる
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(m_StartScale);
		Ptr->SetPosition(m_StartPos);
		Ptr->SetRotation(m_StartRotation);
		//頂点バッファとインデックスバッファを作成
		CreateCustomMesh();
		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<CustomShadowmap>();
		//透明処理（描画順制御のため）
		SetAlphaActive(true);
	}
	void CustomDrawOctahedron::Update(){
		UpdateCustomMesh();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrTransform = GetComponent<Transform>();
		auto Qt = PtrTransform->GetQuaternion();
		Quaternion Span;
		Span.RotationRollPitchYawFromVector(Vector3(0, ElapsedTime, 0));
		Qt *= Span;
		PtrTransform->SetQuaternion(Qt);
	}
	void CustomDrawOctahedron::Draw(){
		//デバイスの取得
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		auto RenderStatePtr = GetStage()->GetRenderState();

		auto PtrT = GetComponent<Transform>();
		//ステージからカメラを取り出す
		auto PtrCamera = GetStage()->GetTargetCamera();
		//カメラの取得
		Matrix4X4 View, Proj, WorldViewProj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();

		//コンスタントバッファの設定
		CustomDrawConstantBuffer cb1;
		//行列の設定(転置する)
		cb1.World = Matrix4X4EX::Transpose(PtrT->GetWorldMatrix());;
		cb1.View = Matrix4X4EX::Transpose(View);
		cb1.Projection = Matrix4X4EX::Transpose(Proj);
		//ライトの設定
		//ステージから0番目のライトを取り出す
		auto PtrLight = GetStage()->GetTargetLight(0);
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;

		//コンスタントバッファの更新
		pID3D11DeviceContext->UpdateSubresource(CBCustomDraw::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalColor);
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//ステータスのポインタ
		//テクスチャを取得
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pNullSR[1] = { 0 };
		//半透明処理
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
		//デプスステンシルは使用する
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(VSCustomDraw::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSCustomDraw::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(VSCustomDraw::GetPtr()->GetInputLayout());
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = CBCustomDraw::GetPtr()->GetBuffer();
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


}
//endof  basedx11

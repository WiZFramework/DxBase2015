#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CustomDrawBox : public GameObject;
	//	独自のシェーダで描画するボックス
	//--------------------------------------------------------------------------------------
	//メッシュリソースの作成
	void CustomDrawBox::CreateMeshResource(){
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		//Face数は6
		const int FaceCount = 6;
		static const Vector3 faceNormals[FaceCount] =
		{
			{ 0, 0, 1 },
			{ 0, 0, -1 },
			{ 1, 0, 0 },
			{ -1, 0, 0 },
			{ 0, 1, 0 },
			{ 0, -1, 0 },
		};
		static const Vector2 textureCoordinates[4] =
		{
			{ 1, 0 },
			{ 1, 1 },
			{ 0, 1 },
			{ 0, 0 },
		};
		//１辺の長さはの半分は0.5
		float size = 0.5f;
		for (int i = 0; i < FaceCount; i++)
		{
			//法線
			auto normal = faceNormals[i];
			auto basis = (i >= 4) ? Vector3(0, 0, 1) : Vector3(0, 1, 0);
			auto side1 = Vector3EX::Cross(normal, basis);
			auto side2 = Vector3EX::Cross(normal, side1);
			//インデックスの登録
			size_t vbase = vertices.size();
			indices.push_back((uint16_t)vbase + 0);
			indices.push_back((uint16_t)vbase + 1);
			indices.push_back((uint16_t)vbase + 2);
			indices.push_back((uint16_t)vbase + 0);
			indices.push_back((uint16_t)vbase + 2);
			indices.push_back((uint16_t)vbase + 3);
			//頂点の登録
			vertices.push_back(VertexPositionNormalTexture((normal - side1 - side2) * size, normal, textureCoordinates[0]));
			vertices.push_back(VertexPositionNormalTexture((normal - side1 + side2) * size, normal, textureCoordinates[1]));
			vertices.push_back(VertexPositionNormalTexture((normal + side1 + side2) * size, normal, textureCoordinates[2]));
			vertices.push_back(VertexPositionNormalTexture((normal + side1 - side2) * size, normal, textureCoordinates[3]));
		}
		//RHからLHに変更
		if ((indices.size() % 3) != 0){
			throw BaseException(
				L"インデックスの数が合いません",
				L"if((indices.size() % 3) == 0)",
				L"CustomDrawBox::CreateMeshResource()"
				);
		}
		for (auto it = indices.begin(); it != indices.end(); it += 3)
		{
			std::swap(*it, *(it + 2));
		}
		for (auto it = vertices.begin(); it != vertices.end(); ++it)
		{
			it->textureCoordinate.x = (1.f - it->textureCoordinate.x);
		}
		//頂点とインデックスの配列からメッシュリソースを作成（頂点を変更できる）
		m_MeshResource = CommonMeshResource::CreateCommonMeshResource(vertices, indices,true);
	}

	//頂点の変更
	void CustomDrawBox::UpdateMeshResource(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		//座標を変更する
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//頂点バッファをリソースから取り出す
		auto pVertexBuffer = m_MeshResource->GetVertexBuffer().Get();
		//バックアップの頂点を取り出す
		vector<VertexPositionNormalTexture>& BacukVertices = m_MeshResource->GetBackupVertices();

		//D3D11_MAP_WRITE_DISCARDは重要。この処理により、GPUに邪魔されない
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//頂点のマップ
		if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))){
			// Map失敗
			throw BaseException(
				L"頂点のMapに失敗しました。",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"CustomDrawBox::UpdateMeshResource()"
				);
		}
		//頂点の変更
		VertexPositionNormalTexture* vertices = (VertexPositionNormalTexture*)mappedBuffer.pData;

		for (size_t i = 0; i < m_MeshResource->GetNumVertices(); i++){
			Vector3 Pos = BacukVertices[i].position;
			if (Pos.x > 0 && Pos.y > 0 && Pos.z > 0){
				//サインを使っていったり来たりするようにする
				Pos.y += sin(m_TotalTime);
			}
			else if (Pos.x < 0 && Pos.y > 0 && Pos.z < 0){
				//サインを使っていったり来たりするようにする
				Pos.y -= sin(m_TotalTime);
			}
			vertices[i] = VertexPositionNormalTexture(
				Pos,
				BacukVertices[i].normal,
				BacukVertices[i].textureCoordinate
				);

		}


		//アンマップ
		pID3D11DeviceContext->Unmap(pVertexBuffer, 0);

	}



	//構築と破棄
	CustomDrawBox::CustomDrawBox(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartScale(StartScale),
		m_StartRotation(StartRotation),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}
	CustomDrawBox::~CustomDrawBox(){
	}
	//初期化
	void CustomDrawBox::Create(){
		//Transformだけは追加しなくても取得できる
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(m_StartScale);
		Ptr->SetPosition(m_StartPos);
		Ptr->SetRotation(m_StartRotation);
		//頂点バッファとインデックスバッファを作成
		CreateMeshResource();
		//シェーダの作成
		//コンスタントバッファ
		m_ConstantBuffer = make_shared<CBuffer<ConstantBuffer>>();
		//頂点シェーダ
		m_VirtexShader = make_shared<VShader<VertexPositionNormalTexture>>(App::GetApp()->m_wstrRelativeShadersPath + L"VSSimpleBase.cso");
		//ピクセルシェーダー
		m_PixelShader = make_shared<PShader>(App::GetApp()->m_wstrRelativeShadersPath + L"PSSimpleBase.cso");
		//影をつける（シャドウマップを描画する）
		auto ShadowPtr = AddComponent<Shadowmap>();
		//影の形（メッシュ）を設定
		ShadowPtr->SetMeshResource(m_MeshResource);
		//透明処理（描画順制御のため）
		SetAlphaActive(true);
	}
	void CustomDrawBox::Update(){
		UpdateMeshResource();
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto PtrTransform = GetComponent<Transform>();
		auto Qt = PtrTransform->GetQuaternion();
		Quaternion Span;
		Span.RotationRollPitchYawFromVector(Vector3(0, ElapsedTime, 0));
		Qt *= Span;
		PtrTransform->SetQuaternion(Qt);
	}
	void CustomDrawBox::Draw(){
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
		ConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
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
		pID3D11DeviceContext->UpdateSubresource(m_ConstantBuffer->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//ストライドとオフセット
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//頂点バッファの設定
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_MeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//インデックスバッファのセット
		pID3D11DeviceContext->IASetIndexBuffer(m_MeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//描画方法（3角形）
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//ステータスのポインタ
		//テクスチャを取得
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pNullSR[1] = { 0 };
		//テクスチャを設定
		auto PtrTextureResource = App::GetApp()->GetResource<TextureResource>(L"TRACE_TX");
		pID3D11DeviceContext->PSSetShaderResources(0, 1, PtrTextureResource->GetShaderResourceView().GetAddressOf());
		//リニアサンプラーを設定
		ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		//半透明処理
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);

		//デプスステンシルは使用する
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
		//シェーダの設定
		pID3D11DeviceContext->VSSetShader(m_VirtexShader->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(m_PixelShader->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pID3D11DeviceContext->IASetInputLayout(m_VirtexShader->GetInputLayout());
		//コンスタントバッファの設定
		ID3D11Buffer* pConstantBuffer = m_ConstantBuffer->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
		//描画
		pID3D11DeviceContext->DrawIndexed(m_MeshResource->GetNumIndicis(), 0, 0);
		//レンダリングステート
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
		//描画
		pID3D11DeviceContext->DrawIndexed(m_MeshResource->GetNumIndicis(), 0, 0);
		//後始末
		Dev->InitializeStates(RenderStatePtr);

	}


}
//endof  basedx11

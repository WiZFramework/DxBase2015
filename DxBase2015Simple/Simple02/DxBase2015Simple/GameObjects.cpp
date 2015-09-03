#include "stdafx.h"
#include "resource.h"
#include "GameObjects.h"
namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CBSimpleDirect : public ConstantBuffer<CBSimpleDirect,SimpleDirectConstantBuffer>;
	//	用途: コンスタントバッファ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<CBSimpleDirect, CBSimpleDirect::Deleter> CBSimpleDirect::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSSimpleDirect : public VertexShader<VSSimpleDirect, VertexPosition>;
	//	用途: VSSimpleDirect頂点シェーダ
	//--------------------------------------------------------------------------------------
	//シングルトン処理
	unique_ptr<VSSimpleDirect, VSSimpleDirect::Deleter> VSSimpleDirect::m_Ptr;
	//構築
	VSSimpleDirect::VSSimpleDirect() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSSimpleDirect.cso"){}

	//--------------------------------------------------------------------------------------
	//	class PSSimpleDirect : public PixelShader<PSSimpleDirect>;
	//	用途: PSSimpleピクセルシェーダ
	//--------------------------------------------------------------------------------------
	unique_ptr<PSSimpleDirect, PSSimpleDirect::Deleter> PSSimpleDirect::m_Ptr;
	//構築
	PSSimpleDirect::PSSimpleDirect() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSSimpleDirect.cso")
	{
	}

	void GameObject::Create(){
		//頂点を作成するための配列
		vector<VertexPosition> vertices = {
			{ VertexPosition(Vector3(0.0f, 0.5f, 0.5f)) },
			{ VertexPosition(Vector3(0.5f, -0.5f, 0.5f)) },
			{ VertexPosition(Vector3(-0.5f, -0.5f, 0.5f)) },
		};
		//メッシュの作成
		VertexUtil::CreateVertexBuffer(m_VertexBuffer, vertices);
		//頂点数の設定
		m_NumVertices = static_cast<UINT>(vertices.size());
	}



	void GameObject::Draw(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//ステータスのポインタ
		auto RenderStatePtr = Dev->GetRenderState();
		//コンスタントバッファの準備
		SimpleDirectConstantBuffer sb;
		//三角形の色は黄色
		sb.m_DiffuseColor = Color4(1.0f, 1.0f, 0, 1.0f);
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSimpleDirect::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
		//描画方法（三角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//デプスステンシルは使用しない
		pD3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
		//コンスタントバッファの設定(ピクセルシェーダのみでよい)
		ID3D11Buffer* pConstantBuffer = CBSimpleDirect::GetPtr()->GetBuffer();
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSSimpleDirect::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSSimpleDirect::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSSimpleDirect::GetPtr()->GetInputLayout());
		//描画
		//ストライドとオフセット
		UINT stride = sizeof(VertexPosition);
		UINT offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		pD3D11DeviceContext->Draw(m_NumVertices, 0);
		//後始末
		Dev->InitializeStates(RenderStatePtr);
	}




	void GameStage::Create(){
		m_View = Object::CreateObject<View>();
		Rect2D<float> rect(0, 0, (float)App::GetApp()->GetGameWidth(), (float)App::GetApp()->GetGameHeight());
		m_View->ResetParamaters<Camera, MultiLight>(rect, Color4(0.0f, 0.0f, 0.0f, 1.0f), 1, 0.0f, 1.0f);
		//デフォルトのレンダリングターゲットを作成
		m_DefaultRenderTarget = make_shared<DefaultRenderTarget>();
		//三角形の追加
		auto Obj = Object::CreateObject<GameObject>();
		m_GameObjectVec.push_back(Obj);
		//文字列をつける
		m_StringSprite = Object::CreateObject<StringSprite>();
		m_StringSprite->SetText(L"");
		m_StringSprite->SetTextRect(Rect2D<float>(16.0f, 16.0f, 640.0f, 480.0f));

	}

	void GameStage::Update(){
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

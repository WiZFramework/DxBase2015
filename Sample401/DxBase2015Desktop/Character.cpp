#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class TriangleObject : public GameObject;
	//	用途: 三角形オブジェクト
	//--------------------------------------------------------------------------------------
	//構築と破棄
	TriangleObject::TriangleObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_VertexBuffer(),
		m_NumVertices(0)
	{}
	TriangleObject::~TriangleObject(){}

	//初期化
	void TriangleObject::Create(){
		//頂点を作成するための配列
		vector<VertexPosition> vertices = {
			{ VertexPosition(Vector3(0.0f, 0.5f, 0.5f)) },
			{ VertexPosition(Vector3(0.5f, -0.5f, 0.5f)) },
			{ VertexPosition(Vector3(-0.5f, -0.5f, 0.5f)) },
		};
		VertexUtil::CreateVertexBuffer(m_VertexBuffer, vertices);
		//頂点数の設定
		m_NumVertices = static_cast<UINT>(vertices.size());

	}
	//描画
	void TriangleObject::Draw(){

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		//コンスタントバッファの準備
		SimpleDirectConstantBuffer sb;
		sb.m_DiffuseColor = Color4(1.0f, 1.0f, 0, 1.0f);
		//コンスタントバッファの更新
		pD3D11DeviceContext->UpdateSubresource(CBSimpleDirect::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//ストライドとオフセット
		UINT stride = sizeof(VertexPosition);
		UINT offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//描画方法（3角形）
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//コンスタントバッファの設定(ピクセルシェーダのみでよい)
		ID3D11Buffer* pConstantBuffer = CBSimpleDirect::GetPtr()->GetBuffer();
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//シェーダの設定
		pD3D11DeviceContext->VSSetShader(VSSimpleDirect::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSSimpleDirect::GetPtr()->GetShader(), nullptr, 0);
		//インプットレイアウトの設定
		pD3D11DeviceContext->IASetInputLayout(VSSimpleDirect::GetPtr()->GetInputLayout());

		//描画
		pD3D11DeviceContext->Draw(m_NumVertices, 0);
		//後始末
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
	}



}
//endof  basedx11

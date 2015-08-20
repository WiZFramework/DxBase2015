#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class TriangleObject : public GameObject;
	//	�p�r: �O�p�`�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	TriangleObject::TriangleObject(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		m_VertexBuffer(),
		m_NumVertices(0)
	{}
	TriangleObject::~TriangleObject(){}

	//������
	void TriangleObject::Create(){
		//���_���쐬���邽�߂̔z��
		vector<VertexPosition> vertices = {
			{ VertexPosition(Vector3(0.0f, 0.5f, 0.5f)) },
			{ VertexPosition(Vector3(0.5f, -0.5f, 0.5f)) },
			{ VertexPosition(Vector3(-0.5f, -0.5f, 0.5f)) },
		};
		VertexUtil::CreateVertexBuffer(m_VertexBuffer, vertices);
		//���_���̐ݒ�
		m_NumVertices = static_cast<UINT>(vertices.size());

	}
	//�`��
	void TriangleObject::Draw(){

		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		//�R���X�^���g�o�b�t�@�̏���
		SimpleDirectConstantBuffer sb;
		sb.m_DiffuseColor = Color4(1.0f, 1.0f, 0, 1.0f);
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBSimpleDirect::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);

		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPosition);
		UINT offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		//�R���X�^���g�o�b�t�@�̐ݒ�(�s�N�Z���V�F�[�_�݂̂ł悢)
		ID3D11Buffer* pConstantBuffer = CBSimpleDirect::GetPtr()->GetBuffer();
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSSimpleDirect::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSSimpleDirect::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSSimpleDirect::GetPtr()->GetInputLayout());

		//�`��
		pD3D11DeviceContext->Draw(m_NumVertices, 0);
		//��n��
		pD3D11DeviceContext->VSSetShader(nullptr, nullptr, 0);
		pD3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
	}



}
//endof  basedx11

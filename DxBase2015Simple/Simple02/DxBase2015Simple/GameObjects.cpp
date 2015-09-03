#include "stdafx.h"
#include "resource.h"
#include "GameObjects.h"
namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CBSimpleDirect : public ConstantBuffer<CBSimpleDirect,SimpleDirectConstantBuffer>;
	//	�p�r: �R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<CBSimpleDirect, CBSimpleDirect::Deleter> CBSimpleDirect::m_Ptr;

	//--------------------------------------------------------------------------------------
	//	class VSSimpleDirect : public VertexShader<VSSimpleDirect, VertexPosition>;
	//	�p�r: VSSimpleDirect���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<VSSimpleDirect, VSSimpleDirect::Deleter> VSSimpleDirect::m_Ptr;
	//�\�z
	VSSimpleDirect::VSSimpleDirect() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSSimpleDirect.cso"){}

	//--------------------------------------------------------------------------------------
	//	class PSSimpleDirect : public PixelShader<PSSimpleDirect>;
	//	�p�r: PSSimple�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	unique_ptr<PSSimpleDirect, PSSimpleDirect::Deleter> PSSimpleDirect::m_Ptr;
	//�\�z
	PSSimpleDirect::PSSimpleDirect() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSSimpleDirect.cso")
	{
	}

	void GameObject::Create(){
		//���_���쐬���邽�߂̔z��
		vector<VertexPosition> vertices = {
			{ VertexPosition(Vector3(0.0f, 0.5f, 0.5f)) },
			{ VertexPosition(Vector3(0.5f, -0.5f, 0.5f)) },
			{ VertexPosition(Vector3(-0.5f, -0.5f, 0.5f)) },
		};
		//���b�V���̍쐬
		VertexUtil::CreateVertexBuffer(m_VertexBuffer, vertices);
		//���_���̐ݒ�
		m_NumVertices = static_cast<UINT>(vertices.size());
	}



	void GameObject::Draw(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�X�e�[�^�X�̃|�C���^
		auto RenderStatePtr = Dev->GetRenderState();
		//�R���X�^���g�o�b�t�@�̏���
		SimpleDirectConstantBuffer sb;
		//�O�p�`�̐F�͉��F
		sb.m_DiffuseColor = Color4(1.0f, 1.0f, 0, 1.0f);
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBSimpleDirect::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
		//�`����@�i�O�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�f�v�X�X�e���V���͎g�p���Ȃ�
		pD3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
		//�R���X�^���g�o�b�t�@�̐ݒ�(�s�N�Z���V�F�[�_�݂̂ł悢)
		ID3D11Buffer* pConstantBuffer = CBSimpleDirect::GetPtr()->GetBuffer();
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSSimpleDirect::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSSimpleDirect::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSSimpleDirect::GetPtr()->GetInputLayout());
		//�`��
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPosition);
		UINT offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		pD3D11DeviceContext->Draw(m_NumVertices, 0);
		//��n��
		Dev->InitializeStates(RenderStatePtr);
	}




	void GameStage::Create(){
		m_View = Object::CreateObject<View>();
		Rect2D<float> rect(0, 0, (float)App::GetApp()->GetGameWidth(), (float)App::GetApp()->GetGameHeight());
		m_View->ResetParamaters<Camera, MultiLight>(rect, Color4(0.0f, 0.0f, 0.0f, 1.0f), 1, 0.0f, 1.0f);
		//�f�t�H���g�̃����_�����O�^�[�Q�b�g���쐬
		m_DefaultRenderTarget = make_shared<DefaultRenderTarget>();
		//�O�p�`�̒ǉ�
		auto Obj = Object::CreateObject<GameObject>();
		m_GameObjectVec.push_back(Obj);
		//�����������
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
		//������̕ω�
		m_StringSprite->SetText(FPS);
		m_StringSprite->Update();
	}

	void GameStage::Draw(){
		m_DefaultRenderTarget->ClearViews();
		m_DefaultRenderTarget->StartRenderTarget();
		for (auto Obj : m_GameObjectVec){
			Obj->Draw();
		}
		//������̕`��
		m_StringSprite->Draw();
		m_DefaultRenderTarget->EndRenderTarget();
	}




}


//end basedx11

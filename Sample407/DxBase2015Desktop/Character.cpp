#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CBCustomDraw : public ConstantBuffer<CBCustomDraw,CustomDrawConstantBuffer>;
	//	�p�r: �J�X�^���R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<CBCustomDraw, CBCustomDraw::Deleter> CBCustomDraw::m_Ptr;
	//--------------------------------------------------------------------------------------
	//	class VSCustomDraw : public VertexShader<VSCustomDraw, VertexPositionColor>;
	//	�p�r: VSCustomDraw���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<VSCustomDraw, VSCustomDraw::Deleter> VSCustomDraw::m_Ptr;
	//�\�z
	VSCustomDraw::VSCustomDraw() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSSimpleBase.cso")
	{}
	//--------------------------------------------------------------------------------------
	//	class PSCustomDraw : public PixelShader<PSCustomDraw>;
	//	�p�r: PSCustomDraw�s�N�Z���V�F�[�_
	//--------------------------------------------------------------------------------------
	unique_ptr<PSCustomDraw, PSCustomDraw::Deleter> PSCustomDraw::m_Ptr;
	//�\�z
	PSCustomDraw::PSCustomDraw() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSSimpleBase.cso")
	{}





	//--------------------------------------------------------------------------------------
	//	class CustomDrawVortex : public GameObject;
	//	�Ǝ��̃V�F�[�_�ŕ`�悷��Q����
	//--------------------------------------------------------------------------------------
	//���b�V���̍쐬
	void CustomDrawVortex::CreateCustomMesh(){
		m_BackupVirtex.clear();
		float rot = 0.0f;
		float Min = 1.0f / 50.0f;
		for (size_t i = 0; i < 50; i++){
			m_BackupVirtex.push_back(
				VertexPositionColor(
				Vector3(sin(rot), (float)i * 0.05f, cos(rot)),	//�Q������ɂȂ�悤������
				Color4(1.0, Min * i, 0.0, 1.0)
					)
				);
			rot += 0.5f;
		}
		//���_�o�b�t�@�̍쐬�i���_��ύX�ł���j
		VertexUtil::CreateDynamicVertexBuffer(m_VertexBuffer, m_BackupVirtex);
		//���_���̐ݒ�
		m_NumVertices = static_cast<UINT>(m_BackupVirtex.size());
	}

	//���_�̕ύX
	void CustomDrawVortex::UpdateCustomMesh(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime * 10.0f;
		//���W��ύX����
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//D3D11_MAP_WRITE_DISCARD�͏d�v�B���̏����ɂ��AGPU�Ɏז�����Ȃ�
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//���_�̃}�b�v
		if (FAILED(pID3D11DeviceContext->Map(m_VertexBuffer.Get(), 0, mapType, 0, &mappedBuffer))){
			// Map���s
			throw BaseException(
				L"���_��Map�Ɏ��s���܂����B",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"CustomDrawOctahedron::UpdateCustomMesh()"
				);
		}
		//���_�̕ύX
		auto* vertices = (VertexPositionColor*)mappedBuffer.pData;

		float rot = m_TotalTime;
		for (size_t i = 0; i < m_NumVertices; i++){
			Vector3 Pos = m_BackupVirtex[i].position;
			Pos = Vector3(sin(rot), (float)i * 0.05f, cos(rot));
			vertices[i] = VertexPositionColor(
				Pos,
				m_BackupVirtex[i].color
				);
			rot += 0.5f;
		}
		//�A���}�b�v
		pID3D11DeviceContext->Unmap(m_VertexBuffer.Get(), 0);

	}



	//�\�z�Ɣj��
	CustomDrawVortex::CustomDrawVortex(shared_ptr<Stage>& StagePtr, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}
	CustomDrawVortex::~CustomDrawVortex(){
	}
	//������
	void CustomDrawVortex::Create(){
		//Transform�����͒ǉ����Ȃ��Ă��擾�ł���
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(1.0f, 1.0f, 1.0f);
		Ptr->SetRotation(0,0,0);
		Ptr->SetPosition(m_StartPos);
		//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬
		CreateCustomMesh();
	}
	void CustomDrawVortex::Update(){
		UpdateCustomMesh();
	}
	void CustomDrawVortex::Draw(){
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�X�e�[�^�X�̃|�C���^
		auto RenderStatePtr = GetStage()->GetRenderState();

		auto PtrT = GetComponent<Transform>();
		//�X�e�[�W����J���������o��
		auto PtrCamera = GetStage()->GetTargetCamera();
		//�J�����̎擾
		Matrix4X4 View, Proj, WorldViewProj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();

		//�R���X�^���g�o�b�t�@�̐ݒ�
		CustomDrawConstantBuffer cb1;
		//�s��̐ݒ�(�]�u����)
		cb1.World = Matrix4X4EX::Transpose(PtrT->GetWorldMatrix());;
		cb1.View = Matrix4X4EX::Transpose(View);
		cb1.Projection = Matrix4X4EX::Transpose(Proj);
		//�R���X�^���g�o�b�t�@�̍X�V
		pID3D11DeviceContext->UpdateSubresource(CBCustomDraw::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionColor);
		UINT offset = 0;
		//���_�o�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//�`����@
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
		//�X�e�[�^�X�̃|�C���^
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pNullSR[1] = { 0 };
		//�f�v�X�X�e���V���͎g�p���Ȃ�
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(VSCustomDraw::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSCustomDraw::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(VSCustomDraw::GetPtr()->GetInputLayout());
		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBCustomDraw::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
		//�`��
		pID3D11DeviceContext->Draw(m_NumVertices, 0);
		//��n��
		Dev->InitializeStates(RenderStatePtr);

	}


}
//endof  basedx11

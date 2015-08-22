#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CustomDrawBox : public GameObject;
	//	�Ǝ��̃V�F�[�_�ŕ`�悷��{�b�N�X
	//--------------------------------------------------------------------------------------
	//���b�V�����\�[�X�̍쐬
	void CustomDrawBox::CreateMeshResource(){
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		//Face����6
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
		//�P�ӂ̒����͂̔�����0.5
		float size = 0.5f;
		for (int i = 0; i < FaceCount; i++)
		{
			//�@��
			auto normal = faceNormals[i];
			auto basis = (i >= 4) ? Vector3(0, 0, 1) : Vector3(0, 1, 0);
			auto side1 = Vector3EX::Cross(normal, basis);
			auto side2 = Vector3EX::Cross(normal, side1);
			//�C���f�b�N�X�̓o�^
			size_t vbase = vertices.size();
			indices.push_back((uint16_t)vbase + 0);
			indices.push_back((uint16_t)vbase + 1);
			indices.push_back((uint16_t)vbase + 2);
			indices.push_back((uint16_t)vbase + 0);
			indices.push_back((uint16_t)vbase + 2);
			indices.push_back((uint16_t)vbase + 3);
			//���_�̓o�^
			vertices.push_back(VertexPositionNormalTexture((normal - side1 - side2) * size, normal, textureCoordinates[0]));
			vertices.push_back(VertexPositionNormalTexture((normal - side1 + side2) * size, normal, textureCoordinates[1]));
			vertices.push_back(VertexPositionNormalTexture((normal + side1 + side2) * size, normal, textureCoordinates[2]));
			vertices.push_back(VertexPositionNormalTexture((normal + side1 - side2) * size, normal, textureCoordinates[3]));
		}
		//RH����LH�ɕύX
		if ((indices.size() % 3) != 0){
			throw BaseException(
				L"�C���f�b�N�X�̐��������܂���",
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
		//���_�ƃC���f�b�N�X�̔z�񂩂烁�b�V�����\�[�X���쐬�i���_��ύX�ł���j
		m_MeshResource = CommonMeshResource::CreateCommonMeshResource(vertices, indices,true);
	}

	//���_�̕ύX
	void CustomDrawBox::UpdateMeshResource(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
		//���W��ύX����
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		ID3D11DeviceContext* pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//���_�o�b�t�@�����\�[�X������o��
		auto pVertexBuffer = m_MeshResource->GetVertexBuffer().Get();
		//�o�b�N�A�b�v�̒��_�����o��
		vector<VertexPositionNormalTexture>& BacukVertices = m_MeshResource->GetBackupVertices();

		//D3D11_MAP_WRITE_DISCARD�͏d�v�B���̏����ɂ��AGPU�Ɏז�����Ȃ�
		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//���_�̃}�b�v
		if (FAILED(pID3D11DeviceContext->Map(pVertexBuffer, 0, mapType, 0, &mappedBuffer))){
			// Map���s
			throw BaseException(
				L"���_��Map�Ɏ��s���܂����B",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"CustomDrawBox::UpdateMeshResource()"
				);
		}
		//���_�̕ύX
		VertexPositionNormalTexture* vertices = (VertexPositionNormalTexture*)mappedBuffer.pData;

		for (size_t i = 0; i < m_MeshResource->GetNumVertices(); i++){
			Vector3 Pos = BacukVertices[i].position;
			if (Pos.x > 0 && Pos.y > 0 && Pos.z > 0){
				//�T�C�����g���Ă������藈���肷��悤�ɂ���
				Pos.y += sin(m_TotalTime);
			}
			else if (Pos.x < 0 && Pos.y > 0 && Pos.z < 0){
				//�T�C�����g���Ă������藈���肷��悤�ɂ���
				Pos.y -= sin(m_TotalTime);
			}
			vertices[i] = VertexPositionNormalTexture(
				Pos,
				BacukVertices[i].normal,
				BacukVertices[i].textureCoordinate
				);

		}


		//�A���}�b�v
		pID3D11DeviceContext->Unmap(pVertexBuffer, 0);

	}



	//�\�z�Ɣj��
	CustomDrawBox::CustomDrawBox(shared_ptr<Stage>& StagePtr, const Vector3& StartScale, const Vector3& StartRotation, const Vector3& StartPos) :
		GameObject(StagePtr),
		m_StartScale(StartScale),
		m_StartRotation(StartRotation),
		m_StartPos(StartPos),
		m_TotalTime(0)
	{}
	CustomDrawBox::~CustomDrawBox(){
	}
	//������
	void CustomDrawBox::Create(){
		//Transform�����͒ǉ����Ȃ��Ă��擾�ł���
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(m_StartScale);
		Ptr->SetPosition(m_StartPos);
		Ptr->SetRotation(m_StartRotation);
		//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬
		CreateMeshResource();
		//�V�F�[�_�̍쐬
		//�R���X�^���g�o�b�t�@
		m_ConstantBuffer = make_shared<CBuffer<ConstantBuffer>>();
		//���_�V�F�[�_
		m_VirtexShader = make_shared<VShader<VertexPositionNormalTexture>>(App::GetApp()->m_wstrRelativeShadersPath + L"VSSimpleBase.cso");
		//�s�N�Z���V�F�[�_�[
		m_PixelShader = make_shared<PShader>(App::GetApp()->m_wstrRelativeShadersPath + L"PSSimpleBase.cso");
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<Shadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(m_MeshResource);
		//���������i�`�揇����̂��߁j
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
		ConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		//�s��̐ݒ�(�]�u����)
		cb1.World = Matrix4X4EX::Transpose(PtrT->GetWorldMatrix());;
		cb1.View = Matrix4X4EX::Transpose(View);
		cb1.Projection = Matrix4X4EX::Transpose(Proj);
		//���C�g�̐ݒ�
		//�X�e�[�W����0�Ԗڂ̃��C�g�����o��
		auto PtrLight = GetStage()->GetTargetLight(0);
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;

		//�R���X�^���g�o�b�t�@�̍X�V
		pID3D11DeviceContext->UpdateSubresource(m_ConstantBuffer->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionNormalTexture);
		UINT offset = 0;
		//���_�o�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_MeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(m_MeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�X�e�[�^�X�̃|�C���^
		//�e�N�X�`�����擾
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pNullSR[1] = { 0 };
		//�e�N�X�`����ݒ�
		auto PtrTextureResource = App::GetApp()->GetResource<TextureResource>(L"TRACE_TX");
		pID3D11DeviceContext->PSSetShaderResources(0, 1, PtrTextureResource->GetShaderResourceView().GetAddressOf());
		//���j�A�T���v���[��ݒ�
		ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
		//����������
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);

		//�f�v�X�X�e���V���͎g�p����
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(m_VirtexShader->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(m_PixelShader->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(m_VirtexShader->GetInputLayout());
		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = m_ConstantBuffer->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
		//�`��
		pID3D11DeviceContext->DrawIndexed(m_MeshResource->GetNumIndicis(), 0, 0);
		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
		//�`��
		pID3D11DeviceContext->DrawIndexed(m_MeshResource->GetNumIndicis(), 0, 0);
		//��n��
		Dev->InitializeStates(RenderStatePtr);

	}


}
//endof  basedx11

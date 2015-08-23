#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class CBCustomShadowmap : public ConstantBuffer<CBCustomShadowmap,CustomShadowmapConstantBuffer>;
	//	�p�r: �J�X�^���R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<CBCustomShadowmap, CBCustomShadowmap::Deleter> CBCustomShadowmap::m_Ptr;
	//--------------------------------------------------------------------------------------
	//	class VSCustomShadowmap : public VertexShader<VSCustomShadowmap, VertexPositionNormalColor>;
	//	�p�r: VSCustomShadowmap���_�V�F�[�_
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<VSCustomShadowmap, VSCustomShadowmap::Deleter> VSCustomShadowmap::m_Ptr;
	//�\�z
	VSCustomShadowmap::VSCustomShadowmap() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSCustomShadowmap.cso")
	{}




	//--------------------------------------------------------------------------------------
	//	class CustomShadowmap : public Shadowmap;
	//	�p�r: �J�X�^���V���h�E�}�b�v�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	CustomShadowmap::CustomShadowmap(const shared_ptr<GameObject>& GameObjectPtr) :
		Shadowmap(GameObjectPtr){}
	CustomShadowmap::~CustomShadowmap(){}

	//����
	void CustomShadowmap::Draw(){
		//m_GameObject��null�Ȃ�Draw�ł��Ȃ�
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrStage = PtrGameObject->GetStage();
			if (!PtrStage){
				return;
			}
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			//�X�e�[�W����0�Ԗڂ̃��C�g�����o��
			auto PtrLight = PtrStage->GetTargetLight(0);
			//�X�e�[�W����J���������o��
			auto PtrCamera = PtrStage->GetTargetCamera();

			if (PtrT && PtrLight && PtrCamera){

				Matrix4X4 RealWorldMatrix = GetMeshToTransform() * PtrT->GetWorldMatrix();


				auto Dev = App::GetApp()->GetDeviceResources();
				auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
				//�X�e�[�^�X�̃|�C���^
				auto RenderStatePtr = PtrStage->GetRenderState();


				//���C�g�̎擾
				Matrix4X4 LightView, LightProj;

				Vector3 LightDir = -1.0 * PtrLight->GetDirectional();
				Vector3 LightAt = PtrCamera->GetAt();
				Vector3 LightEye = LightAt + (LightDir * GetLightHeight());

				//���C�g�̃r���[�Ǝˉe���v�Z
				LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
				LightProj.OrthographicLH(GetViewWidth(), GetViewHeight(), GetLightNear(), GetLightFar());

				CustomShadowmapConstantBuffer Cb;

				Cb.mWorld = Matrix4X4EX::Transpose(RealWorldMatrix);
				Cb.mView = Matrix4X4EX::Transpose(LightView);
				Cb.mProj = Matrix4X4EX::Transpose(LightProj);

				//������`�揈��
				//�R���X�^���g�o�b�t�@�̍X�V
				pID3D11DeviceContext->UpdateSubresource(CBCustomShadowmap::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);


				//�C���v�b�g���C�A�E�g�̃Z�b�g
				pID3D11DeviceContext->IASetInputLayout(VSCustomShadowmap::GetPtr()->GetInputLayout());
				//�X�g���C�h�ƃI�t�Z�b�g
				UINT stride = sizeof(VertexPositionNormalColor);
				UINT offset = 0;

				auto GameObj = dynamic_pointer_cast<CustomDrawOctahedron>(GetGameObject());
				//���_�o�b�t�@���Z�b�g

				pID3D11DeviceContext->IASetVertexBuffers(0, 1, GameObj->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				//���_�V�F�[�_�[�̃Z�b�g
				pID3D11DeviceContext->VSSetShader(VSCustomShadowmap::GetPtr()->GetShader(), nullptr, 0);

				//�C���f�b�N�X�o�b�t�@�̃Z�b�g
				pID3D11DeviceContext->IASetIndexBuffer(GameObj->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//�`����@�i3�p�`�j
				pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//�s�N�Z���V�F�[�_�̓Z�b�g���Ȃ��I
				pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
				//�W�I���g���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
				pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);

				//�R���X�^���g�o�b�t�@�̐ݒ�
				ID3D11Buffer* pConstantBuffer = CBCustomShadowmap::GetPtr()->GetBuffer();
				pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				//�R���X�^���g�o�b�t�@���s�N�Z���V�F�[�_�ɃZ�b�g
				pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
				//�`��
				pID3D11DeviceContext->DrawIndexed(GameObj->GetNumIndicis(), 0, 0);
				//��n��
				Dev->InitializeStates(RenderStatePtr);

			}

		}
	}


	//--------------------------------------------------------------------------------------
	//	class CBCustomDraw : public ConstantBuffer<CBCustomDraw,CustomDrawConstantBuffer>;
	//	�p�r: �J�X�^���R���X�^���g�o�b�t�@
	//--------------------------------------------------------------------------------------
	//�V���O���g������
	unique_ptr<CBCustomDraw, CBCustomDraw::Deleter> CBCustomDraw::m_Ptr;
	//--------------------------------------------------------------------------------------
	//	class VSCustomDraw : public VertexShader<VSCustomDraw, VertexPositionNormalColor>;
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
	//	class CustomDrawOctahedron : public GameObject;
	//	�Ǝ��̃V�F�[�_�ŕ`�悷��8�ʑ�
	//--------------------------------------------------------------------------------------
	//���b�V���̍쐬
	void CustomDrawOctahedron::CreateCustomMesh(){
		vector<VertexPositionNormalTexture> vertices;
		vector<uint16_t> indices;
		//��8�ʑ̂̍쐬
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

		//�~���[�e�b�N�X
		std::mutex Mutex;
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		ID3D11Device* pDx11Device = Dev->GetD3DDevice();
		//�o�b�t�@�̍쐬
		Util::DemandCreate(m_VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
		{
			//���_�o�b�t�@�̍쐬(���_�ύX�\)
			VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, m_BackupVirtex, pResult);
		});
		//���_���̐ݒ�
		m_NumVertices = static_cast<UINT>(vertices.size());
		Util::DemandCreate(m_IndexBuffer, Mutex, [&](ID3D11Buffer** pResult)
		{
			//�C���f�b�N�X�o�b�t�@�̍쐬
			VertexUtil::CreatePrimitiveBuffer(pDx11Device, indices, D3D11_BIND_INDEX_BUFFER, pResult);
		});
		//�C���f�b�N�X���̐ݒ�
		m_NumIndicis = static_cast<UINT>(indices.size());
	}

	//���_�̕ύX
	void CustomDrawOctahedron::UpdateCustomMesh(){

		float ElapsedTime = App::GetApp()->GetElapsedTime();
		m_TotalTime += ElapsedTime;
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
		auto* vertices = (VertexPositionNormalColor*)mappedBuffer.pData;

		for (size_t i = 0; i < m_NumVertices; i++){
			Vector3 Pos = m_BackupVirtex[i].position;
			if (Pos.y > 0){
				//�T�C�����g���Ă������藈���肷��悤�ɂ���
				Pos.y += sin(m_TotalTime);
			}
			vertices[i] = VertexPositionNormalColor(
				Pos,
				m_BackupVirtex[i].normal,
				m_BackupVirtex[i].color
				);
		}
		//�A���}�b�v
		pID3D11DeviceContext->Unmap(m_VertexBuffer.Get(), 0);
	}



	//�\�z�Ɣj��
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
	//������
	void CustomDrawOctahedron::Create(){
		//Transform�����͒ǉ����Ȃ��Ă��擾�ł���
		auto Ptr = GetComponent<Transform>();
		Ptr->SetScale(m_StartScale);
		Ptr->SetPosition(m_StartPos);
		Ptr->SetRotation(m_StartRotation);
		//���_�o�b�t�@�ƃC���f�b�N�X�o�b�t�@���쐬
		CreateCustomMesh();
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<CustomShadowmap>();
		//���������i�`�揇����̂��߁j
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
		//���C�g�̐ݒ�
		//�X�e�[�W����0�Ԗڂ̃��C�g�����o��
		auto PtrLight = GetStage()->GetTargetLight(0);
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;

		//�R���X�^���g�o�b�t�@�̍X�V
		pID3D11DeviceContext->UpdateSubresource(CBCustomDraw::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPositionNormalColor);
		UINT offset = 0;
		//���_�o�b�t�@�̐ݒ�
		pID3D11DeviceContext->IASetVertexBuffers(0, 1, m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(m_IndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�X�e�[�^�X�̃|�C���^
		//�e�N�X�`�����擾
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pNullSR[1] = { 0 };
		//����������
		pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
		//�f�v�X�X�e���V���͎g�p����
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
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
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
		//�`��
		pID3D11DeviceContext->DrawIndexed(m_NumIndicis, 0, 0);
		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
		//�`��
		pID3D11DeviceContext->DrawIndexed(m_NumIndicis, 0, 0);
		//��n��
		Dev->InitializeStates(RenderStatePtr);

	}


}
//endof  basedx11

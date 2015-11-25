#include "stdafx.h"
#include "Project.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	FixedBox::FixedBox(const shared_ptr<Stage>& StagePtr,
		const Vector3& Scale,
		const Vector3& Rotation,
		const Vector3& Position
		) :
		GameObject(StagePtr),
		m_Scale(Scale),
		m_Rotation(Rotation),
		m_Position(Position)
	{
	}
	FixedBox::~FixedBox(){}

	//������
	void FixedBox::Create(){
		auto PtrTransform = GetComponent<Transform>();
		PtrTransform->SetScale(m_Scale);
		PtrTransform->SetRotation(m_Rotation);
		PtrTransform->SetPosition(m_Position);

		//�X�e�[�g�}�V���̍\�z
		m_StateMachine = make_shared< StateMachine<FixedBox> >(GetThis<FixedBox>());
		//�ŏ��̃X�e�[�g��FixedBoxDefaultState�ɐݒ�
		m_StateMachine->SetCurrentState(FixedBoxDefaultState::Instance());
		//DefaultState�̏��������s���s��
		m_StateMachine->GetCurrentState()->Enter(GetThis<FixedBox>());

	}

	void FixedBox::Update(){
		//�X�e�[�g�}�V����Update���s��
		//���̒��ŃX�e�[�g�̐؂�ւ����s����
		m_StateMachine->Update();

	}


	void FixedBox::Update3(){
		if (m_StateMachine->GetCurrentState() == FixedBoxDefaultState::Instance()){
			//�}�l�[�W���Ɏ������g�̍s���ǉ�����
			auto ManagerPtr = GetStage()->GetSharedGameObject<DrawBoxManager>(L"DrawBoxManager");
			auto PtrTransform = GetComponent<Transform>();
			ManagerPtr->AddInstanceMatrix(PtrTransform->GetWorldMatrix());
		}
		else{
			//�����p�}�l�[�W���Ɏ������g�̕����s���ǉ�����
			auto ManagerPtr = GetStage()->GetSharedGameObject<DrawBoxManager>(L"DrawSplitBoxManager");
			for (auto& data : m_SplitBoxDataVec){
				if (data.m_Enabled){
					Matrix4X4 m;
					m.DefTransformation(data.m_Scale, data.m_Qt, data.m_Pos);
					ManagerPtr->AddInstanceMatrix(m);
				}
			}
		}
	}

	//���[�V�����n
	//�v���C���[���q�b�g������
	bool FixedBox::IsPlayerHit() const{
		auto PlayerPtr = GetStage()->GetSharedGameObject<Player>(L"Player");
		auto PtrPlayerTransform = PlayerPtr->GetComponent<Transform>();
		auto PtrTransform = GetComponent<Transform>();
		auto Len = Vector3EX::Length(PtrPlayerTransform->GetPosition() - PtrTransform->GetPosition());
		if (Len < 0.5f){
			return true;
		}
		return false;
	}

	//�����p�̍s��z��̍쐬
	void FixedBox::CreateSplitVec(size_t splitcount){
		m_SplitBoxDataVec.clear();
		if (splitcount <= 0){
			//��������0�Ȃ烊�^�[���i��O�ł��悢�j
			return;
		}
		auto PtrTransform = GetComponent<Transform>();
		Vector3 HalfBaseScale = PtrTransform->GetScale() / 2.0f;
		Vector3 Scale = PtrTransform->GetScale() / (float)splitcount;
		for (size_t x = 0; x < splitcount; x++){
			for (size_t y = 0; y < splitcount; y++){
				for (size_t z = 0; z < splitcount; z++){
					auto BasePos = PtrTransform->GetPosition();
					float PosX = (BasePos.x - HalfBaseScale.x + Scale.x * 0.5f) + x * Scale.x;
					float PosY = (BasePos.y - HalfBaseScale.y + Scale.y * 0.5f) + y * Scale.y;
					float PosZ = (BasePos.z - HalfBaseScale.z + Scale.z * 0.5f) + z * Scale.z;

					Vector3 Velo = Vector3(PosX, PosY, PosZ) - PtrTransform->GetPosition();
					float rv = Util::RandZeroToOne() * 4.0f;
					Velo *= rv;
					Velo.y = rv;
					float rx = (Util::RandZeroToOne() - 0.5f) / 2.0f;
					float ry = (Util::RandZeroToOne() - 0.5f) / 2.0f;
					float rz = (Util::RandZeroToOne() - 0.5f) / 2.0f;

					m_SplitBoxDataVec.push_back(
						SplitBoxData(Scale, Vector3(0, 0, 0), Vector3(PosX, PosY, PosZ), Velo, Vector3(rx,ry,rz))
						);

				}
			}
		}
	}

	//�����p�̍s��z��̍X�V
	void FixedBox::UpdateSplitVec(){
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		for (auto& data : m_SplitBoxDataVec){
			data.UpdateData(ElapsedTime);
		}
	}





	//--------------------------------------------------------------------------------------
	//	class FixedBoxDefaultState : public ObjState<FixedBox>;
	//	�p�r: �ʏ�
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr<FixedBoxDefaultState> FixedBoxDefaultState::Instance(){
		static shared_ptr<FixedBoxDefaultState> instance;
		if (!instance){
			instance = shared_ptr<FixedBoxDefaultState>(new FixedBoxDefaultState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void FixedBoxDefaultState::Enter(const shared_ptr<FixedBox>& Obj){
		//�������Ȃ�
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void FixedBoxDefaultState::Execute(const shared_ptr<FixedBox>& Obj){
		if (Obj->IsPlayerHit()){
			//�X�e�[�g�ύX
			Obj->GetStateMachine()->ChangeState(FixedBoxSplitState::Instance());
		}
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void FixedBoxDefaultState::Exit(const shared_ptr<FixedBox>& Obj){
		//�������Ȃ�
	}

	//--------------------------------------------------------------------------------------
	//	class  FixedBoxSplitState : public ObjState<FixedBox>;
	//	�p�r: �������
	//--------------------------------------------------------------------------------------
	//�X�e�[�g�̃C���X�^���X�擾
	shared_ptr< FixedBoxSplitState>  FixedBoxSplitState::Instance(){
		static shared_ptr< FixedBoxSplitState> instance;
		if (!instance){
			instance = shared_ptr< FixedBoxSplitState>(new  FixedBoxSplitState);
		}
		return instance;
	}
	//�X�e�[�g�ɓ������Ƃ��ɌĂ΂��֐�
	void  FixedBoxSplitState::Enter(const shared_ptr<FixedBox>& Obj){
		//�����̍s��̍쐬
		Obj->CreateSplitVec(4);
	}
	//�X�e�[�g���s���ɖ��^�[���Ă΂��֐�
	void  FixedBoxSplitState::Execute(const shared_ptr<FixedBox>& Obj){
		Obj->UpdateSplitVec();
	}
	//�X�e�[�g�ɂ��甲����Ƃ��ɌĂ΂��֐�
	void  FixedBoxSplitState::Exit(const shared_ptr<FixedBox>& Obj){
		//�������Ȃ�
	}




	//�V�F�[�_�[����
	//�V���h�E�}�b�v�R���X�^���g�o�b�t�@�V���O���g������
	unique_ptr<CBInstanceShadowmap, CBInstanceShadowmap::Deleter> CBInstanceShadowmap::m_Ptr;
	//�V���h�E�}�b�v���_�V�F�[�_�V���O���g������
	unique_ptr< VSInstanceShadowmap, VSInstanceShadowmap::Deleter>  VSInstanceShadowmap::m_Ptr;
	//���_�V�F�[�_����
	VSInstanceShadowmap::VSInstanceShadowmap() :
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSInstanceShadowmap.cso")
	{}


	//--------------------------------------------------------------------------------------
	//	class InstanceShadowmap : public Shadowmap;
	//	�p�r: �C���X�^���X�V���h�E�}�b�v�R���|�[�l���g
	//--------------------------------------------------------------------------------------
	InstanceShadowmap::InstanceShadowmap(const shared_ptr<GameObject>& GameObjectPtr) :
		Shadowmap(GameObjectPtr){}
	InstanceShadowmap::~InstanceShadowmap(){}

	//����
	void InstanceShadowmap::Draw(){
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

				InstanceShadowmapConstantBuffer Cb;
				Cb.mView = Matrix4X4EX::Transpose(LightView);
				Cb.mProj = Matrix4X4EX::Transpose(LightProj);

				//������`�揈��
				//�R���X�^���g�o�b�t�@�̍X�V
				pID3D11DeviceContext->UpdateSubresource(CBInstanceShadowmap::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);

				//�C���v�b�g���C�A�E�g�̃Z�b�g
				pID3D11DeviceContext->IASetInputLayout(VSInstanceShadowmap::GetPtr()->GetInputLayout());
				//�}�l�[�W�����擾
				auto GameObj = dynamic_pointer_cast<DrawBoxManager>(GetGameObject());
				if (GameObj->GetInstanceVec().size() <= 0){
					return;
				}
				//�`��̒��_�o�b�t�@�ƍs��o�b�t�@��ݒ�
				UINT stride[2] = { sizeof(VertexPositionNormalTexture), sizeof(Matrix4X4) };
				UINT offset[2] = { 0, 0 };
				ID3D11Buffer* pBuf[2] = { GetMeshResource()->GetVertexBuffer().Get(), GameObj->GetMatrixBuffer().Get() };
				//���_�o�b�t�@���Z�b�g
				pID3D11DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);
				//���_�V�F�[�_�[�̃Z�b�g
				pID3D11DeviceContext->VSSetShader(VSInstanceShadowmap::GetPtr()->GetShader(), nullptr, 0);
				//�C���f�b�N�X�o�b�t�@�̃Z�b�g
				pID3D11DeviceContext->IASetIndexBuffer(GetMeshResource()->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//�`����@�i3�p�`�j
				pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
				//�s�N�Z���V�F�[�_�̓Z�b�g���Ȃ��I
				pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
				//�W�I���g���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
				pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);
				//�R���X�^���g�o�b�t�@�̐ݒ�
				ID3D11Buffer* pConstantBuffer = CBInstanceShadowmap::GetPtr()->GetBuffer();
				pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				//�R���X�^���g�o�b�t�@���s�N�Z���V�F�[�_�ɃZ�b�g
				pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
				//�`��
				//�C���X�^���X�`��(�C���X�^���X�z��̐������`��)
				pID3D11DeviceContext->DrawIndexedInstanced(GetMeshResource()->GetNumIndicis(), GameObj->GetInstanceVec().size(), 0, 0, 0);
				//��n��
				Dev->InitializeStates(RenderStatePtr);

			}

		}
	}


	//�R���X�^���g�o�b�t�@�V���O���g������
	unique_ptr<CBInstance, CBInstance::Deleter> CBInstance::m_Ptr;
	//���_�V�F�[�_�V���O���g������
	unique_ptr<VSInstance, VSInstance::Deleter> VSInstance::m_Ptr;
	//���_�V�F�[�_����
	VSInstance::VSInstance() : 
		VertexShader(App::GetApp()->m_wstrRelativeShadersPath + L"VSInstance.cso")
	{}
	//�s�N�Z���V�F�[�_�V���O���g������
	unique_ptr<PSInstance, PSInstance::Deleter> PSInstance::m_Ptr;
	//�s�N�Z���V�F�[�_����
	PSInstance::PSInstance() :
		PixelShader(App::GetApp()->m_wstrRelativeShadersPath + L"PSInstance.cso")
	{}

	//--------------------------------------------------------------------------------------
	//	class DrawBoxManager : public GameObject;
	//	�p�r: �Œ�̃{�b�N�X�̕`��}�l�[�W��
	//--------------------------------------------------------------------------------------
	//�s��p�̒��_�o�b�t�@�̍쐬
	void DrawBoxManager::CreateMatrixBuffer(){
		//�s��o�b�t�@�̍쐬
		//Max�l�ō쐬����
		vector<Matrix4X4> matrices(m_MaxInstance);
		for (auto& m : matrices){
			m = Matrix4X4();
		}
		VertexUtil::CreateDynamicVertexBuffer(m_MatrixBuffer, matrices);
	}

	//�\�z�Ɣj��
	DrawBoxManager::DrawBoxManager(const shared_ptr<Stage>& StagePtr,
		size_t MaxInstance, const wstring& MeshKey, const wstring& TextureKey) :
		GameObject(StagePtr),
		m_MaxInstance(MaxInstance),
		m_MeshKey(MeshKey),
		m_TextureKey(TextureKey)
	{}
	DrawBoxManager::~DrawBoxManager(){}

	void DrawBoxManager::Create(){
		//�s��o�b�t�@�̍쐬
		CreateMatrixBuffer();
		//�e������i�V���h�E�}�b�v��`�悷��j
		auto ShadowPtr = AddComponent<InstanceShadowmap>();
		//�e�̌`�i���b�V���j��ݒ�
		ShadowPtr->SetMeshResource(m_MeshKey);

	}
	//����
	//�X�P�[���A��]�A�ʒu�Œǉ�
	void DrawBoxManager::AddInstanceVectors(const Vector3& Scale, const Vector3& Rot, const Vector3& Pos){
		if (m_InstanceVec.size() >= m_MaxInstance){
			wstring strsz = Util::UintToWStr(m_InstanceVec.size());
			wstring strmax = Util::UintToWStr(m_MaxInstance);
			strsz += L">=";
			strsz += strmax;
			throw BaseException(
				L"����ȏ�I�u�W�F�N�g��ǉ��ł��܂���B",
				strsz,
				L"DrawBoxManager::AddInstanceVectors()"
				);
		}
		Matrix4X4 mat;
		mat.DefTransformation(Scale, Rot, Pos);
		m_InstanceVec.push_back(mat);
	}
	//�s��Œǉ�
	void DrawBoxManager::AddInstanceMatrix(const Matrix4X4& Mat){
		if (m_InstanceVec.size() >= m_MaxInstance){
			wstring strsz = Util::UintToWStr(m_InstanceVec.size());
			wstring strmax = Util::UintToWStr(m_MaxInstance);
			strsz += L">=";
			strsz += strmax;
			throw BaseException(
				L"����ȏ�I�u�W�F�N�g��ǉ��ł��܂���B",
				strsz,
				L"DrawBoxManager::AddInstanceMatrix()"
				);
		}
		m_InstanceVec.push_back(Mat);
	}
	//���z�֐�
	void DrawBoxManager::Update3(){
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();

		D3D11_MAP mapType = D3D11_MAP_WRITE_DISCARD;
		D3D11_MAPPED_SUBRESOURCE mappedBuffer;
		//�s��̃}�b�v
		if (FAILED(pID3D11DeviceContext->Map(m_MatrixBuffer.Get(), 0, mapType, 0, &mappedBuffer))){
			// Map���s
			throw BaseException(
				L"�s���Map�Ɏ��s���܂����B",
				L"if(FAILED(pID3D11DeviceContext->Map()))",
				L"DrawBoxManager::Update3()"
				);
		}
		//�s��̕ύX�i�C���X�^���X�z��ɓo�^����Ă��鐔�����ݒ�j
		auto* matrices = (Matrix4X4*)mappedBuffer.pData;
		for (size_t i = 0; i < m_InstanceVec.size(); i++){
			matrices[i] = Matrix4X4EX::Transpose(m_InstanceVec[i]);
		}
		//�A���}�b�v
		pID3D11DeviceContext->Unmap(m_MatrixBuffer.Get(), 0);

	}


	void DrawBoxManager::Draw(){
		//�f�o�C�X�̎擾
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
		//�X�e�[�^�X�̃|�C���^
		auto RenderStatePtr = GetStage()->GetRenderState();

		//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
		auto ShadoumapPtr = GetStage()->GetShadowMapRenderTarget();

		auto PtrT = GetComponent<Transform>();
		//�X�e�[�W����J���������o��
		auto PtrCamera = GetStage()->GetTargetCamera();
		//�J�����̎擾
		Matrix4X4 View, Proj, WorldViewProj;
		View = PtrCamera->GetViewMatrix();
		Proj = PtrCamera->GetProjMatrix();

		//�`�悷�郁�b�V�����\�[�X���擾
		auto MeshRes = App::GetApp()->GetResource<MeshResource>(m_MeshKey);

		//�R���X�^���g�o�b�t�@�̐ݒ�
		InstanceConstantBuffer cb1;
		ZeroMemory(&cb1, sizeof(cb1));
		cb1.View = Matrix4X4EX::Transpose(View);
		cb1.Projection = Matrix4X4EX::Transpose(Proj);
		//���C�g�̐ݒ�
		//�X�e�[�W����0�Ԗڂ̃��C�g�����o��
		auto PtrLight = GetStage()->GetTargetLight(0);
		cb1.LightDir = PtrLight->GetDirectional();
		cb1.LightDir.w = 1.0f;
		Matrix4X4 LightView, LightProj, LightViewProj;
		Vector3 LightDir = -1.0 * PtrLight->GetDirectional();
		Vector3 LightAt = PtrCamera->GetAt();
		Vector3 LightEye = LightDir;
		LightEye *= Shadowmap::GetLightHeight();
		LightEye = LightAt + LightEye;

		//���C�g�̃r���[�Ǝˉe���v�Z
		LightView.LookAtLH(LightEye, LightAt, Vector3(0, 1.0f, 0));
		LightProj.OrthographicLH(Shadowmap::GetViewWidth(), Shadowmap::GetViewHeight(),
			Shadowmap::GetLightNear(), Shadowmap::GetLightFar());
		LightViewProj = LightView * LightProj;
		Matrix4X4 LWMatrix = PtrT->GetWorldMatrix() * LightViewProj;
		//�R���X�^���g�o�b�t�@�ɐݒ�
		cb1.LPos = LightEye;
		cb1.LPos.w = 0;
		cb1.EyePos = PtrCamera->GetEye();
		cb1.EyePos.w = 0;
		cb1.LView = Matrix4X4EX::Transpose(LightView);
		cb1.LProjection = Matrix4X4EX::Transpose(LightProj);

		//�R���X�^���g�o�b�t�@�̍X�V
		pID3D11DeviceContext->UpdateSubresource(CBInstance::GetPtr()->GetBuffer(), 0, nullptr, &cb1, 0, 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		//�`��̒��_�o�b�t�@�ƍs��o�b�t�@��ݒ�
		UINT stride[2] = { sizeof(VertexPositionNormalTexture), sizeof(Matrix4X4) };
		UINT offset[2] = { 0, 0 };
		ID3D11Buffer* pBuf[2] = { MeshRes->GetVertexBuffer().Get(), m_MatrixBuffer.Get() };
		pID3D11DeviceContext->IASetVertexBuffers(0, 2, pBuf, stride, offset);
		//�C���f�b�N�X�o�b�t�@�̃Z�b�g
		pID3D11DeviceContext->IASetIndexBuffer(MeshRes->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
		//�`����@�i3�p�`�j
		pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//�X�e�[�^�X�̃|�C���^
		//�e�N�X�`�����擾
		ID3D11ShaderResourceView* pNull[1] = { 0 };
		ID3D11SamplerState* pNullSR[1] = { 0 };
		//�e�N�X�`����ݒ�
		auto PtrTextureResource = App::GetApp()->GetResource<TextureResource>(m_TextureKey);
		pID3D11DeviceContext->PSSetShaderResources(0, 1, PtrTextureResource->GetShaderResourceView().GetAddressOf());
		//���j�A�T���v���[��ݒ�
		ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
		pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);


		//�V���h�E�}�b�v�̃��\�[�X�r���[���擾
		ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();
		pID3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
		//�V���h�E�p�Ƀ��j�A�T���v���[���擾
		ID3D11SamplerState* pShadowSamplerState = RenderStatePtr->GetLinearClamp();
		//�V���h�E�p���j�A�T���v���[��ݒ�
		pID3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSamplerState);


		//�f�v�X�X�e���V���͎g�p����
		pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
		//�V�F�[�_�̐ݒ�
		pID3D11DeviceContext->VSSetShader(VSInstance::GetPtr()->GetShader(), nullptr, 0);
		pID3D11DeviceContext->PSSetShader(PSInstance::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pID3D11DeviceContext->IASetInputLayout(VSInstance::GetPtr()->GetInputLayout());
		//�R���X�^���g�o�b�t�@�̐ݒ�
		ID3D11Buffer* pConstantBuffer = CBInstance::GetPtr()->GetBuffer();
		pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�����_�����O�X�e�[�g
		pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
		//�C���X�^���X�`��(�C���X�^���X�z��̐������`��)
		pID3D11DeviceContext->DrawIndexedInstanced(MeshRes->GetNumIndicis(), m_InstanceVec.size(), 0, 0, 0);
		//��n��
		Dev->InitializeStates(RenderStatePtr);
		//�C���X�^���X�z�񂪑�������\��������̂Ŕz����N���A
		m_InstanceVec.clear();
	}



}
//endof  basedx11

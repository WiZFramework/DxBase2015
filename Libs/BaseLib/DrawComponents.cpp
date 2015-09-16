#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	struct Shadowmap::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct Shadowmap::Impl{
		static float m_LightHeight;	//���C�g�̍����i���������̒l�Ŋ|����j
		static float m_LightNear;	//���C�g��Near
		static float m_LightFar;		//���C�g��Far
		static float m_ViewWidth;
		static float m_ViewHeight;

		weak_ptr<MeshResource> m_MeshResource;	//���b�V�����\�[�X

		Matrix4X4 m_MeshToTransform;

		Impl():
			m_MeshToTransform()
		{}
		~Impl(){}
	};

	float Shadowmap::Impl::m_LightHeight(20.0f);
	float Shadowmap::Impl::m_LightNear(1.0f);
	float Shadowmap::Impl::m_LightFar(200.0f);
	float Shadowmap::Impl::m_ViewWidth(64.0f);
	float Shadowmap::Impl::m_ViewHeight(64.0f);


	//--------------------------------------------------------------------------------------
	//	class Shadowmap : public Component;
	//	�p�r: �V���h�E�}�b�v�R���|�[�l���g�i�O�����p�j
	//--------------------------------------------------------------------------------------
	Shadowmap::Shadowmap(const shared_ptr<GameObject>& GameObjectPtr) :
		Component(GameObjectPtr),
		pImpl(new Impl())
	{}
	Shadowmap::~Shadowmap(){}

	float Shadowmap::GetLightHeight(){ return Impl::m_LightHeight; }
	float Shadowmap::GetLightNear(){ return  Impl::m_LightNear; }
	float Shadowmap::GetLightFar(){ return  Impl::m_LightFar; }
	float Shadowmap::GetViewWidth(){ return  Impl::m_ViewWidth; }
	float Shadowmap::GetViewHeight(){ return  Impl::m_ViewHeight; }

	void Shadowmap::SetLightHeight(float f){ Impl::m_LightHeight = f; }
	void Shadowmap::SetLightNear(float f){ Impl::m_LightNear = f; }
	void Shadowmap::SetLightFar(float f){ Impl::m_LightFar = f; }
	void Shadowmap::SetViewWidth(float f){ Impl::m_ViewWidth = f; }
	void Shadowmap::SetViewHeight(float f){ Impl::m_ViewHeight = f; }


	shared_ptr<MeshResource> Shadowmap::GetMeshResource(bool ExceptionActive) const{
		if (!pImpl->m_MeshResource.expired()){
			return pImpl->m_MeshResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"���b�V�����\�[�X��������܂���",
					L"if (pImpl->m_MeshResource.expired())",
					L"ShadowmapComp::GetMeshResource()"
					);
			}
		}
		return nullptr;
	}


	void Shadowmap::SetMeshResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"���b�V���L�[���󔒂ł�",
					L"if (ResKey == L\"\"",
					L"ShadowmapComp::SetMeshResource()"
					);
			}
			pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(ResKey);
		}
		catch (...){
			throw;
		}
	}
	void Shadowmap::SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr){
		pImpl->m_MeshResource = MeshResourcePtr;
	}

	const Matrix4X4& Shadowmap::GetMeshToTransform() const{
		return pImpl->m_MeshToTransform;
	}
	void Shadowmap::SetMeshToTransform(const Matrix4X4& Mat){
		pImpl->m_MeshToTransform = Mat;
	}



	void Shadowmap::Draw(){
		//m_GameObject��null�Ȃ�Draw�ł��Ȃ�
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrStage = PtrGameObject->GetStage();
			if (!PtrStage){
				return;
			}
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			//���b�V�����\�[�X�̎擾
			auto PtrMeshResource = GetMeshResource();
			//�X�e�[�W����0�Ԗڂ̃��C�g�����o��
			auto PtrLight = PtrStage->GetTargetLight(0);
			//�X�e�[�W����J���������o��
			auto PtrCamera = PtrStage->GetTargetCamera();

			if (PtrT && PtrMeshResource && PtrLight && PtrCamera){

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

				ShadowConstantBuffer Cb;

				Cb.mWorld = Matrix4X4EX::Transpose(RealWorldMatrix);
				Cb.mView = Matrix4X4EX::Transpose(LightView);
				Cb.mProj = Matrix4X4EX::Transpose(LightProj);

				bool IsSkin = false;
				bool IsSkinStride = false;
				if (PtrMeshResource->IsSkining()){
					auto DrawCompPtr = GetGameObject()->GetDynamicComponent<DrawComponent>(false);
					if (auto* pLocalBoneVec = DrawCompPtr->GetVecLocalBonesPtr()){
						if (pLocalBoneVec){
							//�{�[���̐ݒ�
							size_t BoneSz = pLocalBoneVec->size();
							UINT cb_count = 0;
							for (size_t b = 0; b < BoneSz; b++){
								Matrix4X4 mat = (*pLocalBoneVec)[b].m_ConbinedPose;
								mat.Transpose();
								Cb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
								Cb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
								Cb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
								cb_count += 3;
							}
							IsSkin = true;
						}
					}
					IsSkinStride = true;
				}


				//������`�揈��
				//�R���X�^���g�o�b�t�@�̍X�V
				pID3D11DeviceContext->UpdateSubresource(CBShadow::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);


				if (IsSkin){
					//�C���v�b�g���C�A�E�g�̃Z�b�g
					pID3D11DeviceContext->IASetInputLayout(VSShadowmapBone::GetPtr()->GetInputLayout());
					//�X�g���C�h�ƃI�t�Z�b�g
					UINT stride = sizeof(VertexPositionNormalTextureSkinning);
					UINT offset = 0;
					//���_�o�b�t�@���Z�b�g
					pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
					//���_�V�F�[�_�[�̃Z�b�g
					pID3D11DeviceContext->VSSetShader(VSShadowmapBone::GetPtr()->GetShader(), nullptr, 0);
				}
				else{
					//�C���v�b�g���C�A�E�g�̃Z�b�g
					pID3D11DeviceContext->IASetInputLayout(VSShadowmap::GetPtr()->GetInputLayout());
					//�X�g���C�h�ƃI�t�Z�b�g
					UINT stride = sizeof(VertexPositionNormalTexture);
					if (IsSkinStride){
						stride = sizeof(VertexPositionNormalTextureSkinning);
					}
					UINT offset = 0;
					//���_�o�b�t�@���Z�b�g
					pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
					//���_�V�F�[�_�[�̃Z�b�g
					pID3D11DeviceContext->VSSetShader(VSShadowmap::GetPtr()->GetShader(), nullptr, 0);
				}



				//�C���f�b�N�X�o�b�t�@�̃Z�b�g
				pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//�`����@�i3�p�`�j
				pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//�s�N�Z���V�F�[�_�̓Z�b�g���Ȃ��I
				pID3D11DeviceContext->PSSetShader(nullptr, nullptr, 0);
				//�W�I���g���V�F�[�_�̐ݒ�i�g�p���Ȃ��j
				pID3D11DeviceContext->GSSetShader(nullptr, nullptr, 0);

				//�R���X�^���g�o�b�t�@�̐ݒ�
				ID3D11Buffer* pConstantBuffer = CBShadow::GetPtr()->GetBuffer();
				ID3D11Buffer* pNullConstantBuffer = nullptr;
				pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				//�R���X�^���g�o�b�t�@���s�N�Z���V�F�[�_�ɃZ�b�g
				pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pNullConstantBuffer);
				//�`��
				pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
				//��n��
				Dev->InitializeStates(RenderStatePtr);

			}

		}
	}

	//--------------------------------------------------------------------------------------
	//	struct SimpleDirectDraw::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SimpleDirectDraw::Impl{
		Color4 m_Diffuse;	//�f�t�B�[�Y
		//�w�ʂ��N���A���邽�߂̃o�b�t�@
		ComPtr<ID3D11Buffer> m_VertexBuffer;	//���_�o�b�t�@
		UINT m_NumVertices;				//���_�̐�
		D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveTopology;	//�`����@
		Impl() :
			m_Diffuse(0.5f, 0.5f, 0.5f, 1.0f),
			m_NumVertices(0),
			m_PrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class SimpleDirectDraw : public DrawComponent;
	//	�p�r: SimpleDirectDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	SimpleDirectDraw::SimpleDirectDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	SimpleDirectDraw::~SimpleDirectDraw(){}
	//�A�N�Z�T
	const Color4& SimpleDirectDraw::GetDiffuse() const { return pImpl->m_Diffuse; }
	void SimpleDirectDraw::SetDiffuse(const Color4& c){ pImpl->m_Diffuse = c; }

	D3D11_PRIMITIVE_TOPOLOGY SimpleDirectDraw::GetPrimitiveTopology() const{
		return pImpl->m_PrimitiveTopology;
	}
	void SimpleDirectDraw::SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY t){
		pImpl->m_PrimitiveTopology = t;
	}


	//���_�z���n���Ē��_�o�b�t�@���쐬����
	void SimpleDirectDraw::CreateVertex(const vector<VertexPosition>& Verteces){
		try{
			VertexUtil::CreateVertexBuffer(pImpl->m_VertexBuffer, Verteces);
			//���_���̐ݒ�
			pImpl->m_NumVertices = static_cast<UINT>(Verteces.size());
		}
		catch (...){
			throw;
		}
	}

	void SimpleDirectDraw::Draw(){
		auto PtrGameObject = GetGameObject();
		auto PtrStage = PtrGameObject->GetStage();
		if (!PtrStage){
			//�����X�e�[�W��null��������A�I�u�W�F�N�g���X�e�[�W�̉\��������
			PtrStage = dynamic_pointer_cast<Stage>(PtrGameObject);
			if (!PtrStage){
				return;
			}
		}
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pD3D11DeviceContext = Dev->GetD3DDeviceContext();

		auto RenderStatePtr = PtrStage->GetRenderState();

		//�R���X�^���g�o�b�t�@�̏���
		SimpleDirectConstantBuffer sb;
		sb.m_DiffuseColor = GetDiffuse();
		//�R���X�^���g�o�b�t�@�̍X�V
		pD3D11DeviceContext->UpdateSubresource(CBSimpleDirect::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);
		//�X�g���C�h�ƃI�t�Z�b�g
		UINT stride = sizeof(VertexPosition);
		UINT offset = 0;
		pD3D11DeviceContext->IASetVertexBuffers(0, 1, pImpl->m_VertexBuffer.GetAddressOf(), &stride, &offset);
		//�`����@�i3�p�`�j
		pD3D11DeviceContext->IASetPrimitiveTopology(GetPrimitiveTopology());

		ID3D11Buffer* pNullConstantBuffer = nullptr;
		//�R���X�^���g�o�b�t�@�̐ݒ�(�s�N�Z���V�F�[�_�݂̂ł悢)
		ID3D11Buffer* pConstantBuffer = CBSimpleDirect::GetPtr()->GetBuffer();
		pD3D11DeviceContext->VSSetConstantBuffers(0, 1, &pNullConstantBuffer);
		pD3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
		//�V�F�[�_�̐ݒ�
		pD3D11DeviceContext->VSSetShader(VSSimpleDirect::GetPtr()->GetShader(), nullptr, 0);
		pD3D11DeviceContext->PSSetShader(PSSimpleDirect::GetPtr()->GetShader(), nullptr, 0);
		//�C���v�b�g���C�A�E�g�̐ݒ�
		pD3D11DeviceContext->IASetInputLayout(VSSimpleDirect::GetPtr()->GetInputLayout());

		pD3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
		//�f�v�X�X�e���V���͎g�p���Ȃ�
		pD3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);
		//
		pD3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);

		//�`��
		pD3D11DeviceContext->Draw(pImpl->m_NumVertices, 0);
		//��n��
		Dev->InitializeStates(RenderStatePtr);

	}






	//--------------------------------------------------------------------------------------
	//	struct SimplePNTDraw::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct SimplePNTDraw::Impl{
		Color4 m_Diffuse;	//�f�t�B�[�Y
		bool m_OwnShadowActive;
		weak_ptr<MeshResource> m_MeshResource;	//���b�V�����\�[�X
		weak_ptr<TextureResource> m_TextureResource;	//�e�N�X�`�����\�[�X
		ID3D11SamplerState* m_pSamplerState;	//�T���v���[�X�e�[�g�i�I�v�V�����j
		bool m_ZBufferUse;		//Z�o�b�t�@��L���ɂ��邩�ǂ���
		bool m_AlphaBlendSrcOne;	//����������SRC_ONE�ݒ�
		size_t m_Virsion;		//�V�F�[�_�[�̃o�[�W����
		Impl() :
			m_Diffuse(0.5f, 0.5f, 0.5f, 1.0f),
			m_OwnShadowActive(false),
			m_pSamplerState(nullptr),
			m_ZBufferUse(true),
			m_AlphaBlendSrcOne(false),
			m_Virsion(2)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class SimplePNTDraw : public Component;
	//	�p�r: SimplePNTDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	SimplePNTDraw::SimplePNTDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	SimplePNTDraw::~SimplePNTDraw(){}
	//�A�N�Z�T
	const Color4& SimplePNTDraw::GetDiffuse() const { return pImpl->m_Diffuse; }
	void SimplePNTDraw::SetDiffuse(const Color4& c){ pImpl->m_Diffuse = c; }

	bool SimplePNTDraw::GetOwnShadowActive() const { return pImpl->m_OwnShadowActive; }
	bool SimplePNTDraw::IsOwnShadowActive() const { return pImpl->m_OwnShadowActive; }
	void SimplePNTDraw::SetOwnShadowActive(bool b){ pImpl->m_OwnShadowActive = b; }

	bool SimplePNTDraw::GetZBufferUse() const{
		return pImpl->m_ZBufferUse;

	}
	bool SimplePNTDraw::IsZBufferUse() const{
		return pImpl->m_ZBufferUse;
	}
	void SimplePNTDraw::SetZBufferUse(bool b){
		pImpl->m_ZBufferUse = b;
	}

	bool SimplePNTDraw::IsAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	bool SimplePNTDraw::GetAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	void SimplePNTDraw::SetAlphaBlendSrcOne(bool b){
		pImpl->m_AlphaBlendSrcOne = b;
	}



	size_t SimplePNTDraw::GetShaderVirsion() const{
		return pImpl->m_Virsion;
	}
	void SimplePNTDraw::SetShaderVirsion(size_t v){
		if (v <= 1){
			v = 1;
		}
		else if (v >= 2){
			v = 2;
		}
		pImpl->m_Virsion = v;
	}



	void SimplePNTDraw::SetSamplerState(ID3D11SamplerState* pSamplerState){
		pImpl->m_pSamplerState = pSamplerState;
	}


	shared_ptr<MeshResource> SimplePNTDraw::GetMeshResource(bool ExceptionActive) const{
		if (!pImpl->m_MeshResource.expired()){
			return pImpl->m_MeshResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"���b�V�����\�[�X��������܂���",
					L"if (pImpl->m_MeshResource.expired())",
					L"SimplePNTDraw::GetMeshResource()"
					);
			}
		}
		return nullptr;
	}
	void SimplePNTDraw::SetMeshResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"���b�V���L�[���󔒂ł�",
					L"if (ResKey == L\"\"",
					L"SimplePNTDraw::SetMeshResource()"
					);
			}
			pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(ResKey);
		}
		catch (...){
			throw;
		}
	}
	void SimplePNTDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr){
		pImpl->m_MeshResource = MeshResourcePtr;
	}

	shared_ptr<TextureResource> SimplePNTDraw::GetTextureResource(bool ExceptionActive) const{
		if (!pImpl->m_TextureResource.expired()){
			return pImpl->m_TextureResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"�e�N�X�`�����\�[�X��������܂���",
					L"if (pImpl->m_Texture.expired())",
					L"SimplePNTDraw::GetTextureResource()"
					);
			}
		}
		return nullptr;
	}
	void SimplePNTDraw::SetTextureResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"�e�N�X�`���L�[���󔒂ł�",
					L"if (ResKey == L\"\"",
					L"SimplePNTDraw::SetTextureResource()"
					);
			}
			pImpl->m_TextureResource = App::GetApp()->GetResource<TextureResource>(ResKey);
		}
		catch (...){
			throw;
		}
	}
	void SimplePNTDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr){
		pImpl->m_TextureResource = TextureResourcePtr;
	}

	void SimplePNTDraw::Draw(){
		auto PtrGameObject = GetGameObject();
		auto PtrStage = PtrGameObject->GetStage();
		if (!PtrStage){
			return;
		}
		auto PtrT = PtrGameObject->GetComponent<Transform>();
		auto PtrMeshResource = GetMeshResource();
		//�e�N�X�`���͕K�{�ł͂Ȃ�
		auto PtrTextureResource = GetTextureResource(false);

		//�X�e�[�W����J���������o��
		auto PtrCamera = PtrStage->GetTargetCamera();
		//�X�e�[�W����0�Ԗڂ̃��C�g�����o��
		auto PtrLight = PtrStage->GetTargetLight(0);

		if (PtrMeshResource && PtrCamera && PtrLight){

			auto Dev = App::GetApp()->GetDeviceResources();
			auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
			//�X�e�[�^�X�̃|�C���^
			auto RenderStatePtr = PtrStage->GetRenderState();
			//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
			auto ShadoumapPtr = PtrStage->GetShadowMapRenderTarget();

			//�J�����̎擾
			Matrix4X4 View, Proj;
			View = PtrCamera->GetViewMatrix();
			Proj = PtrCamera->GetProjMatrix();

			//�R���X�^���g�o�b�t�@�̐ݒ�
			SimpleConstantBuffer sb;
			sb.m_Model = Matrix4X4EX::Transpose(PtrT->GetWorldMatrix());
			sb.m_View = Matrix4X4EX::Transpose(View);
			sb.m_Projection = Matrix4X4EX::Transpose(Proj);
			sb.m_Direction = PtrLight->GetDirectional();
			sb.m_DiffuseColor = pImpl->m_Diffuse;
			if (IsOwnShadowActive()){
				//���C�g�̎擾
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
				sb.m_LightWorldViewProj = Matrix4X4EX::Transpose(LWMatrix);
				sb.m_ActiveFlg.y = 1;
				sb.m_LPos = LightEye;
				sb.m_LPos.w = 0;
				sb.m_LView = Matrix4X4EX::Transpose(LightView);
				sb.m_LProjection = Matrix4X4EX::Transpose(LightProj);
				sb.m_EyePos = PtrCamera->GetEye();
				sb.m_EyePos.w = 0;
			}
			if (PtrTextureResource){
				sb.m_ActiveFlg.x = 1;
			}


			//�R���X�^���g�o�b�t�@�̍X�V
			pID3D11DeviceContext->UpdateSubresource(CBSimple::GetPtr()->GetBuffer(), 0, nullptr, &sb, 0, 0);


			//�X�g���C�h�ƃI�t�Z�b�g
			UINT stride = sizeof(VertexPositionNormalTexture);
			UINT offset = 0;
			//���_�o�b�t�@�̐ݒ�
			pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
			//�C���f�b�N�X�o�b�t�@�̃Z�b�g
			pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
			//�`����@�i3�p�`�j
			pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


			ID3D11ShaderResourceView* pNull[1] = { 0 };
			ID3D11SamplerState* pNullSR[1] = { 0 };

			if (PtrTextureResource){
				//���j�A�T���v���[��ݒ�
				ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
				if (pImpl->m_pSamplerState){
					samplerState = pImpl->m_pSamplerState;
				}
				pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
				//�e�N�X�`����ݒ�
				pID3D11DeviceContext->PSSetShaderResources(0, 1, PtrTextureResource->GetShaderResourceView().GetAddressOf());
			}
			else{
				//�T���v���[�͎g�p���Ȃ�
				pID3D11DeviceContext->PSSetSamplers(0, 1, pNullSR);
				//�e�N�X�`�����g�p���Ȃ�
				pID3D11DeviceContext->PSSetShaderResources(0, 1, pNull);
			}

			if (GetShaderVirsion() >= 2){
				//�s�N�Z���V�F�[�_�̐ݒ�
				pID3D11DeviceContext->PSSetShader(PSSimplePNT2::GetPtr()->GetShader(), nullptr, 0);
			}
			else{
				//�s�N�Z���V�F�[�_�̐ݒ�
				pID3D11DeviceContext->PSSetShader(PSSimplePNT::GetPtr()->GetShader(), nullptr, 0);
			}

			//���_�V�F�[�_�̐ݒ�
			pID3D11DeviceContext->VSSetShader(VSSimplePNT::GetPtr()->GetShader(), nullptr, 0);

			//�C���v�b�g���C�A�E�g�̐ݒ�
			pID3D11DeviceContext->IASetInputLayout(VSSimplePNT::GetPtr()->GetInputLayout());

			if (IsOwnShadowActive()){
				//�V���h�E�}�b�v�̃��\�[�X�r���[���擾
				ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();
				pID3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
				//�V���h�E�p�Ƀ��j�A�T���v���[���擾
				ID3D11SamplerState* pShadowSamplerState = RenderStatePtr->GetComparisonLinear();
				if (pImpl->m_Virsion <= 1){
					//�o�[�W�������Ⴂ�ꍇ�̓e�N�X�`���Ɠ����T���v���[���g�p
					pShadowSamplerState = RenderStatePtr->GetLinearClamp();
				}
				//�V���h�E�p���j�A�T���v���[��ݒ�
				pID3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSamplerState);
			}
			else{
				pID3D11DeviceContext->PSSetShaderResources(1, 1, pNull);
				pID3D11DeviceContext->PSSetSamplers(1, 1, pNullSR);
			}

			if (pImpl->m_ZBufferUse){
				//�f�v�X�X�e���V���͎g�p����
				pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
			}
			else{
				//�f�v�X�X�e���V���͎g�p���Ȃ�
				pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);

			}


			//�R���X�^���g�o�b�t�@�̐ݒ�
			ID3D11Buffer* pConstantBuffer = CBSimple::GetPtr()->GetBuffer();
			pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
			pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
			if (PtrGameObject->IsAlphaActive()){
				if (IsAlphaBlendSrcOne()){
					//��������
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);
				}
				else{
					//����������
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
				}
				//�����_�����O�X�e�[�g
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
				//�`��
				pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
				//�����_�����O�X�e�[�g
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				//�`��
				pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
			}
			else{
				//�h��Ԃ�
				pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
				//�����_�����O�X�e�[�g
				pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
				//�`��
				pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
			}
			//��n��
			Dev->InitializeStates(RenderStatePtr);


		}
	}



	//--------------------------------------------------------------------------------------
	//	struct BasicPNTDraw::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct BasicPNTDraw::Impl{
		Color4 m_Diffuse;	//�f�t�B�[�Y
		Color4 m_Emissive;	//�G�~�b�V�u
		Color4 m_SpecularAndPower;	//�X�y�L�����[�ƃp���[
		bool m_OwnShadowActive;		//���g�ւ̉e���e
		bool m_CullNone;		//���ʕ`�悩�ǂ���
		bool m_ZBufferUse;		//Z�o�b�t�@��L���ɂ��邩�ǂ���
		bool m_AlphaBlendSrcOne;	//����������SRC_ONE�ݒ�
		size_t m_Virsion;		//�V�F�[�_�[�̃o�[�W����
		weak_ptr<MeshResource> m_MeshResource;	//���b�V�����\�[�X
		weak_ptr<TextureResource> m_TextureResource;	//�e�N�X�`�����\�[�X
		bool m_TextureOnlyNoLight;				//�e�N�X�`���I�����[���ǂ���
		ID3D11SamplerState* m_pSamplerState;	//�T���v���[�X�e�[�g�i�I�v�V�����j
		Impl() :
			m_Diffuse(0.7f, 0.7f, 0.7f, 1.0f),
			m_Emissive(0.4f, 0.4f, 0.4f, 1.0f),
			m_SpecularAndPower(0.4f, 0.4f, 0.4f, 1.0f),
			m_OwnShadowActive(false),
			m_CullNone(false),
			m_ZBufferUse(true),
			m_AlphaBlendSrcOne(false),
			m_Virsion(1),
			m_TextureOnlyNoLight(false),
			m_pSamplerState(nullptr)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class BasicPNTDraw : public Component;
	//	�p�r: BasicPNTDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	BasicPNTDraw::BasicPNTDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	BasicPNTDraw::~BasicPNTDraw(){}

	//�A�N�Z�T
	const Color4& BasicPNTDraw::GetDiffuse() const { return pImpl->m_Diffuse; }
	void BasicPNTDraw::SetDiffuse(const Color4& c){ pImpl->m_Diffuse = c; }
	const Color4& BasicPNTDraw::GetEmissive() const { return pImpl->m_Emissive; }
	void BasicPNTDraw::SetEmissive(const Color4& c){ pImpl->m_Emissive = c; }
	const Color4& BasicPNTDraw::GetSpecularAndPower() const { return pImpl->m_SpecularAndPower; }
	void BasicPNTDraw::SetSpecularAndPower(const Color4& c){ pImpl->m_SpecularAndPower = c; }

	bool BasicPNTDraw::GetOwnShadowActive() const { return pImpl->m_OwnShadowActive; }
	bool BasicPNTDraw::IsOwnShadowActive() const { return pImpl->m_OwnShadowActive; }
	void BasicPNTDraw::SetOwnShadowActive(bool b){ pImpl->m_OwnShadowActive = b; }

	size_t BasicPNTDraw::GetShaderVirsion() const{
		return pImpl->m_Virsion;
	}
	void BasicPNTDraw::SetShaderVirsion(size_t v){
		if (v <= 1){
			v = 1;
		}
		else if (v >= 2){
			v = 2;
		}
		pImpl->m_Virsion = v;
	}


	bool BasicPNTDraw::GetCullNone() const{
		return pImpl->m_CullNone;
	}
	bool BasicPNTDraw::IsCullNone() const{
		return pImpl->m_CullNone;
	}
	void BasicPNTDraw::SetCullNone(bool b){
		pImpl->m_CullNone = b;
	}

	bool BasicPNTDraw::GetZBufferUse() const{
		return pImpl->m_ZBufferUse;

	}
	bool BasicPNTDraw::IsZBufferUse() const{
		return pImpl->m_ZBufferUse;
	}
	void BasicPNTDraw::SetZBufferUse(bool b){
		pImpl->m_ZBufferUse = b;
	}

	bool BasicPNTDraw::IsAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	bool BasicPNTDraw::GetAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	void BasicPNTDraw::SetAlphaBlendSrcOne(bool b){
		pImpl->m_AlphaBlendSrcOne = b;
	}



	void BasicPNTDraw::SetSamplerState(ID3D11SamplerState* pSamplerState){
		pImpl->m_pSamplerState = pSamplerState;
	}
	ID3D11SamplerState* BasicPNTDraw::GetSamplerState() const{
		if (IsGameObjectActive()){
			if (pImpl->m_pSamplerState){
				return pImpl->m_pSamplerState;
			}
			else{
				auto PtrGameObject = GetGameObject();
				auto PtrStage = PtrGameObject->GetStage();
				//�X�e�[�^�X�̃|�C���^
				auto RenderStatePtr = PtrStage->GetRenderState();
				return RenderStatePtr->GetLinearClamp();
			}
		}
		else{
			return nullptr;
		}
	}





	shared_ptr<MeshResource> BasicPNTDraw::GetMeshResource(bool ExceptionActive) const{
		if (!pImpl->m_MeshResource.expired()){
			return pImpl->m_MeshResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"���b�V�����\�[�X��������܂���",
					L"if (pImpl->m_MeshResource.expired())",
					L"BasicPNTDraw::GetMeshResource()"
					);
			}
		}
		return nullptr;
	}
	void BasicPNTDraw::SetMeshResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"���b�V���L�[���󔒂ł�",
					L"if (ResKey == L\"\"",
					L"BasicPNTDraw::SetMeshResource()"
					);
			}
			pImpl->m_MeshResource = App::GetApp()->GetResource<MeshResource>(ResKey);
		}
		catch (...){
			throw;
		}
	}
	void BasicPNTDraw::SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr){
		pImpl->m_MeshResource = MeshResourcePtr;
	}


	shared_ptr<TextureResource> BasicPNTDraw::GetTextureResource(bool ExceptionActive) const{
		if (!pImpl->m_TextureResource.expired()){
			return pImpl->m_TextureResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"�e�N�X�`�����\�[�X��������܂���",
					L"if (pImpl->m_Texture.expired())",
					L"BasicPNTDraw::GetTextureResource()"
					);
			}
		}
		return nullptr;
	}
	void BasicPNTDraw::SetTextureResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"�e�N�X�`���L�[���󔒂ł�",
					L"if (ResKey == L\"\"",
					L"BasicPNTDraw::SetTextureResource()"
					);
			}
			pImpl->m_TextureResource = App::GetApp()->GetResource<TextureResource>(ResKey);
		}
		catch (...){
			throw;
		}
	}
	void BasicPNTDraw::SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr){
		pImpl->m_TextureResource = TextureResourcePtr;
	}

	void BasicPNTDraw::SetTextureOnlyNoLight(bool b){
		pImpl->m_TextureOnlyNoLight = b;
	}
	bool BasicPNTDraw::GetTextureOnlyNoLight() const{
		return pImpl->m_TextureOnlyNoLight;
	}

	bool BasicPNTDraw::IsTextureOnlyNoLight() const{
		return pImpl->m_TextureOnlyNoLight;
	}



	void BasicPNTDraw::Draw(){

		//m_GameObject��null�Ȃ�Draw�ł��Ȃ�
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			auto PtrStage = PtrGameObject->GetStage();
			if (!PtrStage){
				return;
			}
			//�X�e�[�W����J���������o��
			auto PtrCamera = PtrStage->GetTargetCamera();
			//�}���`���C�g�����o��
			auto Lights = PtrStage->GetTargetMultiLight();
			//�X�e�[�W����0�Ԗڂ̃��C�g�����o��(�V���h�E�p)
			auto PtrLight = PtrStage->GetTargetLight(0);
			//���b�V�����\�[�X�̎擾
			auto PtrMeshResource = GetMeshResource();
			//�e�N�X�`���̎擾(null�̏ꍇ�L)
			auto PtrTextureResource = GetTextureResource(false);
			if (PtrT &&  PtrCamera && PtrMeshResource && PtrLight){

				auto Dev = App::GetApp()->GetDeviceResources();
				auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
				//�X�e�[�^�X�̃|�C���^
				auto RenderStatePtr = PtrStage->GetRenderState();
				//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
				auto ShadoumapPtr = PtrStage->GetShadowMapRenderTarget();

				//�J�����̎擾
				Matrix4X4 View, Proj, TransWorldViewProj;
				View = PtrCamera->GetViewMatrix();
				Proj = PtrCamera->GetProjMatrix();

				TransWorldViewProj = PtrT->GetWorldMatrix() * View * Proj;
				TransWorldViewProj.Transpose();

				//�R���X�^���g�o�b�t�@�̐ݒ�
				BasicConstantBuffer Cb;

				Cb.diffuseColor = pImpl->m_Diffuse;
				Cb.emissiveColor = pImpl->m_Emissive;
				Cb.specularColorAndPower = pImpl->m_SpecularAndPower;
				UINT lightcount = 0;
				for (size_t lc = 0; lc < Lights->GetLightCount(); lc++){
					auto LocalLight = PtrStage->GetTargetLight(lc);
					Cb.lightDirection[lc] = (XMVECTOR)LocalLight->GetDirectional();
					Cb.lightDiffuseColor[lc] = (XMVECTOR)LocalLight->GetDiffuseColor();
					Cb.lightSpecularColor[lc] = (XMVECTOR)LocalLight->GetSpecularColor();
					lightcount++;
					if (lc >= 2){
						break;
					}
				}
				Cb.m_ActiveFlg.z = lightcount;

				XMMATRIX viewInverse = XMMatrixInverse(nullptr, View);
				Cb.eyePosition = viewInverse.r[3];
				Cb.world = Matrix4X4EX::Transpose(PtrT->GetWorldMatrix());

				XMMATRIX worldInverse = XMMatrixInverse(nullptr, PtrT->GetWorldMatrix());
				Cb.worldInverseTranspose[0] = worldInverse.r[0];
				Cb.worldInverseTranspose[1] = worldInverse.r[1];
				Cb.worldInverseTranspose[2] = worldInverse.r[2];
				Cb.worldViewProj = TransWorldViewProj;


				if (IsOwnShadowActive()){
					//���C�g�̎擾
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
					Cb.LightWorldViewProj = Matrix4X4EX::Transpose(LWMatrix);
					Cb.m_ActiveFlg.y = 1;

					Cb.m_LPos = LightEye;
					Cb.m_LPos.w = 0;
					Cb.m_LView = Matrix4X4EX::Transpose(LightView);
					Cb.m_LProjection = Matrix4X4EX::Transpose(LightProj);
					Cb.m_EyePos = PtrCamera->GetEye();
					Cb.m_EyePos.w = 0;


				}
				if (PtrTextureResource){
					Cb.m_ActiveFlg.x = 1;
					if (IsTextureOnlyNoLight()){
						Cb.m_ActiveFlg.z = 0;
						Cb.m_ActiveFlg.w = 1;
					}
					else{
						Cb.m_ActiveFlg.w = 0;
					}
				}

				//�R���X�^���g�o�b�t�@�̍X�V
				pID3D11DeviceContext->UpdateSubresource(CBBasic::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);

				//�X�g���C�h�ƃI�t�Z�b�g
				UINT stride = sizeof(VertexPositionNormalTexture);
				UINT offset = 0;
				//���_�o�b�t�@�̐ݒ�
				pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
				//�C���f�b�N�X�o�b�t�@�̃Z�b�g
				pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);
				//�`����@�i3�p�`�j
				pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//���_�V�F�[�_�̐ݒ�
				pID3D11DeviceContext->VSSetShader(VSBasicPNT::GetPtr()->GetShader(), nullptr, 0);

				ID3D11ShaderResourceView* pNull[1] = { 0 };
				ID3D11SamplerState* pNullSR[1] = { 0 };
				if (PtrTextureResource){
					//�e�N�X�`����ݒ�
					pID3D11DeviceContext->PSSetShaderResources(0, 1, PtrTextureResource->GetShaderResourceView().GetAddressOf());
					//���j�A�T���v���[��ݒ�
					ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
					if (pImpl->m_pSamplerState){
						samplerState = pImpl->m_pSamplerState;
					}
					pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
				}
				else{
					pID3D11DeviceContext->PSSetShaderResources(0, 1, pNull);
					pID3D11DeviceContext->PSSetSamplers(0, 1, pNullSR);
				}

				if (GetShaderVirsion() >= 2){
					//�s�N�Z���V�F�[�_�̐ݒ�
					pID3D11DeviceContext->PSSetShader(PSBasicPNT2::GetPtr()->GetShader(), nullptr, 0);
				}
				else{
					//�s�N�Z���V�F�[�_�̐ݒ�
					pID3D11DeviceContext->PSSetShader(PSBasicPNT::GetPtr()->GetShader(), nullptr, 0);
				}

				//�C���v�b�g���C�A�E�g�̐ݒ�
				pID3D11DeviceContext->IASetInputLayout(VSBasicPNT::GetPtr()->GetInputLayout());

				if (IsOwnShadowActive()){
					//�V���h�E�}�b�v�̃��\�[�X�r���[���擾
					ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();
					pID3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
					//�V���h�E�p�Ƀ��j�A�T���v���[���擾
					ID3D11SamplerState* pShadowSamplerState = RenderStatePtr->GetComparisonLinear();
					if (pImpl->m_Virsion <= 1){
						//�o�[�W�������Ⴂ�ꍇ�̓e�N�X�`���Ɠ����T���v���[���g�p
						pShadowSamplerState = RenderStatePtr->GetLinearClamp();
					}
					//�V���h�E�p���j�A�T���v���[��ݒ�
					pID3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSamplerState);
				}
				else{
					pID3D11DeviceContext->PSSetShaderResources(1, 1, pNull);
					pID3D11DeviceContext->PSSetSamplers(1, 1, pNullSR);
				}

				if (pImpl->m_ZBufferUse){
					//�f�v�X�X�e���V���͎g�p����
					pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthDefault(), 0);
				}
				else{
					//�f�v�X�X�e���V���͎g�p���Ȃ�
					pID3D11DeviceContext->OMSetDepthStencilState(RenderStatePtr->GetDepthNone(), 0);

				}
				//�R���X�^���g�o�b�t�@�̐ݒ�
				ID3D11Buffer* pConstantBuffer = CBBasic::GetPtr()->GetBuffer();
				pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
				pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);
				if (PtrGameObject->IsAlphaActive()){
					if (IsAlphaBlendSrcOne()){
						//��������
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);
					}
					else{
						//����������
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
					}
					//�����_�����O�X�e�[�g
					pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
					//�`��
					pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
					//�����_�����O�X�e�[�g
					if (pImpl->m_CullNone){
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
					}
					else{
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
					}
					//�`��
					pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
				}
				else{
					//�h��Ԃ�
					pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
					//�����_�����O�X�e�[�g
					if (pImpl->m_CullNone){
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullNone());
					}
					else{
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
					}
					//�`��
					pID3D11DeviceContext->DrawIndexed(PtrMeshResource->GetNumIndicis(), 0, 0);
				}
				//��n��
				Dev->InitializeStates(RenderStatePtr);

			}
		}
	}




	//--------------------------------------------------------------------------------------
	//	struct BasicFbxPNTDraw::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct BasicFbxPNTDraw::Impl{
		Color4 m_Diffuse;	//�f�t�B�[�Y
		Color4 m_Emissive;	//�G�~�b�V�u
		Color4 m_SpecularAndPower;	//�X�y�L�����[�ƃp���[
		bool m_OwnShadowActive;		//���g�ւ̉e���e
		weak_ptr<FbxMeshResource> m_FbxMeshResource;	//FBX���b�V�����\�[�X
		Matrix4X4 m_MeshToTransform;
		bool m_TextureOnlyNoLight;				//�e�N�X�`���I�����[���ǂ���
		bool m_AlphaBlendSrcOne;	//����������SRC_ONE�ݒ�
		ID3D11SamplerState* m_pSamplerState;	//�T���v���[�X�e�[�g�i�I�v�V�����j
		Impl() :
			m_Diffuse(0.7f, 0.7f, 0.7f, 1.0f),
			m_Emissive(0.4f, 0.4f, 0.4f, 1.0f),
			m_SpecularAndPower(0.4f, 0.4f, 0.4f, 1.0f),
			m_OwnShadowActive(false),
			m_MeshToTransform(),
			m_TextureOnlyNoLight(false),
			m_AlphaBlendSrcOne(false),
			m_pSamplerState(nullptr)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class BasicFbxPNTDraw : public Component;
	//	�p�r: BasicFbxPNTDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	BasicFbxPNTDraw::BasicFbxPNTDraw(const shared_ptr<GameObject>& GameObjectPtr):
		DrawComponent(GameObjectPtr),
		pImpl(new Impl())
	{}
	BasicFbxPNTDraw::~BasicFbxPNTDraw(){}

	//�A�N�Z�T
	const Color4& BasicFbxPNTDraw::GetDiffuse() const { return pImpl->m_Diffuse; }
	void BasicFbxPNTDraw::SetDiffuse(const Color4& c){ pImpl->m_Diffuse = c; }
	const Color4& BasicFbxPNTDraw::GetEmissive() const { return pImpl->m_Emissive; }
	void BasicFbxPNTDraw::SetEmissive(const Color4& c){ pImpl->m_Emissive = c; }
	const Color4& BasicFbxPNTDraw::GetSpecularAndPower() const { return pImpl->m_SpecularAndPower; }
	void BasicFbxPNTDraw::SetSpecularAndPower(const Color4& c){ pImpl->m_SpecularAndPower = c; }

	bool BasicFbxPNTDraw::GetOwnShadowActive() const { return pImpl->m_OwnShadowActive; }
	bool BasicFbxPNTDraw::IsOwnShadowActive() const { return pImpl->m_OwnShadowActive; }
	void BasicFbxPNTDraw::SetOwnShadowActive(bool b){ pImpl->m_OwnShadowActive = b; }

	bool BasicFbxPNTDraw::IsAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	bool BasicFbxPNTDraw::GetAlphaBlendSrcOne()const{
		return pImpl->m_AlphaBlendSrcOne;
	}
	void BasicFbxPNTDraw::SetAlphaBlendSrcOne(bool b){
		pImpl->m_AlphaBlendSrcOne = b;
	}



	void BasicFbxPNTDraw::SetSamplerState(ID3D11SamplerState* pSamplerState){
		pImpl->m_pSamplerState = pSamplerState;
	}
	ID3D11SamplerState* BasicFbxPNTDraw::GetSamplerState() const{
		return pImpl->m_pSamplerState;
	}


	shared_ptr<FbxMeshResource> BasicFbxPNTDraw::GetFbxMeshResource(bool ExceptionActive) const{
		if (!pImpl->m_FbxMeshResource.expired()){
			return pImpl->m_FbxMeshResource.lock();
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"Fbx���b�V�����\�[�X��������܂���",
					L"if (pImpl->m_FbxMeshResource.expired())",
					L"BasicFbxPNTDraw::GetFbxMeshResource()"
					);
			}
		}
		return nullptr;
	}
	void BasicFbxPNTDraw::SetFbxMeshResource(const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"Fbx���b�V���L�[���󔒂ł�",
					L"if (ResKey == L\"\"",
					L"BasicFbxPNTDraw::SetMeshResource()"
					);
			}
			pImpl->m_FbxMeshResource = App::GetApp()->GetResource<FbxMeshResource>(ResKey);
			//�{�[��������Γǂݍ���
			SetBoneVec();
		}
		catch (...){
			throw;
		}
	}
	void BasicFbxPNTDraw::SetFbxMeshResource(const shared_ptr<FbxMeshResource>& FbxMeshResourcePtr){
		pImpl->m_FbxMeshResource = FbxMeshResourcePtr;
		//�{�[��������Γǂݍ���
		SetBoneVec();
	}

	const Matrix4X4& BasicFbxPNTDraw::GetMeshToTransform() const{
		return pImpl->m_MeshToTransform;
	}
	void BasicFbxPNTDraw::SetMeshToTransform(const Matrix4X4& Mat){
		pImpl->m_MeshToTransform = Mat;
	}

	void BasicFbxPNTDraw::SetTextureOnlyNoLight(bool b){
		pImpl->m_TextureOnlyNoLight = b;
	}
	bool BasicFbxPNTDraw::GetTextureOnlyNoLight() const{
		return pImpl->m_TextureOnlyNoLight;
	}

	bool BasicFbxPNTDraw::IsTextureOnlyNoLight() const{
		return pImpl->m_TextureOnlyNoLight;
	}



	//����
	void BasicFbxPNTDraw::Draw(){

		//m_GameObject��null�Ȃ�Draw�ł��Ȃ�
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			auto PtrStage = PtrGameObject->GetStage();
			if (!PtrStage){
				return;
			}
			//�X�e�[�W����J���������o��
			auto PtrCamera = PtrStage->GetTargetCamera();
			//�}���`���C�g�����o��
			auto Lights = PtrStage->GetTargetMultiLight();
			//�X�e�[�W����0�Ԗڂ̃��C�g�����o��
			auto PtrLight = PtrStage->GetTargetLight(0);
			//���b�V�����\�[�X�̎擾
			auto PtrMeshResource = GetFbxMeshResource();
			if (PtrT &&  PtrCamera && PtrMeshResource && PtrLight){

				Matrix4X4 RealWorldMatrix = GetMeshToTransform() * PtrT->GetWorldMatrix();

				auto Dev = App::GetApp()->GetDeviceResources();
				auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
				//�X�e�[�^�X�̃|�C���^
				auto RenderStatePtr = PtrStage->GetRenderState();
				//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
				auto ShadoumapPtr = PtrStage->GetShadowMapRenderTarget();

				//�J�����̎擾
				Matrix4X4 View, Proj, TransWorldViewProj;
				View = PtrCamera->GetViewMatrix();
				Proj = PtrCamera->GetProjMatrix();

				TransWorldViewProj = RealWorldMatrix * View * Proj;
				TransWorldViewProj.Transpose();

				//�R���X�^���g�o�b�t�@�̐ݒ�
				BasicConstantBuffer Cb;

				Cb.diffuseColor = pImpl->m_Diffuse;
				Cb.emissiveColor = pImpl->m_Emissive;
				Cb.specularColorAndPower = pImpl->m_SpecularAndPower;


				UINT lightcount = 0;
				for (size_t lc = 0; lc < Lights->GetLightCount(); lc++){
					auto LocalLight = PtrStage->GetTargetLight(lc);
					Cb.lightDirection[lc] = (XMVECTOR)LocalLight->GetDirectional();
					Cb.lightDiffuseColor[lc] = (XMVECTOR)LocalLight->GetDiffuseColor();
					Cb.lightSpecularColor[lc] = (XMVECTOR)LocalLight->GetSpecularColor();
					lightcount++;
					if (lc >= 2){
						break;
					}
				}
				Cb.m_ActiveFlg.z = lightcount;

				XMMATRIX viewInverse = XMMatrixInverse(nullptr, View);
				Cb.eyePosition = viewInverse.r[3];
				Cb.world = Matrix4X4EX::Transpose(RealWorldMatrix);

				XMMATRIX worldInverse = XMMatrixInverse(nullptr, RealWorldMatrix);
				Cb.worldInverseTranspose[0] = worldInverse.r[0];
				Cb.worldInverseTranspose[1] = worldInverse.r[1];
				Cb.worldInverseTranspose[2] = worldInverse.r[2];
				Cb.worldViewProj = TransWorldViewProj;


				if (IsOwnShadowActive()){
					//���C�g�̎擾
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
					Matrix4X4 LWMatrix = RealWorldMatrix * LightViewProj;
					Cb.LightWorldViewProj = Matrix4X4EX::Transpose(LWMatrix);
					Cb.m_ActiveFlg.y = 1;
				}
				Cb.m_ActiveFlg.x = 1;
				if (IsTextureOnlyNoLight()){
					Cb.m_ActiveFlg.z = 0;
					Cb.m_ActiveFlg.w = 1;
				}
				else{
					Cb.m_ActiveFlg.w = 0;
				}


				//�X�g���C�h�ƃI�t�Z�b�g
				UINT stride = sizeof(VertexPositionNormalTexture);
				//�{�[�����o�^����Ă�����{�[����T��
				if (PtrMeshResource->IsSkining()){
					stride = sizeof(VertexPositionNormalTextureSkinning);
				}


				UINT offset = 0;
				//�`����@�i3�p�`�j
				pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


				ID3D11ShaderResourceView* pNull[1] = { 0 };
				ID3D11SamplerState* pNullSR[1] = { 0 };


				if (IsOwnShadowActive()){
					//�V���h�E�}�b�v�̃��\�[�X�r���[���擾
					ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();
					pID3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
					//�V���h�E�p�Ƀ��j�A�T���v���[���擾(�e�N�X�`���Ɠ�������)
					ID3D11SamplerState* pShadowSamplerState = RenderStatePtr->GetLinearClamp();
					if (pImpl->m_pSamplerState){
						pShadowSamplerState = pImpl->m_pSamplerState;
					}
					//�V���h�E�p���j�A�T���v���[��ݒ�
					pID3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSamplerState);
				}

				////////////////////////////


				for (auto Mat : PtrMeshResource->GetMaterialVec()){

					Cb.diffuseColor = Mat.m_Diffuse;
					Cb.emissiveColor = Mat.m_Emissive;
					Cb.specularColorAndPower = Mat.m_Specular;

					//���_�o�b�t�@�̐ݒ�

					auto VBuffer = PtrMeshResource->GetVertexBuffer();
					auto IndBff = PtrMeshResource->GetIndexBuffer();

					pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
					//�C���f�b�N�X�o�b�t�@�̃Z�b�g
					pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);


					//�e�N�X�`����ݒ�
					pID3D11DeviceContext->PSSetShaderResources(0, 1, Mat.m_Texture->GetShaderResourceView().GetAddressOf());
					//���j�A�T���v���[��ݒ�
					ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
					if (pImpl->m_pSamplerState){
						samplerState = pImpl->m_pSamplerState;
					}
					pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
					//���_�V�F�[�_�̐ݒ�
					pID3D11DeviceContext->VSSetShader(VSBasicPNT::GetPtr()->GetShader(), nullptr, 0);
					//�C���v�b�g���C�A�E�g�̐ݒ�
					pID3D11DeviceContext->IASetInputLayout(VSBasicPNT::GetPtr()->GetInputLayout());
					//�s�N�Z���V�F�[�_�̐ݒ�
					pID3D11DeviceContext->PSSetShader(PSBasicPNT::GetPtr()->GetShader(), nullptr, 0);

					//�R���X�^���g�o�b�t�@�̍X�V
					pID3D11DeviceContext->UpdateSubresource(CBBasic::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
					//�R���X�^���g�o�b�t�@�̐ݒ�
					ID3D11Buffer* pConstantBuffer = CBBasic::GetPtr()->GetBuffer();
					pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
					pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);


					if (PtrGameObject->IsAlphaActive()){
						if (IsAlphaBlendSrcOne()){
							//��������
							pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);
						}
						else{
							//����������
							pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
						}
						//�����_�����O�X�e�[�g
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
						//�`��
						pID3D11DeviceContext->DrawIndexed(Mat.m_IndexCount, Mat.m_StartIndex, 0);
						//�����_�����O�X�e�[�g
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
						//�`��
						pID3D11DeviceContext->DrawIndexed(Mat.m_IndexCount, Mat.m_StartIndex, 0);
					}
					else{
						//�h��Ԃ�
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
						//�����_�����O�X�e�[�g
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
						//�`��
						pID3D11DeviceContext->DrawIndexed(Mat.m_IndexCount, Mat.m_StartIndex, 0);
					}

				}


				////////////////////////////
				//��n��
				Dev->InitializeStates(RenderStatePtr);
			}
		}

	}


	//--------------------------------------------------------------------------------------
	//	struct BasicFbxPNTBoneDraw::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct BasicFbxPNTBoneDraw::Impl{
		string m_sCurrentAnimationName;	//���ݏ����ΏۂɂȂ��Ă���A�j���[�V�����̖��O
		float m_CurrentTime;			//�A�j���[�V�����̌��݂̌o�ߎ��ԁi�b�j
		bool m_IsAnimeEnd;				//���݂̃A�j���[�V�������I���������ǂ���
		vector< Bone > m_vecLocalBones;	//�e�I�u�W�F�N�g���ƂɃ{�[�����������Ă���
		Impl():
			m_sCurrentAnimationName(""),
			m_CurrentTime(0),
			m_IsAnimeEnd(false)
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class BasicFbxPNTBoneDraw : public BasicFbxPNTDraw;
	//	�p�r: BasicFbxPNTBoneDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	BasicFbxPNTBoneDraw::BasicFbxPNTBoneDraw(const shared_ptr<GameObject>& GameObjectPtr) :
		BasicFbxPNTDraw(GameObjectPtr),
		pImpl(new Impl())
	{}
	BasicFbxPNTBoneDraw::~BasicFbxPNTBoneDraw(){}

	//�{�[���s������\�[�X����ǂݍ���
	void BasicFbxPNTBoneDraw::SetBoneVec(){
		auto PtrMeshResource = GetFbxMeshResource();
		if (PtrMeshResource->GetNumBones() > 0){
			//���[�J���{�[���̃I���W�i������̃R�s�[
			pImpl->m_vecLocalBones.resize(PtrMeshResource->GetBonesVec().size());
			pImpl->m_vecLocalBones = PtrMeshResource->GetBonesVec();
		}
		else{
			throw BaseException(
				L"�{�[����������܂���",
				L"if (PtrMeshResource->GetNumBones() <= 0)",
				L"BasicFbxPNTBoneDraw::SetBoneVec()"
				);
		}
	}


	//�A�N�Z�T
	//�e�I�u�W�F�N�g���Ƃ̃{�[��
	const vector< Bone >& BasicFbxPNTBoneDraw::GetVecLocalBones() const{
		return pImpl->m_vecLocalBones;
	}
	const vector< Bone >* BasicFbxPNTBoneDraw::GetVecLocalBonesPtr() const{
		return &pImpl->m_vecLocalBones;
	}


	const string& BasicFbxPNTBoneDraw::GetCurrentAnimation() const{
		return pImpl->m_sCurrentAnimationName;
	}
	void BasicFbxPNTBoneDraw::SetCurrentAnimation(const string& AnemationName, float StartTime){
		auto PtrMeshResource = GetFbxMeshResource();
		//�w��̃A�j���[�V���������邩�ǂ����`�F�b�N
		//������Η�O���o��
		AnimationData animData = PtrMeshResource->GetAnimationData(AnemationName);
		pImpl->m_sCurrentAnimationName = AnemationName;
		pImpl->m_CurrentTime = StartTime;
	}

	//���݂̃A�j���[�V�������I�����Ă��邩�ǂ���
	bool BasicFbxPNTBoneDraw::IsTargetAnimeEnd() const{
		return pImpl->m_IsAnimeEnd;
	}

	//�w�肵��ID�̃{�[���̌��݂̍s����擾����
	void BasicFbxPNTBoneDraw::GetBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const{
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			Matrix4X4 MeshMatrix = GetMeshToTransform() * PtrT->GetWorldMatrix();
			if (pImpl->m_vecLocalBones.size() <= BoneId){
				throw BaseException(
					L"�{�[��ID���͈͊O�ł�",
					L"if (pImpl->m_vecLocalBones.size() <= BoneId)",
					L"BasicFbxPNTBoneDraw::GetBoneMatrix()"
					);
			}
			Matrix = pImpl->m_vecLocalBones[BoneId].m_CurrentPose * MeshMatrix;
		}
	}

	//�w�肵��ID�̃{�[���̌��݂̃��[�J���s����擾����i�e�q�֌W���\�z����ȂǗp�j
	void BasicFbxPNTBoneDraw::GetLocalBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const{
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			Matrix4X4 MeshMatrix = GetMeshToTransform();// *PtrT->GetWorldMatrix();
			if (pImpl->m_vecLocalBones.size() <= BoneId){
				throw BaseException(
					L"�{�[��ID���͈͊O�ł�",
					L"if (pImpl->m_vecLocalBones.size() <= BoneId)",
					L"BasicFbxPNTBoneDraw::GetLocalBoneMatrix()"
					);
			}
			Matrix = pImpl->m_vecLocalBones[BoneId].m_CurrentPose * MeshMatrix;
		}
	}



	void BasicFbxPNTBoneDraw::Update(){
		//�A�j���[�V�������X�V����
		//���b�V�����\�[�X�̎擾
		auto PtrMeshResource = GetFbxMeshResource();
		//�{�[�����o�^����Ă�����{�[����T��
		if (PtrMeshResource->IsSkining()){
			pImpl->m_CurrentTime += App::GetApp()->GetElapsedTime();
			//�A�j���[�V�����f�[�^�̎擾
			//�`�F�b�N���邾���Ȃ̂Ńe���|�����ł悢
			AnimationData animData = PtrMeshResource->GetAnimationData(pImpl->m_sCurrentAnimationName);
			//�{�[�������݂̎��ԂɍX�V����
			//�߂�l�͏I�����Ă邩�ǂ���
			pImpl->m_IsAnimeEnd = PtrMeshResource->GenerateCurrentPose(pImpl->m_vecLocalBones, animData, pImpl->m_CurrentTime);
		}
	}


	void  BasicFbxPNTBoneDraw::Draw(){

		//m_GameObject��null�Ȃ�Draw�ł��Ȃ�
		if (IsGameObjectActive()){
			auto PtrGameObject = GetGameObject();
			auto PtrT = PtrGameObject->GetComponent<Transform>();
			auto PtrStage = PtrGameObject->GetStage();
			if (!PtrStage){
				return;
			}
			//�X�e�[�W����J���������o��
			auto PtrCamera = PtrStage->GetTargetCamera();
			//�}���`���C�g�����o��
			auto Lights = PtrStage->GetTargetMultiLight();
			//�X�e�[�W����0�Ԗڂ̃��C�g�����o��
			auto PtrLight = PtrStage->GetTargetLight(0);
			//���b�V�����\�[�X�̎擾
			auto PtrMeshResource = GetFbxMeshResource();
			if (PtrT &&  PtrCamera && PtrMeshResource && PtrLight){

				Matrix4X4 RealWorldMatrix = GetMeshToTransform() * PtrT->GetWorldMatrix();


				auto Dev = App::GetApp()->GetDeviceResources();
				auto pID3D11DeviceContext = Dev->GetD3DDeviceContext();
				//�X�e�[�^�X�̃|�C���^
				auto RenderStatePtr = PtrStage->GetRenderState();
				//�V���h�E�}�b�v�̃����_���[�^�[�Q�b�g
				auto ShadoumapPtr = PtrStage->GetShadowMapRenderTarget();

				ID3D11ShaderResourceView* pNull[1] = { 0 };
				ID3D11SamplerState* pNullSR[1] = { 0 };

				//�R���X�^���g�o�b�t�@�̐ݒ�
				BasicBoneShadowConstantBuffer CbShadow;

				if (IsOwnShadowActive()){
					//�V���h�E�}�b�v�̃��\�[�X�r���[���擾
					ID3D11ShaderResourceView* pShadowSRV = ShadoumapPtr->GetShaderResourceView();
					pID3D11DeviceContext->PSSetShaderResources(1, 1, &pShadowSRV);
					//�V���h�E�p�Ƀ��j�A�T���v���[���擾(�e�N�X�`���Ɠ�������)
					ID3D11SamplerState* pShadowSamplerState = RenderStatePtr->GetLinearClamp();
					if (GetSamplerState()){
						pShadowSamplerState = GetSamplerState();
					}
					//�V���h�E�p���j�A�T���v���[��ݒ�
					pID3D11DeviceContext->PSSetSamplers(1, 1, &pShadowSamplerState);
					CbShadow.m_ActiveFlg.y = 1;
				}
				else{
					pID3D11DeviceContext->PSSetShaderResources(1, 1, pNull);
					pID3D11DeviceContext->PSSetSamplers(1, 1, pNullSR);
				}


				//�J�����̎擾
				Matrix4X4 View, Proj, TransWorldViewProj;
				View = PtrCamera->GetViewMatrix();
				Proj = PtrCamera->GetProjMatrix();

				TransWorldViewProj = RealWorldMatrix * View * Proj;
				TransWorldViewProj.Transpose();

				//�R���X�^���g�o�b�t�@�̐ݒ�
				BasicBoneConstantBuffer Cb;

				Cb.diffuseColor = GetDiffuse();
				Cb.emissiveColor = GetEmissive();
				Cb.specularColorAndPower = GetSpecularAndPower();

				UINT lightcount = 0;
				for (size_t lc = 0; lc < Lights->GetLightCount(); lc++){
					auto LocalLight = PtrStage->GetTargetLight(lc);
					Cb.lightDirection[lc] = (XMVECTOR)LocalLight->GetDirectional();
					Cb.lightDiffuseColor[lc] = (XMVECTOR)LocalLight->GetDiffuseColor();
					Cb.lightSpecularColor[lc] = (XMVECTOR)LocalLight->GetSpecularColor();
					lightcount++;
					if (lc >= 2){
						break;
					}
				}
				CbShadow.m_ActiveFlg.z = lightcount;

				if (IsTextureOnlyNoLight()){
					CbShadow.m_ActiveFlg.z = 0;
					CbShadow.m_ActiveFlg.w = 1;
				}
				else{
					CbShadow.m_ActiveFlg.w = 0;
				}


				XMMATRIX viewInverse = XMMatrixInverse(nullptr, View);
				Cb.eyePosition = viewInverse.r[3];
				Cb.world = Matrix4X4EX::Transpose(RealWorldMatrix);

				XMMATRIX worldInverse = XMMatrixInverse(nullptr, RealWorldMatrix);
				Cb.worldInverseTranspose[0] = worldInverse.r[0];
				Cb.worldInverseTranspose[1] = worldInverse.r[1];
				Cb.worldInverseTranspose[2] = worldInverse.r[2];
				Cb.worldViewProj = TransWorldViewProj;

				//�{�[���̐ݒ�
				size_t BoneSz = pImpl->m_vecLocalBones.size();
				UINT cb_count = 0;
				for (size_t b = 0; b < BoneSz; b++){
					Matrix4X4 mat = pImpl->m_vecLocalBones[b].m_ConbinedPose;
					mat.Transpose();
					Cb.Bones[cb_count] = ((XMMATRIX)mat).r[0];
					Cb.Bones[cb_count + 1] = ((XMMATRIX)mat).r[1];
					Cb.Bones[cb_count + 2] = ((XMMATRIX)mat).r[2];
					cb_count += 3;
				}


				//�X�g���C�h�ƃI�t�Z�b�g
				UINT stride = sizeof(VertexPositionNormalTextureSkinning);
				UINT offset = 0;
				//�`����@�i3�p�`�j
				pID3D11DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				//ID3D11ShaderResourceView* pNull[1] = { 0 };
				//ID3D11SamplerState* pNullSR[1] = { 0 };

				////////////////////////////

				for (auto Mat : PtrMeshResource->GetMaterialVec()){


					Cb.diffuseColor = Mat.m_Diffuse;
					Cb.emissiveColor = Mat.m_Emissive;
					Cb.specularColorAndPower = Mat.m_Specular;

					//���_�o�b�t�@�̐ݒ�

					auto VBuffer = PtrMeshResource->GetVertexBuffer();
					auto IndBff = PtrMeshResource->GetIndexBuffer();

					pID3D11DeviceContext->IASetVertexBuffers(0, 1, PtrMeshResource->GetVertexBuffer().GetAddressOf(), &stride, &offset);
					//�C���f�b�N�X�o�b�t�@�̃Z�b�g
					pID3D11DeviceContext->IASetIndexBuffer(PtrMeshResource->GetIndexBuffer().Get(), DXGI_FORMAT_R16_UINT, 0);


					//�e�N�X�`����ݒ�
					pID3D11DeviceContext->PSSetShaderResources(0, 1, Mat.m_Texture->GetShaderResourceView().GetAddressOf());
					//���j�A�T���v���[��ݒ�
					ID3D11SamplerState* samplerState = RenderStatePtr->GetLinearClamp();
					if (GetSamplerState()){
						samplerState = GetSamplerState();
					}
					pID3D11DeviceContext->PSSetSamplers(0, 1, &samplerState);
					//���_�V�F�[�_�̐ݒ�
					pID3D11DeviceContext->VSSetShader(VSBasicPNTBone::GetPtr()->GetShader(), nullptr, 0);
					//�C���v�b�g���C�A�E�g�̐ݒ�
					pID3D11DeviceContext->IASetInputLayout(VSBasicPNTBone::GetPtr()->GetInputLayout());
					//�s�N�Z���V�F�[�_�̐ݒ�
					pID3D11DeviceContext->PSSetShader(PSBasicPNTBone::GetPtr()->GetShader(), nullptr, 0);

					//�R���X�^���g�o�b�t�@�̍X�V
					pID3D11DeviceContext->UpdateSubresource(CBBasicBone::GetPtr()->GetBuffer(), 0, nullptr, &Cb, 0, 0);
					pID3D11DeviceContext->UpdateSubresource(CBBasicBoneShadow::GetPtr()->GetBuffer(), 0, nullptr, &CbShadow, 0, 0);
					//�R���X�^���g�o�b�t�@�̐ݒ�
					ID3D11Buffer* pConstantBuffer = CBBasicBone::GetPtr()->GetBuffer();
					pID3D11DeviceContext->VSSetConstantBuffers(0, 1, &pConstantBuffer);
					pID3D11DeviceContext->PSSetConstantBuffers(0, 1, &pConstantBuffer);

					pConstantBuffer = CBBasicBoneShadow::GetPtr()->GetBuffer();
					pID3D11DeviceContext->VSSetConstantBuffers(1, 1, &pConstantBuffer);
					pID3D11DeviceContext->PSSetConstantBuffers(1, 1, &pConstantBuffer);



					if (PtrGameObject->IsAlphaActive()){
						if (IsAlphaBlendSrcOne()){
							//��������
							pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlend(), nullptr, 0xffffffff);
						}
						else{
							//����������
							pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetAlphaBlendEx(), nullptr, 0xffffffff);
						}
						//�����_�����O�X�e�[�g
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullFront());
						//�`��
						pID3D11DeviceContext->DrawIndexed(Mat.m_IndexCount, Mat.m_StartIndex, 0);
						//�����_�����O�X�e�[�g
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
						//�`��
						pID3D11DeviceContext->DrawIndexed(Mat.m_IndexCount, Mat.m_StartIndex, 0);
					}
					else{
						//�h��Ԃ�
						pID3D11DeviceContext->OMSetBlendState(RenderStatePtr->GetOpaque(), nullptr, 0xffffffff);
						//�����_�����O�X�e�[�g
						pID3D11DeviceContext->RSSetState(RenderStatePtr->GetCullBack());
						//�`��
						pID3D11DeviceContext->DrawIndexed(Mat.m_IndexCount, Mat.m_StartIndex, 0);
					}

				}


				////////////////////////////
				//��n��
				Dev->InitializeStates(RenderStatePtr);

			}
		}
	}




}

//end basedx11

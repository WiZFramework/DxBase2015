
/*!
@file DrawComponents.h
@brief �R���|�[�l���g�i�`��n�j

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{

	class TextureResource;
	class MeshResource;
	class FbxMeshResource;

	struct	Bone;


	//--------------------------------------------------------------------------------------
	//	class Shadowmap : public Component;
	//	�p�r: �V���h�E�}�b�v�R���|�[�l���g�i�O�����p�j
	//--------------------------------------------------------------------------------------
	class Shadowmap : public Component{
	public:
		explicit Shadowmap(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~Shadowmap();
		//�A�N�Z�T
		static float GetLightHeight();
		static float GetLightNear();
		static float GetLightFar();
		static float GetViewWidth();
		static float GetViewHeight();

		static void SetLightHeight(float f);
		static void SetLightNear(float f);
		static void SetLightFar(float f);
		static void SetViewWidth(float f);
		static void SetViewHeight(float f);

		shared_ptr<MeshResource> GetMeshResource(bool ExceptionActive = true) const;
		void SetMeshResource(const wstring& ResKey);
		void SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr);

		const Matrix4X4& GetMeshToTransform() const;
		void SetMeshToTransform(const Matrix4X4& Mat);

		//����
		virtual void Update()override{}
		virtual void Draw();
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class DrawComponent : public Component;
	//	�p�r: �`��R���|�[�l���g�̐e�N���X
	//--------------------------------------------------------------------------------------
	class DrawComponent : public Component{
	protected:
		DrawComponent(const shared_ptr<GameObject>& GameObjectPtr) :
			Component(GameObjectPtr){}
		virtual ~DrawComponent(){}
	public:
		//�e�I�u�W�F�N�g���ƂɃ{�[�����������Ă����|�C���^
		//�V���h�E�}�b�v����e�N���X�ŎQ�Ƃł���悤�ɉ��z�֐��ɂ���
		virtual const vector< Bone >* GetVecLocalBonesPtr() const{ return nullptr; }
	};

	//--------------------------------------------------------------------------------------
	//	class SimpleDirectDraw : public DrawComponent;
	//	�p�r: SimpleDirectDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class SimpleDirectDraw : public DrawComponent{
	public:
		explicit SimpleDirectDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~SimpleDirectDraw();
		//�A�N�Z�T
		const Color4& GetDiffuse() const;
		void SetDiffuse(const Color4& c);
		D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const;
		void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY t);
		//���_�z���n���Ē��_�o�b�t�@���쐬����
		void CreateVertex(const vector<VertexPosition>& Verteces);
		//����
		virtual void Update()override{}
		virtual void Draw();
	private:
		// pinpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class SimplePNTDraw : public DrawComponent;
	//	�p�r: SimplePNTDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class SimplePNTDraw : public DrawComponent{
	public:
		explicit SimplePNTDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~SimplePNTDraw();
		//�A�N�Z�T
		const Color4& GetDiffuse() const;
		void SetDiffuse(const Color4& c);

		bool GetOwnShadowActive() const;
		bool IsOwnShadowActive() const;
		void SetOwnShadowActive(bool b);

		bool GetZBufferUse() const;
		bool IsZBufferUse() const;
		void SetZBufferUse(bool b);

		bool IsAlphaBlendSrcOne()const;
		bool GetAlphaBlendSrcOne()const;
		void SetAlphaBlendSrcOne(bool b);

		size_t GetShaderVirsion() const;
		void SetShaderVirsion(size_t v);


		void SetSamplerState(ID3D11SamplerState* pSamplerState);


		shared_ptr<MeshResource> GetMeshResource(bool ExceptionActive = true) const;
		void SetMeshResource(const wstring& ResKey);
		void SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr);

		shared_ptr<TextureResource> GetTextureResource(bool ExceptionActive = true) const;
		void SetTextureResource(const wstring& ResKey);
		void SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr);

		//����
		virtual void Update()override{}
		virtual void Draw();
	private:
		// pinpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class BasicPNTDraw : public DrawComponent;
	//	�p�r: BasicPNTDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class BasicPNTDraw : public DrawComponent{
		void DrawWithShadow();
		void DrawNotShadow();
	public:
		//�\�z�Ɣj��
		explicit BasicPNTDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~BasicPNTDraw();
		//�A�N�Z�T
		const Color4& GetDiffuse() const;
		void SetDiffuse(const Color4& c);
		const Color4& GetEmissive() const;
		void SetEmissive(const Color4& c);
		const Color4& GetSpecularAndPower() const;
		void SetSpecularAndPower(const Color4& c);

		bool GetOwnShadowActive() const;
		bool IsOwnShadowActive() const;
		void SetOwnShadowActive(bool b);

		bool GetCullNone() const;
		bool IsCullNone() const;
		void SetCullNone(bool b);

		bool GetZBufferUse() const;
		bool IsZBufferUse() const;
		void SetZBufferUse(bool b);

		bool IsAlphaBlendSrcOne()const;
		bool GetAlphaBlendSrcOne()const;
		void SetAlphaBlendSrcOne(bool b);



		size_t GetShaderVirsion() const;
		void SetShaderVirsion(size_t v);


		void SetSamplerState(ID3D11SamplerState* pSamplerState);
		ID3D11SamplerState* GetSamplerState() const;

		shared_ptr<MeshResource> GetMeshResource(bool ExceptionActive = true) const;
		void SetMeshResource(const wstring& ResKey);
		void SetMeshResource(const shared_ptr<MeshResource>& MeshResourcePtr);

		shared_ptr<TextureResource> GetTextureResource(bool ExceptionActive = true) const;
		void SetTextureResource(const wstring& ResKey);
		void SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr);

		void SetTextureOnlyNoLight(bool b);
		bool GetTextureOnlyNoLight() const;
		bool IsTextureOnlyNoLight() const;


		//����
		virtual void Update()override{}
		virtual void Draw();
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




	//--------------------------------------------------------------------------------------
	//	class BasicFbxPNTDraw : public DrawComponent;
	//	�p�r: BasicFbxPNTDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class BasicFbxPNTDraw : public DrawComponent{
		void DrawWithShadow();
		void DrawNotShadow();
	protected:
		//�{�[���s������\�[�X����ǂݍ���
		//�e�N���X�ł͉������Ȃ�
		virtual void SetBoneVec(){}
	public:
		//�\�z�Ɣj��
		explicit BasicFbxPNTDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~BasicFbxPNTDraw();
		//�A�N�Z�T
		const Color4& GetDiffuse() const;
		void SetDiffuse(const Color4& c);
		const Color4& GetEmissive() const;
		void SetEmissive(const Color4& c);
		const Color4& GetSpecularAndPower() const;
		void SetSpecularAndPower(const Color4& c);

		bool GetOwnShadowActive() const;
		bool IsOwnShadowActive() const;
		void SetOwnShadowActive(bool b);

		bool IsAlphaBlendSrcOne()const;
		bool GetAlphaBlendSrcOne()const;
		void SetAlphaBlendSrcOne(bool b);


		void SetSamplerState(ID3D11SamplerState* pSamplerState);
		ID3D11SamplerState* GetSamplerState() const;


		shared_ptr<FbxMeshResource> GetFbxMeshResource(bool ExceptionActive = true) const;
		void SetFbxMeshResource(const wstring& ResKey);
		void SetFbxMeshResource(const shared_ptr<FbxMeshResource>& FbxMeshResourcePtr);

		const Matrix4X4& GetMeshToTransform() const;
		void SetMeshToTransform(const Matrix4X4& Mat);

		void SetTextureOnlyNoLight(bool b);
		bool GetTextureOnlyNoLight() const;
		bool IsTextureOnlyNoLight() const;

		//����
		virtual void Update()override{}
		virtual void Draw();
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class BasicFbxPNTBoneDraw : public BasicFbxPNTDraw;
	//	�p�r: BasicFbxPNTBoneDraw�`��R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class BasicFbxPNTBoneDraw : public BasicFbxPNTDraw{
		void DrawWithShadow();
		void DrawNotShadow();
	protected:
		//�{�[���s������\�[�X����ǂݍ���
		virtual void SetBoneVec()override;
	public:
		//�\�z�Ɣj��
		explicit BasicFbxPNTBoneDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~BasicFbxPNTBoneDraw();
		//�A�N�Z�T
		//�e�I�u�W�F�N�g���ƂɃ{�[�����������Ă����|�C���^
		const vector< Bone >& GetVecLocalBones() const;
		virtual const vector< Bone >* GetVecLocalBonesPtr() const;

		//�J�����g�A�j���[�V����
		const string& GetCurrentAnimation() const;
		void SetCurrentAnimation(const string& AnemationName,float StartTime = 0.0f);
		//���݂̃A�j���[�V�������I�����Ă��邩�ǂ���
		bool IsTargetAnimeEnd() const;

		//�w�肵��ID�̃{�[���̌��݂̍s����擾����
		void GetBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const;

		//�w�肵��ID�̃{�[���̌��݂̃��[�J���s����擾����i�e�q�֌W���\�z����ȂǗp�j
		void GetLocalBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const;

		//����
		virtual void Update()override;
		virtual void Draw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};






}

//end basedx11

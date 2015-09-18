#pragma once

#include "stdafx.h"

namespace basedx11{

	struct FbxMeshObjData{
		wstring m_DataDir;
		wstring m_FbxFileName;
		size_t m_MeshIndex;
		float m_Scale;
	};

	struct FbxMeshObjSaveData{
		wstring m_DataDir;
		wstring m_BinFileName;
		wstring m_Header;
		float m_Scale;
		UINT m_StartFrame;
		UINT m_FrameCount;
		float m_FrameParSecond;
		float m_SampleSpan;

		FbxMeshObjSaveData() :
			m_Scale(1.0f),
			m_StartFrame(0),
			m_FrameCount(1),
			m_FrameParSecond(30.0f),
			m_SampleSpan(1.0f / 30.0f)
		{}
	};

	struct MaterialEx{
		//!�J�n�C���f�b�N�X
		UINT m_StartIndex;
		//!�`��C���f�b�N�X�J�E���g
		UINT m_IndexCount;
		//! �f�t�B�[�Y�i���̂̐F�j
		Color4 m_Diffuse;
		//! �X�y�L�����[�i���ˌ��j
		Color4 m_Specular;
		//! �A���r�G���g�i���F�j
		Color4 m_Ambient;
		//! �G�~�b�V�u�i���ˌ��j
		Color4 m_Emissive;
		//! �Q�Ɨp�e�N�X�`���i�폜���Ȃ��j
		shared_ptr<TextureResource> m_Texture;
	};

	struct SaveMaterialEx{
		//!�J�n�C���f�b�N�X
		UINT m_StartIndex;
		//!�`��C���f�b�N�X�J�E���g
		UINT m_IndexCount;
		//! �f�t�B�[�Y�i���̂̐F�j
		Color4 m_Diffuse;
		//! �X�y�L�����[�i���ˌ��j
		Color4 m_Specular;
		//! �A���r�G���g�i���F�j
		Color4 m_Ambient;
		//! �G�~�b�V�u�i���ˌ��j
		Color4 m_Emissive;
	};


	enum class BlockType{
		Vertex,
		Index,
		Material,
		MaterialCount,
		SkinedVertex,
		BoneCount,
		AnimeMatrix,
		End = 100
	};

	struct BlockHeader{
		BlockType m_Type;
		UINT m_Size;
	};



	//--------------------------------------------------------------------------------------
	//	class FbxMeshObject : public GameObject;
	//	�p�r: FBX���b�V���̃I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class FbxMeshObject : public GameObject{
		//�C�x���g�}�V�[��
		shared_ptr< EventMachine<FbxMeshObject> >  m_EventMachine;
		wstring m_DataDir;
		wstring m_FbxFileName;
		wstring m_FbxResName;
		size_t m_MeshIndex;
		wstring m_FbxMeshResName;


		void SaveStaticMeshBinFile(const shared_ptr<FbxMeshObjSaveData>& Data);
		void SaveSkinedMeshBinFile(const shared_ptr<FbxMeshObjSaveData>& Data);

	public:
		FbxMeshObject(const shared_ptr<Stage>& StagePtr);
		virtual ~FbxMeshObject(){}
		void ResetFbxMesh(const wstring& DirName, const wstring& FbxName,size_t MeshIndex,float Scale);

		void SaveBinFile(const shared_ptr<FbxMeshObjSaveData>& Data);
		void SaveSkinBinFile(const shared_ptr<FbxMeshObjSaveData>& Data);

		void RunSkin(const shared_ptr<FbxMeshObjSaveData>& Data);


		void ReadMesh(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices, vector<MaterialEx>& materials,
			float Scale);
		void ReadSkinMesh(vector<VertexPositionNormalTextureSkinning>& vertices, vector<uint16_t>& indices, 
			vector<MaterialEx>& materials, vector<Bone>& bones, vector<Matrix4X4>& animematrix,float Scale,
			const shared_ptr<FbxMeshObjSaveData>& Dat);


		//������
		virtual void Create() override;
		//�C�x���g�̃n���h��
		virtual void OnEvent(const shared_ptr<Event>& event)override;

	};

	//--------------------------------------------------------------------------------------
	//	class DataResetEvent : public EventState<FbxMeshObject>;
	//	�p�r: �f�[�^�������ւ���ꂽ�C�x���g
	//--------------------------------------------------------------------------------------
	class DataResetEvent : public EventState<FbxMeshObject>
	{
		DataResetEvent(){}
	public:
		//�C�x���g�X�e�[�g�̃C���X�^���X�𓾂�
		static shared_ptr<DataResetEvent> Instance();
		//���̃C�x���g�����������Ƃ��ɌĂ΂��
		virtual void Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event)override;
	};

	//--------------------------------------------------------------------------------------
	//	class DataSaveEvent : public EventState<FbxMeshObject>;
	//	�p�r: �f�[�^�ۑ��̃C�x���g
	//--------------------------------------------------------------------------------------
	class DataSaveEvent : public EventState<FbxMeshObject>
	{
		DataSaveEvent(){}
	public:
		//�C�x���g�X�e�[�g�̃C���X�^���X�𓾂�
		static shared_ptr<DataSaveEvent> Instance();
		//���̃C�x���g�����������Ƃ��ɌĂ΂��
		virtual void Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event)override;
	};


	//--------------------------------------------------------------------------------------
	//	class DataSaveSkinEvent : public EventState<FbxMeshObject>;
	//	�p�r: �X�L���f�[�^�ۑ��̃C�x���g
	//--------------------------------------------------------------------------------------
	class DataSaveSkinEvent : public EventState<FbxMeshObject>
	{
		DataSaveSkinEvent(){}
	public:
		//�C�x���g�X�e�[�g�̃C���X�^���X�𓾂�
		static shared_ptr<DataSaveSkinEvent> Instance();
		//���̃C�x���g�����������Ƃ��ɌĂ΂��
		virtual void Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event)override;
	};


	//--------------------------------------------------------------------------------------
	//	class RunSkinEvent : public EventState<FbxMeshObject>;
	//	�p�r: �X�L�����s�̃C�x���g
	//--------------------------------------------------------------------------------------
	class RunSkinEvent : public EventState<FbxMeshObject>
	{
		RunSkinEvent(){}
	public:
		//�C�x���g�X�e�[�g�̃C���X�^���X�𓾂�
		static shared_ptr<RunSkinEvent> Instance();
		//���̃C�x���g�����������Ƃ��ɌĂ΂��
		virtual void Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event)override;
	};





}
//endof  basedx11

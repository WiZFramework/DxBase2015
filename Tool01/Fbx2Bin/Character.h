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
		//!開始インデックス
		UINT m_StartIndex;
		//!描画インデックスカウント
		UINT m_IndexCount;
		//! デフィーズ（物体の色）
		Color4 m_Diffuse;
		//! スペキュラー（反射光）
		Color4 m_Specular;
		//! アンビエント（環境色）
		Color4 m_Ambient;
		//! エミッシブ（放射光）
		Color4 m_Emissive;
		//! 参照用テクスチャ（削除しない）
		shared_ptr<TextureResource> m_Texture;
	};

	struct SaveMaterialEx{
		//!開始インデックス
		UINT m_StartIndex;
		//!描画インデックスカウント
		UINT m_IndexCount;
		//! デフィーズ（物体の色）
		Color4 m_Diffuse;
		//! スペキュラー（反射光）
		Color4 m_Specular;
		//! アンビエント（環境色）
		Color4 m_Ambient;
		//! エミッシブ（放射光）
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
	//	用途: FBXメッシュのオブジェクト
	//--------------------------------------------------------------------------------------
	class FbxMeshObject : public GameObject{
		//イベントマシーン
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


		//初期化
		virtual void Create() override;
		//イベントのハンドラ
		virtual void OnEvent(const shared_ptr<Event>& event)override;

	};

	//--------------------------------------------------------------------------------------
	//	class DataResetEvent : public EventState<FbxMeshObject>;
	//	用途: データが差し替えられたイベント
	//--------------------------------------------------------------------------------------
	class DataResetEvent : public EventState<FbxMeshObject>
	{
		DataResetEvent(){}
	public:
		//イベントステートのインスタンスを得る
		static shared_ptr<DataResetEvent> Instance();
		//このイベントが発生したときに呼ばれる
		virtual void Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event)override;
	};

	//--------------------------------------------------------------------------------------
	//	class DataSaveEvent : public EventState<FbxMeshObject>;
	//	用途: データ保存のイベント
	//--------------------------------------------------------------------------------------
	class DataSaveEvent : public EventState<FbxMeshObject>
	{
		DataSaveEvent(){}
	public:
		//イベントステートのインスタンスを得る
		static shared_ptr<DataSaveEvent> Instance();
		//このイベントが発生したときに呼ばれる
		virtual void Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event)override;
	};


	//--------------------------------------------------------------------------------------
	//	class DataSaveSkinEvent : public EventState<FbxMeshObject>;
	//	用途: スキンデータ保存のイベント
	//--------------------------------------------------------------------------------------
	class DataSaveSkinEvent : public EventState<FbxMeshObject>
	{
		DataSaveSkinEvent(){}
	public:
		//イベントステートのインスタンスを得る
		static shared_ptr<DataSaveSkinEvent> Instance();
		//このイベントが発生したときに呼ばれる
		virtual void Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event)override;
	};


	//--------------------------------------------------------------------------------------
	//	class RunSkinEvent : public EventState<FbxMeshObject>;
	//	用途: スキン実行のイベント
	//--------------------------------------------------------------------------------------
	class RunSkinEvent : public EventState<FbxMeshObject>
	{
		RunSkinEvent(){}
	public:
		//イベントステートのインスタンスを得る
		static shared_ptr<RunSkinEvent> Instance();
		//このイベントが発生したときに呼ばれる
		virtual void Enter(const shared_ptr<FbxMeshObject>& Obj, const shared_ptr<Event>& event)override;
	};





}
//endof  basedx11

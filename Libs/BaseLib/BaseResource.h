/*!
@file BaseResource.h
@brief メモリ上に保持するリソース（テクスチャ、メッシュ、オーディオなど）

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/


#pragma once
#include "stdafx.h"

namespace basedx11{
	//--------------------------------------------------------------------------------------
	//	class BaseResource : public Object;
	/*!
	メモリ上に保持するリソースの親クラス
	*/
	//--------------------------------------------------------------------------------------
	class BaseResource : public Object{
	protected:
		//--------------------------------------------------------------------------------------
		//	BaseResource();
		/*!
		@breaf プロテクトコンスラトクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		BaseResource(){}
		//--------------------------------------------------------------------------------------
		//	virtual ~BaseResource();
		/*!
		@breaf プロテクトデストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~BaseResource(){}
	};

	//--------------------------------------------------------------------------------------
	//	class TextureResource : public BaseResource;
	/*!
	テクスチャリソース
	*/
	//--------------------------------------------------------------------------------------
	class TextureResource : public BaseResource{
	public:
		//--------------------------------------------------------------------------------------
		//	explicit TextureResource(
		//	const wstring& FileName	//ファイル名
		//	);
		/*!
		@breaf コンストラクタ
		@param const wstring& FileName	ファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		explicit TextureResource(const wstring& FileName,const wstring& TexType = L"WIC");
		//--------------------------------------------------------------------------------------
		//	virtual ~TextureResource();
		/*!
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~TextureResource();
		//アクセサ
		//--------------------------------------------------------------------------------------
		//	ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const;
		/*!
		@breaf シェーダーリソースビューの取得
		@param なし
		@return	このテクスチャのシェーダーリソースビュー
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() const;
		//--------------------------------------------------------------------------------------
		//	const wstring& GetTextureFileName() const;
		/*!
		@breaf テクスチャのファイル名の取得
		@param なし
		@return	このテクスチャのファイル名
		*/
		//--------------------------------------------------------------------------------------
		const wstring& GetTextureFileName() const;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class AudioResource : public BaseResource;
	/*!
	オーディオリソース（wav）クラス
	*/
	//--------------------------------------------------------------------------------------
	class AudioResource : public BaseResource{
	public:
		//--------------------------------------------------------------------------------------
		//	AudioResource(
		//	const wstring& FileName	//ファイル名
		//	);
		/*!
		@breaf コンストラクタ<br />
		ウェブファイル名を渡す
		@param const wstring& FileName	ファイル名
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		AudioResource(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		//	virtual ~AudioResource();
		/*!
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~AudioResource();
		//アクセサ
		//--------------------------------------------------------------------------------------
		//	const vector<byte>& GetSoundData()const;
		/*!
		@breaf サウンドデータの取得
		@param なし
		@return	このウェブリソースのサウンドデータ
		*/
		//--------------------------------------------------------------------------------------
		const vector<byte>& GetSoundData()const;
		//--------------------------------------------------------------------------------------
		//	WAVEFORMATEX*  GetOutputWaveFormatEx()const;
		/*!
		@breaf ウェブフォーマットの取得
		@param なし
		@return	このウェブリソースのWAVEFORMATEXウェブフォーマットのポインタ
		*/
		//--------------------------------------------------------------------------------------
		WAVEFORMATEX*  GetOutputWaveFormatEx() const;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MeshResource : public BaseResource;
	/*!
	メッシュ親クラス
	*/
	//--------------------------------------------------------------------------------------
	class MeshResource : public BaseResource{
	protected:
		//--------------------------------------------------------------------------------------
		//	MeshResource();
		/*!
		@breaf プロテクトコンストラクタ<br />
		派生クラスから構築する
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		MeshResource();
		//--------------------------------------------------------------------------------------
		//	virtual ~MeshResource();
		/*!
		@breaf プロテクトデストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~MeshResource();
		//派生クラスからのみアクセスできるアクセサ
		//--------------------------------------------------------------------------------------
		//	void SetVertexBuffer(
		//	ComPtr<ID3D11Buffer>& VertexBuffer	//頂点バッファ
		//	);
		/*!
		@breaf 頂点バッファの設定
		@param ComPtr<ID3D11Buffer>& VertexBuffer	頂点バッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetVertexBuffer(ComPtr<ID3D11Buffer>& VertexBuffer);
		//--------------------------------------------------------------------------------------
		//	void SetNumVertices(
		//		UINT NumVertices	//頂点数
		//	);
		/*!
		@breaf 頂点数の設定<br />
		頂点バッファの数と合わせなければならないので、慎重に使用する
		@param UINT NumVertices	頂点数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetNumVertices(UINT NumVertices);
		//--------------------------------------------------------------------------------------
		//	void SetIndexBuffer(
		//		ComPtr<ID3D11Buffer>& IndexBuffer	//インデックスバッファ
		//	);
		/*!
		@breaf インデックスバッファの設定
		@param ComPtr<ID3D11Buffer>& IndexBuffer	インデックスバッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetIndexBuffer(ComPtr<ID3D11Buffer>& IndexBuffer);
		//--------------------------------------------------------------------------------------
		//	void SetNumIndicis(
		//		UINT NumIndicis	//インデックス数
		//	);
		/*!
		@breaf インデックス数の設定<br />
		インデックスバッファの数と合わせなければならないので、慎重に使用する
		@param ComPtr<ID3D11Buffer>& IndexBuffer	インデックスバッファ
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void SetNumIndicis(UINT NumIndicis);
	public:
		//アクセサ
		//--------------------------------------------------------------------------------------
		//	ComPtr<ID3D11Buffer>& GetVertexBuffer() const;
		/*!
		@breaf 頂点バッファの取得
		@param なし
		@return	頂点バッファ
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer>& GetVertexBuffer() const;
		//--------------------------------------------------------------------------------------
		//	ComPtr<ID3D11Buffer>& GetIndexBuffer() const;
		/*!
		@breaf インデックスバッファの取得
		@param なし
		@return	インデックスバッファ
		*/
		//--------------------------------------------------------------------------------------
		ComPtr<ID3D11Buffer>& GetIndexBuffer() const;
		//--------------------------------------------------------------------------------------
		//	UINT GetNumVertices() const;
		/*!
		@breaf 頂点数の取得
		@param なし
		@return	頂点数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumVertices() const;
		//--------------------------------------------------------------------------------------
		//	UINT GetNumIndicis() const;
		/*!
		@breaf インデックス数の取得
		@param なし
		@return	インデックス数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumIndicis() const;
		//--------------------------------------------------------------------------------------
		//	virtual bool IsSkining() const;
		/*!
		@breaf スキニングするかどうか.<br />
		仮想関数なので、派生クラスで独自に多重定義する
		@param なし
		@return	スキニングする場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		virtual bool IsSkining() const{ return false; }
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class CommonMeshResource : public MeshResource;
	/*!
	@breaf プリミティブメッシュクラス.<br />
	プリミティブメッシュは、スタティック関数を使って生成する
	*/
	//--------------------------------------------------------------------------------------
	class CommonMeshResource : public MeshResource{
		//--------------------------------------------------------------------------------------
		//	CommonMeshResource();
		/*!
		@breaf プライベートコンストラクタ<br />
		構築はスタティック関数を利用する
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		CommonMeshResource();
	public:
		//--------------------------------------------------------------------------------------
		//	virtual ~CommonMeshResource();
		/*!
		@breaf デストラクタ
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~CommonMeshResource();
		//リソース構築
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateCommonMeshResource(
		//		const vector<VertexPositionNormalTexture>& Vertices,	//頂点の配列
		//		const vector<uint16_t>& Indices				//インデックスの配列
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf 頂点とインデックスを設定したコモンメッシュの作成
		@param	const vector<VertexPositionNormalTexture>& Vertices		頂点の配列
		@param	const vector<uint16_t>& Indices	インデックスの配列
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateCommonMeshResource(const vector<VertexPositionNormalTexture>& Vertices, const vector<uint16_t>& Indices, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateSquare(
		//		float Size,		//1辺のサイズ
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf 4角形平面の作成
		@param	float size		1辺のサイズ
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateSquare(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateCube(
		//		float Size,		//1辺のサイズ
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf キューブ（立方体）の作成
		@param	float size		1辺のサイズ
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateCube(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateSphere(
		//		float Diameter,		//直径
		//		size_t Tessellation,	//分割数
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf 球体の作成
		@param	float Diameter		直径
		@param	size_t Tessellation	分割数
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateSphere(float Diameter, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateCapsule(
		//		float Diameter,		//直径
		//		float Height,		//高さ
		//		size_t Tessellation, //分割数
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf カプセルの作成
		@param	float Diameter		直径
		@param	float Height,		高さ
		@param	size_t Tessellation	分割数
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateCapsule(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateCylinder(
		//		float Height,	//高さ
		//		float Diameter, //直径
		//		size_t Tessellation,	//分割数
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf シリンダーの作成
		@param	float Height,		高さ
		@param	float Diameter		直径
		@param	size_t Tessellation	分割数
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateCylinder(float Height, float Diameter, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateCone(
		//		float Diameter, //直径
		//		float Height, //高さ
		//		size_t Tessellation, //分割数
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf コーンの作成
		@param	float Diameter		直径
		@param	float Height,		高さ
		@param	size_t Tessellation	分割数
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateCone(float Diameter, float Height, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateTorus(
		//		float Diameter, //直径
		//		float Thickness, //ドーナッツの太さ
		//		size_t Tessellation, //分割数
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf トーラスの作成
		@param	float Diameter		直径
		@param	float Thickness		ドーナッツの太さ
		@param	size_t Tessellation	分割数
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateTorus(float Diameter, float Thickness, size_t Tessellation, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateTetrahedron(
		//		float Size,		//サイズ
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf 正4面体の作成
		@param	float Size		1辺のサイズ
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateTetrahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateOctahedron(
		//		float Size,		//サイズ
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf 正8面体の作成
		@param	float Size		1辺のサイズ
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateOctahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateDodecahedron(
		//		float Size,		//サイズ
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf 正12面体の作成
		@param	float Size		1辺のサイズ
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateDodecahedron(float Size, bool AccessWrite = false);
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<CommonMeshResource> CreateIcosahedron(
		//		float Size,		//サイズ
		//		bool AccessWrite = false	//頂点を変更できるかどうか
		//	);
		/*!
		@breaf 正20面体の作成
		@param	float Size		1辺のサイズ
		@param	bool AccessWrite = false	頂点を変更できるかどうか
		@return	リソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<CommonMeshResource> CreateIcosahedron(float Size, bool AccessWrite = false);

		//アクセサ
		//--------------------------------------------------------------------------------------
		//	vector<VertexPositionNormalTexture>& GetBackupVertices() const;
		/*!
		@breaf バックアップ頂点配列の取得<br />
		AccessWriteがtrueで作成されたリソースは、バックアップ頂点の配列を持つ<br />
		クライアントはこの配列を利用して、必要な個所のみ書き換えることができる
		@param	なし
		@return	バックアップ頂点配列
		*/
		//--------------------------------------------------------------------------------------
		vector<VertexPositionNormalTexture>& GetBackupVertices() const;
		//--------------------------------------------------------------------------------------
		//	void UpdateVirtexBuffer(
		//		const vector<VertexPositionNormalTexture>& NewBuffer	//頂点の配列
		//	);
		/*!
		@breaf 頂点の変更.<br />
		AccessWriteがtrueで作成されたリソースは、頂点の配列によって頂点を変更する。
		@param	const vector<VertexPositionNormalTexture>& NewBuffer	頂点の配列
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void UpdateVirtexBuffer(const vector<VertexPositionNormalTexture>& NewBuffer);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	class FbxSceneResource;

	//--------------------------------------------------------------------------------------
	//	struct	AnimationData;
	/*!
	アニメーションデータ構造体.
	*/
	//--------------------------------------------------------------------------------------
	struct	AnimationData
	{
		//!	スタートフレーム
		UINT	m_StartFrame;
		//!	フレームの長さ
		UINT	m_FrameLength;
		//!	ループするかどうか
		bool	m_IsLoop;
		//!	ループが終了したかどうか
		bool	m_IsLoopEnd;
		//!	1秒当たりのフレーム
		float	m_FramesParSecond;
		//--------------------------------------------------------------------------------------
		//	AnimationData();
		/*!
		@breaf コンストラクタ.
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		AnimationData()
		{
			ZeroMemory(this, sizeof(AnimationData));
		}
		//--------------------------------------------------------------------------------------
		//	AnimationData(
		//		UINT StartFrame,	//スタートフレーム
		//		UINT FrameLength,	//フレームの長さ
		//		bool bLoop,			//ループするかどうか
		//		float FramesParSecond = 30.0f	//1秒あたりのフレーム数
		//	);
		/*!
		@breaf コンストラクタ.
		@param UINT StartFrame	スタートフレーム
		@param UINT FrameLength	フレームの長さ
		@param bool bLoop	ループするかどうか
		@param float FramesParSecond = 30.0f	1秒あたりのフレーム数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		AnimationData(UINT StartFrame, UINT FrameLength, bool bLoop,
			float FramesParSecond = 30.0f): 
			m_StartFrame{ StartFrame },
			m_FrameLength{ FrameLength },
			m_IsLoop{ bLoop },
			m_IsLoopEnd{ false },
			m_FramesParSecond{ FramesParSecond }
		{}
	};

	//--------------------------------------------------------------------------------------
	//	struct	Bone;
	/*!
	Bone構造体.
	*/
	//--------------------------------------------------------------------------------------
	struct	Bone
	{
		//!基本ポーズへの行列
		Matrix4X4	m_BindPose;
		//!現在の行列
		Matrix4X4	m_CurrentPose;
		//!計算された現在の行列
		Matrix4X4	m_ConbinedPose;
		//--------------------------------------------------------------------------------------
		//	Bone();
		/*!
		@breaf コンストラクタ.
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		Bone()
		{
			::ZeroMemory(this, sizeof(Bone));
		}
	};

	//--------------------------------------------------------------------------------------
	//	struct Material;
	/*!
	マテリアル構造体.
	*/
	//--------------------------------------------------------------------------------------
	struct Material{
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

	//--------------------------------------------------------------------------------------
	//	class FbxMeshResource : public MeshResource;
	/*!
	FBXメッシュリソースクラス.
	*/
	//--------------------------------------------------------------------------------------
	class FbxMeshResource : public MeshResource{
		//--------------------------------------------------------------------------------------
		//	void CreateInstanceFromStaticFbx();
		/*!
		@breaf スタティックFBXからのインスタンスの構築.
		@param なし
		@return	なし（例外がthrowされる）
		*/
		//--------------------------------------------------------------------------------------
		void CreateInstanceFromStaticFbx();
		//--------------------------------------------------------------------------------------
		//	void CreateInstanceFromSkinFbx();
		/*!
		@breaf スキンメッシュFBXからのインスタンスの構築.
		@param なし
		@return	なし（例外がthrowされる）
		*/
		//--------------------------------------------------------------------------------------
		void CreateInstanceFromSkinFbx();
		//--------------------------------------------------------------------------------------
		//	void CreateMaterial();
		/*!
		@breaf FBXからマテリアルを読み込む.
		@param なし
		@return	なし（例外がthrowされる）
		*/
		//--------------------------------------------------------------------------------------
		void CreateMaterial();
	public:
		//--------------------------------------------------------------------------------------
		//	struct FbxMeshDeleter;
		/*!
		FBXメッシュのデリーター関数オブジェクト.
		*/
		//--------------------------------------------------------------------------------------
		struct FbxMeshDeleter
		{
			//--------------------------------------------------------------------------------------
			//	void operator()(
			//		FbxMesh *p	//FBXメッシュのポインタ
			//	);
			/*!
			@breaf FBXメッシュのdelete.<br />
			削除はマネージャが行うので何もしない
			@param FbxMesh *p	FBXメッシュのポインタ
			@return	なし
			*/
			//--------------------------------------------------------------------------------------
			void operator()(FbxMesh *p){
				//削除はマネージャが行うので何もしない
			}
		};
		//--------------------------------------------------------------------------------------
		//	struct FbxSkinDeleter;
		/*!
		FBXスキンのデリーター関数オブジェクト.
		*/
		//--------------------------------------------------------------------------------------
		struct FbxSkinDeleter
		{
			//--------------------------------------------------------------------------------------
			//	void operator()(
			//		FbxSkin *p	//FBXスキンのポインタ
			//	);
			/*!
			@breaf FBXスキンのdelete.<br />
			削除はマネージャが行うので何もしない
			@param FbxSkin *p	FBXスキンのポインタ
			@return	なし
			*/
			//--------------------------------------------------------------------------------------
			void operator()(FbxSkin *p){
				//削除はマネージャが行うので何もしない
			}
		};
		//構築と破棄
		//--------------------------------------------------------------------------------------
		//	FbxMeshResource(
		//		shared_ptr<FbxSceneResource> FbxSceneResourcePtr,	//FBXシーンリソース
		//		FbxMesh* pFbxMesh,	//FBXメッシュリソース
		//		bool NeedStatic = false	//スキンメッシュでもスタティックメッシュにするかどうか
		//	);
		/*!
		@breaf コンストラクタ.<br/>
		FBXメッシュからデータを読み出す
		@param shared_ptr<FbxSceneResource> FbxSceneResourcePtr	FBXシーンリソース
		@param FbxMesh* pFbxMesh	FBXメッシュリソース
		@param bool NeedStatic = false	スキンメッシュでもスタティックメッシュにするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		FbxMeshResource(shared_ptr<FbxSceneResource> FbxSceneResourcePtr, FbxMesh* pFbxMesh, bool NeedStatic = false);
		//--------------------------------------------------------------------------------------
		//	virtual ~FbxMeshResource();
		/*!
		@breaf デストラクタ.
		@param　なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~FbxMeshResource();
		//初期化
		//--------------------------------------------------------------------------------------
		//	virtual void Create();
		/*!
		@breaf 初期化
		@param　なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Create();
		//アクセサ
		//--------------------------------------------------------------------------------------
		//	virtual bool IsSkining() const;
		/*!
		@breaf スキニングするかどうか
		@param　なし
		@return	スキニングする場合はtrue
		*/
		//--------------------------------------------------------------------------------------
		virtual bool IsSkining() const;
		//--------------------------------------------------------------------------------------
		//	const unique_ptr<FbxMesh, FbxMeshDeleter>& GetFbxMesh() const;
		/*!
		@breaf FBXメッシュの取得
		@param　なし
		@return	FBXメッシュのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		const unique_ptr<FbxMesh, FbxMeshDeleter>& GetFbxMesh() const;
		//--------------------------------------------------------------------------------------
		//	const unique_ptr<FbxSkin, FbxSkinDeleter>& GetFbxSkin() const;
		/*!
		@breaf FBXスキンの取得
		@param　なし
		@return	FBXスキンのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		const unique_ptr<FbxSkin, FbxSkinDeleter>& GetFbxSkin() const;
		//--------------------------------------------------------------------------------------
		//	UINT GetNumBones() const;
		/*!
		@breaf ボーン数の取得
		@param　なし
		@return	ボーン数
		*/
		//--------------------------------------------------------------------------------------
		UINT GetNumBones() const;
		//--------------------------------------------------------------------------------------
		//	const vector<Material>& GetMaterialVec()const;
		/*!
		@breaf マテリアル配列の取得
		@param　なし
		@return	マテリアル配列
		*/
		//--------------------------------------------------------------------------------------
		const vector<Material>& GetMaterialVec()const;
		//--------------------------------------------------------------------------------------
		//	const AnimationData& GetAnimationData(const string& AnimeName) const;
		/*!
		@breaf アニメーションデータを得る。無ければ例外
		@param　なし
		@return	アニメーションデータ
		*/
		//--------------------------------------------------------------------------------------
		const AnimationData& GetAnimationData(const string& AnimeName) const;
		//--------------------------------------------------------------------------------------
		//	const vector< Bone >& GetBonesVec() const;
		/*!
		@breaf ボーンの配列を得る
		@param　なし
		@return	ボーンの配列
		*/
		//--------------------------------------------------------------------------------------
		const vector< Bone >& GetBonesVec() const;
		//操作
		//--------------------------------------------------------------------------------------
		//	bool GenerateCurrentPose(
		//		vector< Bone >& tgtBones,			//変更すべきボーン配列
		//		AnimationData& rAnimationData,		//指定のアニメーションデータ
		//		float CurrentTime					//時間指定
		//	);
		/*!
		@breaf 指定のアニメーションの設定されている時間における、カレントポーズと合成変換行列を再計算する
		@param　vector< Bone >& tgtBones	変更すべきボーン配列
		@param　AnimationData& rAnimationData	指定のアニメーションデータ
		@param　float CurrentTime	時間指定
		@return	ループなしで最後に到達したらtrue
		*/
		//--------------------------------------------------------------------------------------
		bool GenerateCurrentPose(vector<Bone>& tgtBones, AnimationData& rAnimationData, float CurrentTime);
		//--------------------------------------------------------------------------------------
		//	void AddAnimation(
		//		const char* Name,	//アニメーション名
		//		UINT StartFrame,	//スタート位置
		//		UINT FrameLength,	//長さ
		//		bool Loop,			//ループするかどうか
		//		float FramesParSecond = 30.0f //１秒あたりのフレーム数
		//	);
		/*!
		@breaf アニメーションを設定する（すでにその定義があれば、差し替える）
		@param　const char* Name	アニメーション名
		@param　UINT StartFrame,	スタート位置
		@param　UINT FrameLength	長さ
		@param　bool Loop,			ループするかどうか
		@param　float FramesParSecond = 30.0f １秒あたりのフレーム数
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void AddAnimation(const char* Name, UINT StartFrame, UINT FrameLength, bool Loop,
			float FramesParSecond = 30.0f);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class FbxSceneResource : public BaseResource;
	/*!
	FBXシーンリソースクラス.<br />
	一つのFBXファイルは１つのFbxSceneResourceとして管理する
	*/
	//--------------------------------------------------------------------------------------
	class FbxSceneResource : public BaseResource{
		//--------------------------------------------------------------------------------------
		//	void CreateFbxMeshVector(
		//		FbxNode* pFbxNode				//ターゲットとなるノード
		//	);
		/*!
		@breaf FBXメッシュ配列を再帰的に読み込む
		@param　FbxNode* pFbxNode	ターゲットとなるノード
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		void CreateFbxMeshVector(FbxNode* pFbxNode);
	protected:
		//--------------------------------------------------------------------------------------
		//	FbxSceneResource(
		//		const wstring& DataDir,	//データディレクトリ
		//		const wstring& FileName,	//FBXファイル名
		//		const string& SceneName,	//シーン名
		//		bool NeedStatic				//static構築をするかどうか
		//	);
		/*!
		@breaf プロテクトコンストラクタ.<br/>
		FBXファイルからFBXシーンを構築する
		@param const wstring& DataDir	データディレクトリ
		@param const wstring& FileName	FBXファイル名
		@param const string& SceneName	シーン名
		@param bool NeedStatic	static構築をするかどうか
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		FbxSceneResource(const wstring& DataDir,
			const wstring& FileName, const string& SceneName, bool NeedStatic);
		//初期化
		//--------------------------------------------------------------------------------------
		//	virtual void Create();
		/*!
		@breaf 初期化
		@param　なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual void Create();
	public:
		//--------------------------------------------------------------------------------------
		//	struct FbxSceneDeleter;
		/*!
		FBXシーンのデリーター関数オブジェクト.
		*/
		//--------------------------------------------------------------------------------------
		struct FbxSceneDeleter
		{
			//--------------------------------------------------------------------------------------
			//	void operator()(
			//		FbxScene *p	//FBXシーンのポインタ
			//	);
			/*!
			@breaf FBXシーンのdelete.<br />
			削除はマネージャが行うので何もしない
			@param FbxScene *p	FBXシーンのポインタ
			@return	なし
			*/
			//--------------------------------------------------------------------------------------
			void operator()(FbxScene *p){
				//削除はマネージャが行うので何もしない
			}
		};
		//--------------------------------------------------------------------------------------
		//	virtual ~FbxSceneResource();
		/*!
		@breaf デストラクタ.
		@param なし
		@return	なし
		*/
		//--------------------------------------------------------------------------------------
		virtual ~FbxSceneResource();
		//--------------------------------------------------------------------------------------
		//	static shared_ptr<FbxSceneResource> CreateFbxScene(
		//		const wstring& DataDir,		//データディレクトリ
		//		const wstring& FileName,	//ファイル名
		//		const string& SceneName = "",	//シーン名
		//		bool NeedStatic = false		//staticメッシュとして構築するかどうか
		//	);
		/*!
		@breaf 構築用static関数.
		@param const wstring& DataDir	データディレクトリ
		@param const wstring& FileName		ファイル名
		@param const string& SceneName = ""	シーン名
		@param bool NeedStatic = false	staticメッシュとして構築するかどうか
		@return　FBXシーンリソース
		*/
		//--------------------------------------------------------------------------------------
		static shared_ptr<FbxSceneResource> CreateFbxScene(const wstring& DataDir,
			const wstring& FileName, const string& SceneName = "", bool NeedStatic = false);
		//アクセサ
		//--------------------------------------------------------------------------------------
		//	wstring GetDataDir() const;
		/*!
		@breaf データディレクトリを得る
		@param　なし
		@return	データディレクトリ
		*/
		//--------------------------------------------------------------------------------------
		wstring GetDataDir() const;
		//--------------------------------------------------------------------------------------
		//	wstring GetFileName() const;
		/*!
		@breaf ファイル名を得る
		@param　なし
		@return	ファイル名
		*/
		//--------------------------------------------------------------------------------------
		wstring GetFileName() const;
		//--------------------------------------------------------------------------------------
		//	wstring GetFileName() const;
		/*!
		@breaf FBXシーン名を得る
		@param　なし
		@return	FBXシーン名
		*/
		//--------------------------------------------------------------------------------------
		string GetFbxSceneName() const;
		//--------------------------------------------------------------------------------------
		//	shared_ptr<FbxMeshResource> GetFbxMeshResource(
		//		size_t Index	//FBX内のメッシュID
		//	) const;
		/*!
		@breaf メッシュリソースを得る
		@param　size_t Index	FBX内のメッシュID
		@return	FBXメッシュリソースのスマートポインタ
		*/
		//--------------------------------------------------------------------------------------
		shared_ptr<FbxMeshResource> GetFbxMeshResource(size_t Index) const;
		//--------------------------------------------------------------------------------------
		//	size_t GetFbxMeshResourceSize() const;
		/*!
		@breaf メッシュリソース数を得る
		@param　なし
		@return	FBXシーンに含まれるメッシュリソース数
		*/
		//--------------------------------------------------------------------------------------
		size_t GetFbxMeshResourceSize() const;
		//--------------------------------------------------------------------------------------
		//	vector< shared_ptr<FbxMeshResource> >& GetFbxMeshResourceVec() const;
		/*!
		@breaf メッシュリソース配列を得る
		@param　なし
		@return	FBXシーンに含まれるメッシュリソース配列
		*/
		//--------------------------------------------------------------------------------------
		vector< shared_ptr<FbxMeshResource> >& GetFbxMeshResourceVec() const;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



}

//endof  basedx11

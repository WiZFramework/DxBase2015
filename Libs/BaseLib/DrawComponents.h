
/*!
@file DrawComponents.h
@brief コンポーネント（描画系）

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
	//	用途: シャドウマップコンポーネント（前処理用）
	//--------------------------------------------------------------------------------------
	class Shadowmap : public Component{
	public:
		explicit Shadowmap(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~Shadowmap();
		//アクセサ
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

		//操作
		virtual void Update()override{}
		virtual void Draw();
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class DrawComponent : public Component;
	//	用途: 描画コンポーネントの親クラス
	//--------------------------------------------------------------------------------------
	class DrawComponent : public Component{
	protected:
		DrawComponent(const shared_ptr<GameObject>& GameObjectPtr) :
			Component(GameObjectPtr){}
		virtual ~DrawComponent(){}
	public:
		//各オブジェクトごとにボーンを所持しておくポインタ
		//シャドウマップから親クラスで参照できるように仮想関数にする
		virtual const vector< Bone >* GetVecLocalBonesPtr() const{ return nullptr; }
	};

	//--------------------------------------------------------------------------------------
	//	class SimpleDirectDraw : public DrawComponent;
	//	用途: SimpleDirectDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	class SimpleDirectDraw : public DrawComponent{
	public:
		explicit SimpleDirectDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~SimpleDirectDraw();
		//アクセサ
		const Color4& GetDiffuse() const;
		void SetDiffuse(const Color4& c);
		D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveTopology() const;
		void SetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY t);
		//頂点配列を渡して頂点バッファを作成する
		void CreateVertex(const vector<VertexPosition>& Verteces);
		//操作
		virtual void Update()override{}
		virtual void Draw();
	private:
		// pinplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class SimplePNTDraw : public DrawComponent;
	//	用途: SimplePNTDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	class SimplePNTDraw : public DrawComponent{
	public:
		explicit SimplePNTDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~SimplePNTDraw();
		//アクセサ
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

		//操作
		virtual void Update()override{}
		virtual void Draw();
	private:
		// pinplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class BasicPNTDraw : public DrawComponent;
	//	用途: BasicPNTDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	class BasicPNTDraw : public DrawComponent{
		void DrawWithShadow();
		void DrawNotShadow();
	public:
		//構築と破棄
		explicit BasicPNTDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~BasicPNTDraw();
		//アクセサ
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


		//操作
		virtual void Update()override{}
		virtual void Draw();
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




	//--------------------------------------------------------------------------------------
	//	class BasicFbxPNTDraw : public DrawComponent;
	//	用途: BasicFbxPNTDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	class BasicFbxPNTDraw : public DrawComponent{
		void DrawWithShadow();
		void DrawNotShadow();
	protected:
		//ボーン行列をリソースから読み込む
		//親クラスでは何もしない
		virtual void SetBoneVec(){}
	public:
		//構築と破棄
		explicit BasicFbxPNTDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~BasicFbxPNTDraw();
		//アクセサ
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

		//操作
		virtual void Update()override{}
		virtual void Draw();
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class BasicFbxPNTBoneDraw : public BasicFbxPNTDraw;
	//	用途: BasicFbxPNTBoneDraw描画コンポーネント
	//--------------------------------------------------------------------------------------
	class BasicFbxPNTBoneDraw : public BasicFbxPNTDraw{
		void DrawWithShadow();
		void DrawNotShadow();
	protected:
		//ボーン行列をリソースから読み込む
		virtual void SetBoneVec()override;
	public:
		//構築と破棄
		explicit BasicFbxPNTBoneDraw(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~BasicFbxPNTBoneDraw();
		//アクセサ
		//各オブジェクトごとにボーンを所持しておくポインタ
		const vector< Bone >& GetVecLocalBones() const;
		virtual const vector< Bone >* GetVecLocalBonesPtr() const;

		//カレントアニメーション
		const string& GetCurrentAnimation() const;
		void SetCurrentAnimation(const string& AnemationName,float StartTime = 0.0f);
		//現在のアニメーションが終了しているかどうか
		bool IsTargetAnimeEnd() const;

		//指定したIDのボーンの現在の行列を取得する
		void GetBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const;

		//指定したIDのボーンの現在のローカル行列を取得する（親子関係を構築するなど用）
		void GetLocalBoneMatrix(UINT BoneId, Matrix4X4& Matrix) const;

		//操作
		virtual void Update()override;
		virtual void Draw()override;
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};






}

//end basedx11

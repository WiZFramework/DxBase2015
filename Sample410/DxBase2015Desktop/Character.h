#pragma once

#include "stdafx.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	struct SplitBoxData;
	//	用途:分割されたオブジェクトのデータ
	//--------------------------------------------------------------------------------------
	struct SplitBoxData{
		bool m_Enabled;	//有効かどうか
		float m_Time;	//経過時間
		Vector3 m_Scale;	//現在のスケール
		Quaternion m_Qt;	//現在の回転
		Vector3 m_Pos;	//現在の位置
		Vector3 m_Velocity;	//速度
		Quaternion m_QtVelocity;	//回転速度
		Vector3 m_GravityVelocity;	//重力による速度
		SplitBoxData(const Vector3&scale, const Vector3&rot,const Vector3&pos, const Vector3&velo, const Vector3&rotvelo) :
			m_Enabled(true),
			m_Time(0),
			m_Scale(scale),
			m_Qt(),
			m_Pos(pos),
			m_Velocity(velo),
			m_QtVelocity(),
			m_GravityVelocity(0.0f,0.0f,0.0f)
		{
			m_Qt.RotationRollPitchYawFromVector(rot);
			m_QtVelocity.RotationRollPitchYawFromVector(rotvelo);
		}
		void UpdateData(float timespan){
			if (m_Enabled){
				m_Time += timespan;
				m_Pos += m_Velocity * timespan;
				m_GravityVelocity += Vector3(0, -9.8f, 0) * timespan;
				m_Pos += m_GravityVelocity * timespan;
				//回転の追加
				m_Qt *= m_QtVelocity;
				//m_PosのYが0になるまで更新する
				//場合によってはm_PosのYが0ではないケースも設定できる
				//砕けた状態を少し見せたい場合は、m_Time条件で消す形にもできる
				//その場合、インスタンスのMAXに引っかからないよう注意
				if (m_Pos.y <= 0.0f || m_Time >= 1.0f){
					m_Enabled = false;
				}
			}
		}
	};

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject{
		shared_ptr< StateMachine<FixedBox> >  m_StateMachine;	//ステートマシーン
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
		//分割状態の時のデータの配列
		vector<SplitBoxData> m_SplitBoxDataVec;
	public:
		//構築と破棄
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
			);
		virtual ~FixedBox();
		//アクセサ
		shared_ptr< StateMachine<FixedBox> > GetStateMachine() const{
			return m_StateMachine;
		}
		//初期化
		virtual void Create() override;
		//操作
		virtual void Update() override;
		virtual void Update3() override;
		//モーション系
		//プレイヤーがヒットしたか
		bool IsPlayerHit() const;
		//分割用の行列配列の作成
		void CreateSplitVec(size_t splitcount);
		//分割用の行列配列の更新
		void UpdateSplitVec();
	};

	//--------------------------------------------------------------------------------------
	//	class FixedBoxDefaultState : public ObjState<FixedBox>;
	//	用途: 通常
	//--------------------------------------------------------------------------------------
	class FixedBoxDefaultState : public ObjState<FixedBox>
	{
		FixedBoxDefaultState(){}
	public:
		//ステートのインスタンス取得
		static shared_ptr<FixedBoxDefaultState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<FixedBox>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<FixedBox>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<FixedBox>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class FixedBoxSplitState : public ObjState<FixedBox>;
	//	用途: 分割状態
	//--------------------------------------------------------------------------------------
	class  FixedBoxSplitState : public ObjState<FixedBox>
	{
		FixedBoxSplitState(){}
	public:
		//ステートのインスタンス取得
		static shared_ptr< FixedBoxSplitState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<FixedBox>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<FixedBox>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<FixedBox>& Obj)override;
	};




	//--------------------------------------------------------------------------------------
	//const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureMatrixLayout[];
	//用途: 位置と法線とテクスチャと行列を持つ入力レイアウトの定義
	//--------------------------------------------------------------------------------------
	const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureMatrixLayout[] =
	{
		{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "MATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
		{ "MATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
	};

	//--------------------------------------------------------------------------------------
	// struct VertexPositionNormalTextureMatrix;
	//用途: 位置と法線とテクスチャと行列を持つ頂点の定義
	//--------------------------------------------------------------------------------------
	struct  VertexPositionNormalTextureMatrix
	{
		VertexPositionNormalTextureMatrix()
		{ }

		VertexPositionNormalTextureMatrix(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT2 const& textureCoordinate,
			XMFLOAT4X4 const& matrix)
			: position(position),
			normal(normal),
			textureCoordinate(textureCoordinate),
			matrix(matrix)
		{ }

		VertexPositionNormalTextureMatrix(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR textureCoordinate,
			FXMMATRIX const& matrix)
		{
			XMStoreFloat3(&this->position, position);
			XMStoreFloat3(&this->normal, normal);
			XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
			XMStoreFloat4x4(&this->matrix, matrix);
		}

		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 textureCoordinate;
		XMFLOAT4X4 matrix;

		static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
			return VertexPositionNormalTextureMatrixLayout;
		}
		static UINT GetNumElements(){
			return  ARRAYSIZE(VertexPositionNormalTextureMatrixLayout);
		}
	};



	//インスタンスシャドウマップ用コンスタントバッファ構造体
	struct InstanceShadowmapConstantBuffer
	{
		XMMATRIX mView;
		XMMATRIX mProj;
		InstanceShadowmapConstantBuffer() {
			memset(this, 0, sizeof(InstanceShadowmapConstantBuffer));
		};
	};
	//--------------------------------------------------------------------------------------
	//	class CBInstanceShadowmap : public ConstantBuffer<CBInstanceShadowmap,InstanceShadowmapConstantBuffer>;
	//	用途: インスタンスシャドウマップコンスタントバッファ
	//--------------------------------------------------------------------------------------
	class CBInstanceShadowmap : public ConstantBuffer<CBInstanceShadowmap, InstanceShadowmapConstantBuffer>{
	public:
	};

	//--------------------------------------------------------------------------------------
	//	class VSInstanceShadowmap : public VertexShader<VSInstanceShadowmap, VertexPositionNormalTextureMatrix>;
	//	用途: VSInstanceShadowmap頂点シェーダ
	//--------------------------------------------------------------------------------------
	class VSInstanceShadowmap : public VertexShader<VSInstanceShadowmap, VertexPositionNormalTextureMatrix>{
	public:
		//構築
		VSInstanceShadowmap();
	};


	//--------------------------------------------------------------------------------------
	//	class InstanceShadowmap : public Shadowmap;
	//	用途: インスタンスシャドウマップコンポーネント
	//--------------------------------------------------------------------------------------
	class InstanceShadowmap : public Shadowmap{
	public:
		explicit InstanceShadowmap(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~InstanceShadowmap();
		//操作
		virtual void Draw();
	};





	//--------------------------------------------------------------------------------------
	//	struct InstanceConstantBuffer;
	//	用途: 入力バッファのCPU側構造体
	//--------------------------------------------------------------------------------------
	struct InstanceConstantBuffer
	{
		Matrix4X4 View;
		Matrix4X4 Projection;
		Vector4 LightDir;
		Vector4 Param;	//汎用パラメータ
		Vector4 LPos;
		Vector4 EyePos;
		Matrix4X4 LView;
		Matrix4X4 LProjection;
		InstanceConstantBuffer() {
			memset(this, 0, sizeof(InstanceConstantBuffer));
		};
	};

	//シェーダ宣言
	//コンスタントバッファ
	class CBInstance : public ConstantBuffer < CBInstance, InstanceConstantBuffer>{
	public: 
	};
	//頂点シェーダ
	class VSInstance : public VertexShader<VSInstance, VertexPositionNormalTextureMatrix> {
	public: 
		VSInstance();
	};
	//ピクセルシェーダ
	class PSInstance : public PixelShader<PSInstance>
	{
	public:
		PSInstance();
	};




	//--------------------------------------------------------------------------------------
	//	class DrawBoxManager : public GameObject;
	//	用途: 固定のボックスの描画マネージャ
	//--------------------------------------------------------------------------------------
	class DrawBoxManager : public GameObject{
		//インスタンスの最大値
		const size_t m_MaxInstance;
		wstring m_MeshKey;
		wstring m_TextureKey;
		vector<Matrix4X4> m_InstanceVec;
		//行列用の頂点バッファ
		ComPtr<ID3D11Buffer> m_MatrixBuffer;
		//行列用の頂点バッファの作成
		void CreateMatrixBuffer();
	public:
		//構築と破棄
		DrawBoxManager(const shared_ptr<Stage>& StagePtr, size_t MaxInstance, const wstring& MeshKey,const wstring& TextureKey);
		virtual ~DrawBoxManager();
		//初期化
		virtual void Create() override;
		//アクセサ
		const ComPtr<ID3D11Buffer>& GetMatrixBuffer() const{
			return m_MatrixBuffer;
		}
		const vector<Matrix4X4>& GetInstanceVec(){
			return m_InstanceVec;
		}
		//操作
		//スケール、回転、位置で追加
		void AddInstanceVectors(const Vector3& Scale, const Vector3& Rot, const Vector3& Pos);
		//行列で追加
		void AddInstanceMatrix(const Matrix4X4& Mat);
		//仮想関数
		virtual void Update3() override;
		virtual void Draw() override;
	};


}
//endof  basedx11

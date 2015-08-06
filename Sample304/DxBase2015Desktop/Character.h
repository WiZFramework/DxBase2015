#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class FixedBox : public GameObject;
	//	用途: 固定のボックス
	//--------------------------------------------------------------------------------------
	class FixedBox : public GameObject{
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//構築と破棄
		FixedBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
			);
		virtual ~FixedBox();
		//初期化
		virtual void Create() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	class MoveBox : public GameObject;
	//	用途: 上下移動するボックス
	//--------------------------------------------------------------------------------------
	class MoveBox : public GameObject{
		Vector3 m_Scale;
		Vector3 m_Rotation;
		Vector3 m_Position;
	public:
		//構築と破棄
		MoveBox(const shared_ptr<Stage>& StagePtr,
			const Vector3& Scale,
			const Vector3& Rotation,
			const Vector3& Position
			);
		virtual ~MoveBox();
		//初期化
		virtual void Create() override;
		//操作
	};

	//--------------------------------------------------------------------------------------
	//	class NumberSprite : public GameObject;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	class NumberSprite : public GameObject{
		Vector3 m_StartPos;
		vector< vector<VertexPositionColorTexture> > m_NumberBurtexVec;
		float m_TotalTime;
	public:
		//構築と破棄
		NumberSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~NumberSprite();
		//初期化
		virtual void Create() override;
		//変化
		virtual void Update() override;
	};

	//--------------------------------------------------------------------------------------
	//	class BarSprite : public GameObject;
	//	用途: 配置オブジェクト
	//--------------------------------------------------------------------------------------
	class BarSprite : public GameObject{
		Vector3 m_StartPos;
		vector< vector<VertexPositionColorTexture> > m_BarBurtexVec;
		//最大時間（持ち時間）
		float m_MaxTime;
		//残り時間
		float m_LastTime;
	public:
		//構築と破棄
		BarSprite(shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~BarSprite();
		//初期化
		virtual void Create() override;
		//変化
		virtual void Update() override;
	};



}
//endof  basedx11

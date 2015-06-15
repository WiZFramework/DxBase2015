#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class MoveObject : public GameObject;
	//	用途: 動くオブジェクト
	//--------------------------------------------------------------------------------------
	class MoveObject : public GameObject{
		Vector3 m_StartPos;
		Vector3 m_StartVelocity;
		bool m_Type;
	public:
		//構築と破棄
		MoveObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos, const Vector3& StartVelocity, bool Type);
		virtual ~MoveObject();
		//初期化
		virtual void Create() override;
		//更新
		virtual void Update2() override;
	};

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



}
//endof  basedx11

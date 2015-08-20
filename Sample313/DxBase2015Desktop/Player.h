#pragma once

#include "stdafx.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	class Player : public GameObject;
	//	用途: プレイヤー
	//--------------------------------------------------------------------------------------
	class Player : public GameObject{
		shared_ptr< StateMachine<Player> >  m_StateMachine;	//ステートマシーン
		//移動の向きを得る
		Vector3 GetAngle();
		//最高速度
		float m_MaxSpeed;
		//減速率
		float m_Decel;
		//質量
		float m_Mass;
		//プレイヤーの保存用csv
		CsvFile m_PlayerCsv;
	public:
		//構築と破棄
		Player(const shared_ptr<Stage>& StagePtr);
		virtual ~Player();
		//初期化
		virtual void Create() override;
		//アクセサ
		shared_ptr< StateMachine<Player> > GetStateMachine() const{
			return m_StateMachine;
		}
		//モーションを実装する関数群
		//移動して向きを移動方向にする
		void MoveRotationMotion();

		//Aボタンでジャンプするどうかを得る
		bool IsJumpMotion();
		//Aボタンでジャンプする瞬間の処理
		void JumpMotion();
		//Aボタンでジャンプしている間の処理
		//ジャンプ終了したらtrueを返す
		bool JumpMoveMotion();

		//更新
		virtual void Update() override;
		virtual void Update2() override;
		virtual void Update3() override;
	};

	//--------------------------------------------------------------------------------------
	//	class DefaultState : public ObjState<Player>;
	//	用途: 通常移動
	//--------------------------------------------------------------------------------------
	class DefaultState : public ObjState<Player>
	{
		DefaultState(){}
	public:
		//ステートのインスタンス取得
		static shared_ptr<DefaultState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class JumpState : public ObjState<Player>;
	//	用途: ジャンプ状態
	//--------------------------------------------------------------------------------------
	class JumpState : public ObjState<Player>
	{
		JumpState(){}
	public:
		//ステートのインスタンス取得
		static shared_ptr<JumpState> Instance();
		//ステートに入ったときに呼ばれる関数
		virtual void Enter(const shared_ptr<Player>& Obj)override;
		//ステート実行中に毎ターン呼ばれる関数
		virtual void Execute(const shared_ptr<Player>& Obj)override;
		//ステートにから抜けるときに呼ばれる関数
		virtual void Exit(const shared_ptr<Player>& Obj)override;
	};



}
//endof  basedx11

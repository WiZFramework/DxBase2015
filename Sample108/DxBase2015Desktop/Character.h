#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class SeekObject : public GameObject;
	//	�p�r: �ǂ�������z�u�I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class SeekObject : public GameObject{
		shared_ptr< StateMachine<SeekObject> >  m_StateMachine;	//�X�e�[�g�}�V�[��
		Vector3 m_StartPos;
		float m_BaseY;
		float m_StateChangeSize;
		//���[�e�B���e�B�֐��Q
		//�v���C���[�̈ʒu��Ԃ�
		Vector3 GetPlayerPosition() const;
		//�v���C���[�܂ł̋�����Ԃ�
		float GetPlayerLength() const;
	public:
		//�\�z�Ɣj��
		SeekObject(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~SeekObject();
		//������
		virtual void Create() override;
		//�A�N�Z�T
		shared_ptr< StateMachine<SeekObject> > GetStateMachine() const{
			return m_StateMachine;
		}
		//���[�V��������������֐��Q
		void  SeekStartMoton();
		bool  SeekUpdateMoton();
		void  SeekEndMoton();

		void  ArriveStartMoton();
		bool  ArriveUpdateMoton();
		void  ArriveEndMoton();

		//����
		virtual void Update() override;
		virtual void Update2() override;
		virtual void Update3() override;
	};

	//--------------------------------------------------------------------------------------
	//	class FarState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[���牓���Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	class FarState : public ObjState<SeekObject>
	{
		FarState(){}
	public:
		static shared_ptr<FarState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};

	//--------------------------------------------------------------------------------------
	//	class NearState : public ObjState<SeekObject>;
	//	�p�r: �v���C���[����߂��Ƃ��̈ړ�
	//--------------------------------------------------------------------------------------
	class NearState : public ObjState<SeekObject>
	{
		NearState(){}
	public:
		static shared_ptr<NearState> Instance();
		virtual void Enter(const shared_ptr<SeekObject>& Obj)override;
		virtual void Execute(const shared_ptr<SeekObject>& Obj)override;
		virtual void Exit(const shared_ptr<SeekObject>& Obj)override;
	};



}
//endof  basedx11

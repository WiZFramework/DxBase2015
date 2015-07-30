#pragma once
#include "stdafx.h"

namespace basedx11{
	
	/**
	 * @class StageSelectAnimation
	 * @brief �X�e�[�W�Z���N�g���̃A�j���[�V����.
	 * @author Program ���� �ʎ�, Design �Ð� ���
	 * @date 2015/05/21 Start
	 * @date 2015/07/28 Release Update
	 */
	class StageSelectAnimation : public SS5ssae 
	{
	public : 
		//	---------- Method ------------.
		StageSelectAnimation( shared_ptr<Stage>& StagePtr, const wstring& BaseDir, const wstring& AnimationName ) ;
		virtual ~StageSelectAnimation () {}

		virtual void Create() override ;

		virtual void Update() ;

		void PlaySE ( const wstring& i_wsSEName ) ;
		//	------------------------------.

		//	---------- Accessor ----------.
		shared_ptr< StateMachine<StageSelectAnimation> > GetStateMachine() const
		{
			return m_StateMachine;
		}

		const float GetCursorLimit () const
		{
			return m_fCursorLimit ;
		}

		const bool GetIsCursorMove () const
		{
			return m_bIsCursorMove ;
		}

		void SetIsCursorMove ( const bool i_bIsCursorMove )
		{
			m_bIsCursorMove= i_bIsCursorMove ;
		}
		//	------------------------------.

		//	----------- Member -----------.
		//	NONE.
		//	------------------------------.

	private :
		//	----------- Method -----------.
		//	: �X�e�B�b�N�̏�Ԃ��Ǘ�����.
		void StickManage () ;
		//	------------------------------.

		//	----------- Member -----------.
		shared_ptr< StateMachine<StageSelectAnimation> >  m_StateMachine ;	//	: �X�e�[�g�}�V�[��.

		bool m_bIsCursorMove ;	//	: �J�[�\��������\���ǂ���.
		float m_fCursorLimit ;	//	: �X�e�B�b�N���|���ꂽ�Ɣ��f���鐧��l.
		//	------------------------------.
	};


	//--------------------------------------------------------------------------------------
	//	class OpeningState : public ObjState<StageSelectAnimation>
	//	�p�r: �X�e�[�W�Z���N�g��ʓW�J�A�j���[�V�����X�e�[�g.
	//--------------------------------------------------------------------------------------
	class OpeningState : public ObjState<StageSelectAnimation>
	{
		OpeningState(){}
	public:

		static shared_ptr<OpeningState> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class OpenToStage1State : public ObjState<StageSelectAnimation>
	//	�p�r: �I�[�v�j���O����X�e�[�W1�Z���N�g�A�j���[�V�����X�e�[�g.
	//--------------------------------------------------------------------------------------
	class OpenToStage1State : public ObjState<StageSelectAnimation>
	{
		OpenToStage1State(){}
	public:

		static shared_ptr<OpenToStage1State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override;
	};


	//--------------------------------------------------------------------------------------
	//	class Stage1ToStage2State : public ObjState<StageSelectAnimation>
	//	�p�r: �X�e�[�W1����X�e�[�W2�Z���N�g�A�j���[�V�����X�e�[�g.
	//--------------------------------------------------------------------------------------
	class Stage1ToStage2State : public ObjState<StageSelectAnimation>
	{
		Stage1ToStage2State(){}
	public:

		static shared_ptr<Stage1ToStage2State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class Stage2ToStage3State : public ObjState<StageSelectAnimation>
	//	�p�r: �X�e�[�W2����X�e�[�W3�Z���N�g�A�j���[�V�����X�e�[�g.
	//--------------------------------------------------------------------------------------
	class Stage2ToStage3State : public ObjState<StageSelectAnimation>
	{
		Stage2ToStage3State(){}
	public:

		static shared_ptr<Stage2ToStage3State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class Stage3ToStage2State : public ObjState<StageSelectAnimation>
	//	�p�r: �X�e�[�W3����X�e�[�W2�Z���N�g�A�j���[�V�����X�e�[�g.
	//--------------------------------------------------------------------------------------
	class Stage3ToStage2State : public ObjState<StageSelectAnimation>
	{
		Stage3ToStage2State(){}
	public:

		static shared_ptr<Stage3ToStage2State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class Stage2ToStage1State : public ObjState<StageSelectAnimation>
	//	�p�r: �X�e�[�W2����X�e�[�W1�Z���N�g�A�j���[�V�����X�e�[�g.
	//--------------------------------------------------------------------------------------
	class Stage2ToStage1State : public ObjState<StageSelectAnimation>
	{
		Stage2ToStage1State(){}
	public:

		static shared_ptr<Stage2ToStage1State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class NoiseStage1State : public ObjState<StageSelectAnimation>
	//	�p�r: �X�e�[�W1�m�C�Y�A�j���[�V�����X�e�[�g.
	//--------------------------------------------------------------------------------------
	class NoiseStage1State : public ObjState<StageSelectAnimation>
	{
		NoiseStage1State(){}
	public:

		static shared_ptr<NoiseStage1State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class NoiseStage2State : public ObjState<StageSelectAnimation>
	//	�p�r: �X�e�[�W2�m�C�Y�A�j���[�V�����X�e�[�g.
	//--------------------------------------------------------------------------------------
	class NoiseStage2State : public ObjState<StageSelectAnimation>
	{
		NoiseStage2State(){}
	public:

		static shared_ptr<NoiseStage2State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class NoiseStage3State : public ObjState<StageSelectAnimation>
	//	�p�r: �X�e�[�W3�m�C�Y�A�j���[�V�����X�e�[�g.
	//--------------------------------------------------------------------------------------
	class NoiseStage3State : public ObjState<StageSelectAnimation>
	{
		NoiseStage3State(){}
	public:

		static shared_ptr<NoiseStage3State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class DecideStage1State : public ObjState<StageSelectAnimation>
	//	�p�r: �X�e�[�W1����A�j���[�V�����X�e�[�g.
	//--------------------------------------------------------------------------------------
	class DecideStage1State : public ObjState<StageSelectAnimation>
	{
		DecideStage1State(){}
	public:

		static shared_ptr<DecideStage1State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class DecideStage2State : public ObjState<StageSelectAnimation>
	//	�p�r: �X�e�[�W2����A�j���[�V�����X�e�[�g.
	//--------------------------------------------------------------------------------------
	class DecideStage2State : public ObjState<StageSelectAnimation>
	{
		DecideStage2State(){}
	public:

		static shared_ptr<DecideStage2State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};


	//--------------------------------------------------------------------------------------
	//	class DecideStage3State : public ObjState<StageSelectAnimation>
	//	�p�r: �X�e�[�W3����A�j���[�V�����X�e�[�g.
	//--------------------------------------------------------------------------------------
	class DecideStage3State : public ObjState<StageSelectAnimation>
	{
		DecideStage3State(){}
	public:

		static shared_ptr<DecideStage3State> Instance();

		virtual void Enter(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Execute(const shared_ptr<StageSelectAnimation>& Obj)override;

		virtual void Exit(const shared_ptr<StageSelectAnimation>& Obj)override{}
	};
}
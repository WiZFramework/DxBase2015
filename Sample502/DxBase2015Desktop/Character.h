#pragma once

#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class JumpBase;
	//	�p�r: �W�����v�̃A���S���Y���̃x�[�X�N���X
	//--------------------------------------------------------------------------------------
	class JumpBase{
	protected:
		JumpBase(){}
	public:
		virtual ~JumpBase(){}
		virtual float JumpMethod() = 0;
	};

	//--------------------------------------------------------------------------------------
	//	class JumpShort : public JumpBase;
	//	�p�r: �Z���W�����v
	//--------------------------------------------------------------------------------------
	class JumpShort : public JumpBase{
	public:
		JumpShort() :JumpBase(){}
		virtual ~JumpShort(){}
		virtual float JumpMethod()override{
			return 2.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class JumpMiddle : public JumpBase;
	//	�p�r: �ʏ�̃W�����v
	//--------------------------------------------------------------------------------------
	class JumpMiddle : public JumpBase{
	public:
		JumpMiddle() :JumpBase(){}
		virtual ~JumpMiddle(){}
		virtual float JumpMethod()override{
			return 4.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class JumpLong : public JumpBase;
	//	�p�r: �����̃W�����v
	//--------------------------------------------------------------------------------------
	class JumpLong : public JumpBase{
	public:
		JumpLong() :JumpBase(){}
		virtual ~JumpLong(){}
		virtual float JumpMethod()override{
			return 6.0f;
		}
	};

	//--------------------------------------------------------------------------------------
	//	class JumpLongLong : public JumpBase;
	//	�p�r:�@���������W�����v
	//--------------------------------------------------------------------------------------
	class JumpLongLong : public JumpBase{
	public:
		JumpLongLong() :JumpBase(){}
		virtual ~JumpLongLong(){}
		virtual float JumpMethod()override{
			return 8.0f;
		}
	};



	//--------------------------------------------------------------------------------------
	//	class Box : public GameObject;
	//	�p�r: �{�b�N�X
	//--------------------------------------------------------------------------------------
	class Box : public GameObject{
		Vector3 m_StartPos;
		unique_ptr<JumpBase> m_JumpStrategy;
	public:
		//�\�z�Ɣj��
		Box(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~Box();
		//������
		virtual void Create() override;
		//�X�V
		virtual void Update() override;
	};

	//--------------------------------------------------------------------------------------
	//	class RollingTorus : public GameObject;
	//	�p�r: ��]����g�[���X
	//--------------------------------------------------------------------------------------
	class RollingTorus : public GameObject{
		Vector3 m_StartPos;
		float m_YRot;
		float m_RotationSpeed;
		unique_ptr<JumpBase> m_JumpStrategy;
	public:
		//�\�z�Ɣj��
		RollingTorus(const shared_ptr<Stage>& StagePtr, const Vector3& StartPos);
		virtual ~RollingTorus();
		//������
		virtual void Create() override;
		//�X�V
		virtual void Update() override;
	};



}
//endof  basedx11

#pragma once
#include "stdafx.h"

namespace basedx11{

	struct Event;
	class App;

	//--------------------------------------------------------------------------------------
	//	class ShapeInterface;
	//	�p�r: Update��Draw�̃C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	class ShapeInterface{
	public:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		//	ShapeInterface();
		/*!
		@breaf �R���X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		ShapeInterface(){}
		//--------------------------------------------------------------------------------------
		//	virtual ~ShapeInterface();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~ShapeInterface(){}
		//����
		//--------------------------------------------------------------------------------------
		//	virtual void Update() = 0;
		/*!
		@breaf Update����
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Update() = 0;
		//--------------------------------------------------------------------------------------
		//	virtual void Update2();
		/*!
		@breaf Update2����
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Update2() = 0;
		//--------------------------------------------------------------------------------------
		//	virtual void Update3();
		/*!
		@breaf Update3����.
		���̊֐��̂݃f�t�H���g��`���Ă���
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Update3(){}
		//--------------------------------------------------------------------------------------
		//	virtual void Draw() = 0;
		/*!
		@breaf Draw����
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual void Draw() = 0;
	};



	//--------------------------------------------------------------------------------------
	//	class Object;
	//	�p�r: ���ׂẴI�u�W�F�N�g�̊��N���X�i���ۃN���X�j
	//--------------------------------------------------------------------------------------
	class Object : public std::enable_shared_from_this<Object>{
	protected:
		//�\�z�Ɣj��
		Object(){}
		virtual ~Object(){}
	public:
		//this�|�C���^�擾
		template<typename T>
		std::shared_ptr<T> GetThis(){
			auto Ptr = dynamic_pointer_cast<T>(shared_from_this());
			if (Ptr){
				return Ptr;
			}
			else{
				throw BaseException(
					L"this��T�^�ɃL���X�g�ł��܂���",
					L"if( ! dynamic_pointer_cast<T>(shared_from_this()) )",
					L"Object::GetThis()"
					);
			}
		}
		//�I�u�W�F�N�g�쐬�istatic�֐��j
		template<typename T, typename... Ts>
		static shared_ptr<T> CreateObject(Ts&&... params){
			shared_ptr<T> Ptr = shared_ptr<T>(new T(params...));
			//���z�֐��Ăяo��
			Ptr->PreCreate();
			Ptr->Create();
			return Ptr;
		}

		//���������s���i���z�֐��j
		//��this�|�C���^���K�v�ȃI�u�W�F�N�g�͂��̊֐��𑽏d��`���āA�\�z����
		virtual void PreCreate(){}
		virtual void Create(){}


		// �C�x���g��POST�i�L���[�ɓ����j
		void PostEvent(float DispatchTime, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());
		//�w��̃_�C�i�~�b�N�N���X�ɑ���
		template<typename T>
		void PostEvent(float DispatchTime, const shared_ptr<Object>& Sender, const wstring& MsgStr,
			shared_ptr<void>& Info = shared_ptr<void>()){
			auto vec = App::GetApp()->GetSceneBase()->GetActiveStage()->GetGameObjectVec();
			for (auto Ptr : vec){
				auto TgtPtr = dynamic_pointer_cast<T>(Ptr);
				if (TgtPtr){
					PostEvent(DispatchTime, Sender, TgtPtr, MsgStr, Info);
				}
			}
		}
		// �C�x���g��SEND�i�L���[�ɓ��ꂸ�ɂ��̂܂ܑ���j
		void SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());
		//�w��̃_�C�i�~�b�N�N���X�ɑ���
		template<typename T>
		void SendEvent(const shared_ptr<Object>& Sender, const wstring& MsgStr,
			shared_ptr<void>& Info = shared_ptr<void>()){
			auto vec = App::GetApp()->GetSceneBase()->GetActiveStage()->GetGameObjectVec();
			for (auto Ptr : vec){
				auto TgtPtr = dynamic_pointer_cast<T>(Ptr);
				if (TgtPtr){
					SendEvent(Sender, TgtPtr, MsgStr, Info);
				}
			}
		}

		//�C�x���g���󂯎��i���z�֐��j
		//���I�u�W�F�N�g�̓C�x���g���󂯎��ꍇ�ɂ��̊֐��𑽏d��`����
		virtual void OnEvent(const shared_ptr<Event>& event){}
	private:
		//�R�s�[�֎~
		Object(const Object&) = delete;
		Object& operator=(const Object&) = delete;
		//���[�u�֎~
		Object(const Object&&) = delete;
		Object& operator=(const Object&&) = delete;

	};

	//--------------------------------------------------------------------------------------
	//	struct Event;
	//	�p�r: �C�x���g�\����
	//--------------------------------------------------------------------------------------
	struct Event{
		//�x�����ԁiSendEvent�̏ꍇ�͏��0�j
		float m_DispatchTime;
		//���̃��b�Z�[�W�𑗂����I�u�W�F�N�g
		weak_ptr<Object> m_Sender;
		//�󂯎��I�u�W�F�N�g�inullptr�̏ꍇ�̓A�N�e�B�u�X�e�[�W�����ׂĂ������̓L�[���[�h�Ŏ��ʂ���I�u�W�F�N�g�j
		weak_ptr<Object> m_Receiver;
		//�󂯎��I�u�W�F�N�g�̎Q�Ɩ��������͔z��
		wstring m_ReceiverKey;
		//���b�Z�[�W������
		wstring m_MsgStr;
		//�ǉ��������I�u�W�F�N�g�̃|�C���^
		shared_ptr<void> m_Info;
		//�\�z�Ɣj��
		Event(float DispatchTime,const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>()) :
			m_DispatchTime(DispatchTime),
			m_Sender(Sender),
			m_Receiver(Receiver),
			m_ReceiverKey(L""),
			m_MsgStr(MsgStr),
			m_Info(Info)
		{}
		Event(float DispatchTime, const shared_ptr<Object>& Sender, const wstring& ReceiverKey,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>()) :
			m_DispatchTime(DispatchTime),
			m_Sender(Sender),
			m_Receiver(),
			m_ReceiverKey(ReceiverKey),
			m_MsgStr(MsgStr),
			m_Info(Info)
		{}
		~Event(){}
	};

	class Stage;
	class SceneBase;

	//--------------------------------------------------------------------------------------
	//	class EventDispatcher;
	//	�p�r: �C�x���g�z���N���X
	//--------------------------------------------------------------------------------------
	class EventDispatcher{
	public:
		//�\�z�Ɣj��
		explicit EventDispatcher(const shared_ptr<SceneBase>& SceneBasePtr);
		virtual ~EventDispatcher();

		//�C�x���g��POST�i�L���[�ɓ����j
		void PostEvent(float Delay, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());
		//�C�x���g��SEND�i�L���[�ɓ��ꂸ�ɂ��̂܂ܑ���j
		void SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
			const wstring& MsgStr, shared_ptr<void>& Info = shared_ptr<void>());

		//--------------------------------------------------------------------------------------
		//	void DispatchDelayedEvwnt();
		//	�p�r: POST�C�x���g�̑��M(���C�����[�v�ŌĂ΂��)
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void DispatchDelayedEvwnt();

		//--------------------------------------------------------------------------------------
		//	void ClearEventQ();
		//	�p�r:�L���[�ɂ��܂��Ă��郁�b�Z�[�W���폜����
		//	�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void ClearEventQ();

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
		//�R�s�[�֎~
		EventDispatcher(const EventDispatcher&) = delete;
		EventDispatcher& operator=(const EventDispatcher&) = delete;
		//���[�u�֎~
		EventDispatcher(const EventDispatcher&&) = delete;
		EventDispatcher& operator=(const EventDispatcher&&) = delete;
	};

}
//end basedx11

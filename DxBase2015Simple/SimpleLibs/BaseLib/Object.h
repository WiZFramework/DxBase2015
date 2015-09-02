#pragma once
#include "stdafx.h"

namespace basedx11{

	class App;
	struct Event;

	//--------------------------------------------------------------------------------------
	//	class SimpleInterface;
	//	�p�r: Simple��Update��Draw�̃C���^�[�t�F�C�X
	//--------------------------------------------------------------------------------------
	class SimpleInterface{
	protected:
		//�\�z�Ɣj��
		//--------------------------------------------------------------------------------------
		//	SimpleInterface();
		/*!
		@breaf �R���X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		SimpleInterface(){}
		//--------------------------------------------------------------------------------------
		//	virtual ~SimpleInterface();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~SimpleInterface(){}
	public:
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
		//�N���G�C�g�ς݂��ǂ���
		//Create�֐����Ăяo�����true�ɂȂ�
		bool m_Created;
		void SetCreated(bool b){
			m_Created = b;
		}
	protected:
		//�\�z�Ɣj��
		Object():m_Created(false){}
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
			Ptr->SetCreated(true);
			return Ptr;
		}

		//���������s���i���z�֐��j
		//��this�|�C���^���K�v�ȃI�u�W�F�N�g�͂��̊֐��𑽏d��`���āA�\�z����
		virtual void PreCreate(){}
		virtual void Create(){}
		//�N���G�C�g�ς݂��ǂ���
		bool IsCreated(){
			return m_Created;
		}
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
		Event(float DispatchTime, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
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

	//--------------------------------------------------------------------------------------
	//	class EventDispatcher;
	//	�p�r: �C�x���g�z���N���X
	//--------------------------------------------------------------------------------------
	class EventDispatcher{
	public:
		//�\�z�Ɣj��
		explicit EventDispatcher();
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



	//--------------------------------------------------------------------------------------
	//	class StageBase : public Object, public SimpleInterface;
	//	�p�r: �X�e�[�W�̐e�N���X
	//--------------------------------------------------------------------------------------
	class StageBase : public Object, public SimpleInterface{
	protected:
		StageBase() :Object(), SimpleInterface(){}
		virtual ~StageBase(){}
	};


	//--------------------------------------------------------------------------------------
	//	class Camera : public Object, public SimpleInterface ;
	//	�p�r: �J�����i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	class Camera : public Object, public SimpleInterface{
	public:
		//�\�z�Ɣj��
		explicit Camera();
		virtual ~Camera();
		//�A�N�Z�T
		const Vector3& GetEye() const;
		void SetEye(const Vector3& Eye);
		void SetEye(float x, float y, float z);

		const Vector3& GetAt() const;
		void SetAt(const Vector3& At);
		void SetAt(float x, float y, float z);

		const Vector3& GetUp() const;
		void SetUp(const Vector3& Up);
		void SetUp(float x, float y, float z);

		bool IsPers()const;
		bool GetPers()const;
		void SetPers(bool b);

		float GetFovY() const;
		void SetFovY(float f);

		float GetAspect() const;
		void SetAspect(float f);

		float GetWidth() const;
		void SetWidth(float f);

		float GetHeight() const;
		void SetHeight(float f);

		float GetNear() const;
		void SetNear(float f);

		float GetFar() const;
		void SetFar(float f);

		void SetViewPort(const D3D11_VIEWPORT& v);

		const Matrix4X4& GetViewMatrix() const;
		const Matrix4X4& GetProjMatrix() const;
		//����
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class Light : public Object, public SimpleInterface ;
	//	�p�r: ���C�g�i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	class Light : public Object, public SimpleInterface{
	public:
		explicit Light();
		Light(
			const Vector3& Directional,
			const Color4& DiffuseColor,
			const Color4& SpecularColor
			);

		virtual ~Light();
		//�A�N�Z�T
		const Vector3& GetDirectional() const;
		void SetDirectional(const Vector3& Directional);
		void SetDirectional(float x, float y, float z);
		void SetPositionToDirectional(const Vector3& Position);
		void SetPositionToDirectional(float x, float y, float z);

		const Color4& GetDiffuseColor()const;
		void SetDiffuseColor(const Color4& col);
		void SetDiffuseColor(float r, float g, float b, float a);

		const Color4& GetSpecularColor() const;
		void SetSpecularColor(const Color4& col);
		void SetSpecularColor(float r, float g, float b, float a);

		virtual void Update()override{}
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class MultiLight : public Object, public SimpleInterface ;
	//	�p�r: �}���`���C�g�i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	class MultiLight : public Object, public SimpleInterface{
	public:
		//�\�z�Ɣj��
		explicit MultiLight();
		virtual ~MultiLight();
		//�A�N�Z�T
		//Light�̐�
		size_t GetLightCount() const;
		//Light�̎擾
		shared_ptr<Light> GetLight(size_t Index)const;
		//Light�̃Z�b�g
		void SetLight(size_t Index, shared_ptr<Light>& Ptr);
		//�z��̎Q��
		vector< shared_ptr<Light> >& GetLightVec();
		//Light�̒ǉ�
		shared_ptr<Light> AddLight();
		//Light�̍폜
		void RemoveLight(size_t Index);
		//�f�t�H���g�̃��C�e�B���O�̐ݒ�
		void SetDefaultLighting();

		//����
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class View: public Object, public SimpleInterface;
	//	�p�r: �r���[�N���X�i�r���[�|�[�g�ƃ��C�g�A�J�������Ǘ��A�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	class View : public Object, public SimpleInterface{
		//�e���v���[�g����Ă΂��T�u�֐�
		void ResetParamatersSub(const D3D11_VIEWPORT& vp, const Color4& Col,
		const shared_ptr<Camera>& CameraPtr, const shared_ptr<MultiLight>& MultiLightPtr);
	public:
		//�\�z�Ɣj��
		View();
		virtual ~View();
		//�A�N�Z�T
		const D3D11_VIEWPORT& GetViewPort()const;
		const D3D11_VIEWPORT* GetViewPortPtr()const;
		void SetViewPort(const D3D11_VIEWPORT& v);
		const Color4& GetBackColor() const;
		void SetBackColor(const Color4& c);
		shared_ptr<Camera> GetCamera() const;
		void SetCamera(shared_ptr<Camera>& Ptr);

		shared_ptr<MultiLight> GetMultiLight() const;
		void SetMultiLight(shared_ptr<MultiLight>& Lights);

		shared_ptr<MultiLight> SetDefaultLighting();


		//����
		shared_ptr<Light> AddLight();

		template<typename CameraType, typename LightType>
		void ResetParamaters(const Rect2D<float>& ViewPortSize, Color4& Col, size_t LightCount, float MinDepth = 0.0f, float MaxDepth = 1.0f){
			D3D11_VIEWPORT vp;
			ZeroMemory(&vp, sizeof(vp));
			vp.MinDepth = MinDepth;
			vp.MaxDepth = MaxDepth;
			vp.TopLeftX = ViewPortSize.left;
			vp.TopLeftY = ViewPortSize.top;
			vp.Width = ViewPortSize.Width();
			vp.Height = ViewPortSize.Height();
			//�J�����������ւ���
			auto CameraPtr = Object::CreateObject<CameraType>();
			//���C�g�������ւ���
			auto MultiLightPtr = Object::CreateObject<LightType>();
			for (size_t i = 0; i < LightCount; i++){
				MultiLightPtr->AddLight();
			}
			//�T�u�֐����g���Đݒ�
			ResetParamatersSub(vp, Col,
				dynamic_pointer_cast<Camera>(CameraPtr),
				dynamic_pointer_cast<MultiLight>(MultiLightPtr));
		}
		//����
		virtual void Update()override;
		virtual void Draw()override{}
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Object, public SimpleInterface;
	//	�p�r: StringSprite
	//	������\���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	class StringSprite : public Object, public SimpleInterface{
	public:
		explicit StringSprite();
		virtual ~StringSprite();
		//�A�N�Z�T
		enum TextAlignment{
			m_Left,		//�����킹
			m_Center,	//�������킹
			m_Right		//�E���킹
		};
		//�A�N�Z�T
		bool GetKeyInput() const;
		bool IsKeyInput() const;
		void SetKeyInput(bool b);


		void SetFont(const wstring& FontName, float FontSize);

		StringSprite::TextAlignment GetTextAlignment() const;
		void SetTextAlignment(StringSprite::TextAlignment Alignment);
		const wstring& GetText() const;
		void SetText(const wstring& str);
		void AddText(const wstring& str);
		void InsertText(const wstring& str);
		void InsertText(const wstring& str, size_t CaretIndex);


		const Color4& GetFontColor() const;
		void SetFontColor(const Color4& Col);
		const wstring& GetFontName() const;
		float GetFontSize() const;

		const Color4& GetBackColor() const;
		void SetBackColor(const Color4& Col);

		Point2D<float> GetBackTextMargin() const;
		void SetBackTextMargin(Point2D<float> p);

		const Color4& GetCaretColor() const;
		void SetCaretColor(const Color4& Col);

		float GetCaretSpeed() const;
		void SetCaretSpeed(float f);



		float GetTextBlockWidth() const;
		void SetTextBlockWidth(float f);
		float GetTextBlockHeight() const;
		void SetTextBlockHeight(float f);

		ComPtr<IDWriteTextLayout>& GetTextLayout()const;
		ComPtr<IDWriteTextFormat>&	GetTextFormat()const;
		const DWRITE_TEXT_METRICS& GetDriteTextMetrics() const;

		const Point2D<float>& GetStartPosition() const;
		void SetStartPosition(const Point2D<float>& pos);

		Rect2D<float> GetTextRect() const;
		void SetTextRect(const Rect2D<float>& rect);

		//����
		virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
		virtual void OnChar(WPARAM wParam, LPARAM lParam);
		virtual void Update()override;
		virtual void Draw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class AudioResource : public Object;
	/*!
	�I�[�f�B�I���\�[�X�iwav�j�N���X
	*/
	//--------------------------------------------------------------------------------------
	class AudioResource : public Object{
	public:
		//--------------------------------------------------------------------------------------
		//	AudioResource(
		//	const wstring& FileName	//�t�@�C����
		//	);
		/*!
		@breaf �R���X�g���N�^<br />
		�E�F�u�t�@�C������n��
		@param const wstring& FileName	�t�@�C����
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		AudioResource(const wstring& FileName);
		//--------------------------------------------------------------------------------------
		//	virtual ~AudioResource();
		/*!
		@breaf �f�X�g���N�^
		@param �Ȃ�
		@return	�Ȃ�
		*/
		//--------------------------------------------------------------------------------------
		virtual ~AudioResource();
		//�A�N�Z�T
		//--------------------------------------------------------------------------------------
		//	const vector<byte>& GetSoundData()const;
		/*!
		@breaf �T�E���h�f�[�^�̎擾
		@param �Ȃ�
		@return	���̃E�F�u���\�[�X�̃T�E���h�f�[�^
		*/
		//--------------------------------------------------------------------------------------
		const vector<byte>& GetSoundData()const;
		//--------------------------------------------------------------------------------------
		//	WAVEFORMATEX*  GetOutputWaveFormatEx()const;
		/*!
		@breaf �E�F�u�t�H�[�}�b�g�̎擾
		@param �Ȃ�
		@return	���̃E�F�u���\�[�X��WAVEFORMATEX�E�F�u�t�H�[�}�b�g�̃|�C���^
		*/
		//--------------------------------------------------------------------------------------
		WAVEFORMATEX*  GetOutputWaveFormatEx() const;

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



	//--------------------------------------------------------------------------------------
	//	class MultiAudioObject: public Object;
	//	�p�r: �}���`�I�[�f�B�I
	//--------------------------------------------------------------------------------------
	class MultiAudioObject : public Object{
	public:
		//�\�z�Ɣj��
		MultiAudioObject();
		virtual ~MultiAudioObject();
		//�A�N�Z�T
		shared_ptr<AudioResource> GetAudioResource(const wstring& ResKey, bool ExceptionActive = true) const;
		//����
		shared_ptr<AudioResource> AddAudioResource(const wstring& FileName,const wstring& ResKey);
		void RemoveAudioResource(const wstring& ResKey);
		void Start(const wstring& ResKey, const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Start(const wstring& ResKey, size_t LoopCount = 0, float Volume = 1.0f);
		void Stop(const wstring& ResKey);
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



}
//end basedx11

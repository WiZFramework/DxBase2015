
#include "stdafx.h"


namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Object;
	//	�p�r: ���ׂẴI�u�W�F�N�g�̊��N���X�i���ۃN���X�j
	//--------------------------------------------------------------------------------------
	void Object::PostEvent(float DispatchTime, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		App::GetApp()->GetEventDispatcher()->PostEvent(DispatchTime, Sender, Receiver, MsgStr, Info);
	}
	void Object::SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		App::GetApp()->GetEventDispatcher()->SendEvent(Sender, Receiver, MsgStr, Info);
	}

	//--------------------------------------------------------------------------------------
	//	struct EventDispatcher::Impl;
	//	�p�r: Impl�\����
	//--------------------------------------------------------------------------------------
	struct EventDispatcher::Impl{
		//�C�x���g�̃L���[
		list< shared_ptr<Event> > m_PriorityQ;
		//--------------------------------------------------------------------------------------
		//	void Discharge(
		//	const Event& event	//�C�x���g
		//	);
		//�p�r: �C�x���g�̑��M
		//�߂�l: �Ȃ�
		//--------------------------------------------------------------------------------------
		void Discharge(const shared_ptr<Event>& event);
		Impl(){}
		~Impl(){}
	};

	void EventDispatcher::Impl::Discharge(const shared_ptr<Event>& event){
		if (!event->m_Receiver.expired()){
			//�󂯎肪�L��
			auto RePtr = event->m_Receiver.lock();
			RePtr->OnEvent(event);
		}
	}



	//--------------------------------------------------------------------------------------
	//	class EventDispatcher;
	//	�p�r: �C�x���g�z���N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	EventDispatcher::EventDispatcher() :
		pImpl(new Impl())
	{}
	EventDispatcher::~EventDispatcher(){}


	//�C�x���g��POST�i�L���[�ɓ����j
	void EventDispatcher::PostEvent(float Delay, const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		//�C�x���g�̍쐬 
		auto Ptr = make_shared<Event>(Delay, Sender, Receiver, MsgStr, Info);
		//�L���[�ɂ��߂�
		pImpl->m_PriorityQ.push_back(Ptr);
	}

	//�C�x���g��SEND�i�L���[�ɓ��ꂸ�ɂ��̂܂ܑ���j
	void EventDispatcher::SendEvent(const shared_ptr<Object>& Sender, const shared_ptr<Object>& Receiver,
		const wstring& MsgStr, shared_ptr<void>& Info){
		//�C�x���g�̍쐬 
		auto Ptr = make_shared<Event>(0.0f, Sender, Receiver, MsgStr, Info);
		//���M
		pImpl->Discharge(Ptr);
	}

	void EventDispatcher::DispatchDelayedEvwnt(){
		//�O��̃^�[������̎���
		float ElapsedTime = App::GetApp()->GetElapsedTime();
		auto it = pImpl->m_PriorityQ.begin();
		while (it != pImpl->m_PriorityQ.end()){
			(*it)->m_DispatchTime -= ElapsedTime;
			if ((*it)->m_DispatchTime <= 0.0f){
				(*it)->m_DispatchTime = 0.0f;
				//���b�Z�[�W�̑��M
				pImpl->Discharge(*it);
				//�L���[����폜
				it = pImpl->m_PriorityQ.erase(it);
				//�폜��̃C�e���[�^���u�Ō�v��
				//�Ƃ��̓��[�v�𔲂���
				if (it == pImpl->m_PriorityQ.end()){
					break;
				}
			}
			else{
				it++;
			}
		}
	}

	void EventDispatcher::ClearEventQ(){
		pImpl->m_PriorityQ.clear();
	}




	//--------------------------------------------------------------------------------------
	//	struct Camera::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct Camera::Impl{
		Vector3 m_Eye;	//�J�����ʒu
		Vector3 m_At;	//���ړ_
		Vector3 m_Up;   //�J�����̌X���i�A�b�v�j
		bool m_Pers;	//���ߖ@���g�����ǂ���

		D3D11_VIEWPORT m_ViewPort;

		float m_FovY;   //�ˉe�p�x
		float m_Aspect;	//�A�X�y�N�g��

		float m_Width;	//���ߖ@���g��Ȃ��ꍇ�i���ˉe�j�̕�
		float m_Height;	//���ߖ@���g��Ȃ��ꍇ�i���ˉe�j�̍���
		float m_Near;	//��O�̍ŏ�����
		float m_Far;	//���̍ő勗��

		Matrix4X4 m_ViewMatrix;
		Matrix4X4 m_ProjMatrix;
		Impl() :
			m_Eye(0, 10.0f, -20.0f),	//�f�t�H���g�͌���΂ߏ�
			m_At(0, 0, 0),
			m_Up(0, 1.0f, 0),
			m_Pers(true),
			m_FovY(XM_PIDIV4),
			m_Aspect(0),
			m_Width(0),
			m_Height(0),
			m_Near(0.5f),
			m_Far(1000.0f),
			m_ViewMatrix(),
			m_ProjMatrix()
		{
			try{
				m_ViewPort.Height = (float)App::GetApp()->GetGameHeight();
				m_ViewPort.Width = (float)App::GetApp()->GetGameWidth();
				m_ViewPort.MinDepth = 0.0f;
				m_ViewPort.MaxDepth = 1.0f;
				m_ViewPort.TopLeftX = 0;
				m_ViewPort.TopLeftY = 0;

				m_Width = m_ViewPort.Width;
				m_Height = m_ViewPort.Height;
				m_Aspect = m_Width / m_Height;
			}
			catch (...){
				throw;
			}
		}
		~Impl(){}





	};

	//--------------------------------------------------------------------------------------
	//	class Camera : public Object, public SimpleInterface ;
	//	�p�r: �J�����i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	Camera::Camera()
		:Object(),
		SimpleInterface(),
		pImpl(new Impl())
	{

	}
	Camera::~Camera(){}

	//�A�N�Z�T
	const Vector3& Camera::GetEye() const{ return pImpl->m_Eye; }
	void Camera::SetEye(const Vector3& Eye){ pImpl->m_Eye = Eye; }
	void Camera::SetEye(float x, float y, float z){ pImpl->m_Eye = Vector3(x, y, z); }

	const Vector3& Camera::GetAt() const{ return pImpl->m_At; }
	void Camera::SetAt(const Vector3& At){ pImpl->m_At = At; }
	void Camera::SetAt(float x, float y, float z){ pImpl->m_At = Vector3(x, y, z); }

	const Vector3& Camera::GetUp() const{ return pImpl->m_Up; }
	void Camera::SetUp(const Vector3& Up){ pImpl->m_Up = Up; }
	void Camera::SetUp(float x, float y, float z){ pImpl->m_Up = Vector3(x, y, z); }

	bool Camera::IsPers()const{ return pImpl->m_Pers; }
	bool Camera::GetPers()const{ return pImpl->m_Pers; }
	void Camera::SetPers(bool b){ pImpl->m_Pers = b; }

	float Camera::GetFovY() const{ return pImpl->m_FovY; }
	void Camera::SetFovY(float f){ pImpl->m_FovY = f; }

	float Camera::GetAspect() const{ return pImpl->m_Aspect; }
	void Camera::SetAspect(float f){ pImpl->m_Aspect = f; }

	float Camera::GetWidth() const{ return pImpl->m_Width; }
	void Camera::SetWidth(float f){ pImpl->m_Width = f; }

	float Camera::GetHeight() const{ return pImpl->m_Height; }
	void Camera::SetHeight(float f){ pImpl->m_Height = f; }

	float Camera::GetNear() const{ return pImpl->m_Near; }
	void Camera::SetNear(float f){ pImpl->m_Near = f; }

	float Camera::GetFar() const{ return pImpl->m_Far; }
	void Camera::SetFar(float f){ pImpl->m_Far = f; }

	void Camera::SetViewPort(const D3D11_VIEWPORT& v){
		pImpl->m_ViewPort = v;
		pImpl->m_Width = pImpl->m_ViewPort.Width;
		pImpl->m_Height = pImpl->m_ViewPort.Height;
		pImpl->m_Aspect = pImpl->m_Width / pImpl->m_Height;
	}


	const Matrix4X4& Camera::GetViewMatrix() const{ return pImpl->m_ViewMatrix; }
	const Matrix4X4& Camera::GetProjMatrix() const{ return pImpl->m_ProjMatrix; }

	//����
	void Camera::Update(){
		pImpl->m_ViewMatrix.LookAtLH(pImpl->m_Eye, pImpl->m_At, pImpl->m_Up);
		if (pImpl->m_Pers){
			pImpl->m_ProjMatrix.PerspectiveFovLH(pImpl->m_FovY, pImpl->m_Aspect, pImpl->m_Near, pImpl->m_Far);
		}
		else{
			pImpl->m_ProjMatrix.OrthographicLH(pImpl->m_Width, pImpl->m_Height, pImpl->m_Near, pImpl->m_Far);
		}
	}


	//--------------------------------------------------------------------------------------
	//	struct Light::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct Light::Impl{
		Vector3 m_Directional;	//���C�g�̌���
		Color4 m_DiffuseColor;	//�f�B�t�B�[�Y�F
		Color4 m_SpecularColor;	//�X�y�L�����[�F
		Impl() :
			m_Directional(0, 0, 0),
			m_DiffuseColor(1.0000000f, 0.9607844f, 0.8078432f, 1.0f),
			m_SpecularColor(1.0000000f, 0.9607844f, 0.8078432f, 1.0f)
		{
		}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class Light : public Object, public SimpleInterface ;
	//	�p�r: ���C�g�i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	Light::Light() :
		Object(),
		SimpleInterface(),
		pImpl(new Impl())
	{
		SetPositionToDirectional(Vector3(-1.0f, 1.0f, -1.0f));
	}
	Light::Light(
		const Vector3& Directional,
		const Color4& DiffuseColor,
		const Color4& SpecularColor
		) :
		Object(),
		SimpleInterface(),
		pImpl(new Impl())
	{
		pImpl->m_Directional = Directional;
		pImpl->m_DiffuseColor = DiffuseColor;
		pImpl->m_SpecularColor = SpecularColor;
	}

	Light::~Light(){}

	//�A�N�Z�T
	const Vector3& Light::GetDirectional() const{
		return pImpl->m_Directional;
	}
	void Light::SetDirectional(const Vector3& Directional){
		pImpl->m_Directional = Directional;
		pImpl->m_Directional.Normalize();
	}
	void Light::SetDirectional(float x, float y, float z){
		pImpl->m_Directional = Vector3(x, y, z);
		pImpl->m_Directional.Normalize();
	}
	void Light::SetPositionToDirectional(const Vector3& Position){
		pImpl->m_Directional = Position;
		pImpl->m_Directional *= -1.0f;
		pImpl->m_Directional.Normalize();
	}
	void Light::SetPositionToDirectional(float x, float y, float z){
		pImpl->m_Directional = Vector3(x, y, z);
		pImpl->m_Directional *= -1.0f;
		pImpl->m_Directional.Normalize();
	}

	const Color4& Light::GetDiffuseColor() const{
		return pImpl->m_DiffuseColor;
	}
	void Light::SetDiffuseColor(const Color4& col){
		pImpl->m_DiffuseColor = col;
	}
	void Light::SetDiffuseColor(float r, float g, float b, float a){
		pImpl->m_DiffuseColor = Color4(r, g, b, a);
	}

	const Color4& Light::GetSpecularColor() const{
		return pImpl->m_SpecularColor;
	}
	void Light::SetSpecularColor(const Color4& col){
		pImpl->m_SpecularColor = col;
	}
	void Light::SetSpecularColor(float r, float g, float b, float a){
		pImpl->m_SpecularColor = Color4(r, g, b, a);
	}


	//--------------------------------------------------------------------------------------
	//	struct MultiLight::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct MultiLight::Impl{
		vector< shared_ptr<Light> > m_LightVec;
		Impl(){}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class MultiLight : public Object, public SimpleInterface ;
	//	�p�r: �}���`���C�g�i�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiLight::MultiLight() :
		Object(),
		SimpleInterface(),
		pImpl(new Impl())
	{
	}
	MultiLight::~MultiLight(){
	}
	//Light�̐�
	size_t MultiLight::GetLightCount() const{
		return pImpl->m_LightVec.size();
	}
	//Light�̎擾
	shared_ptr<Light> MultiLight::GetLight(size_t Index)const{
		if (Index >= pImpl->m_LightVec.size()){
			wstring str = L"if (";
			str += Util::UintToWStr(Index);
			str += L" >= pImpl->m_LightVec.size())";
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				str,
				L"MultiLight::GetLight()"
				);
		}
		return pImpl->m_LightVec[Index];
	}
	//Light�̃Z�b�g
	void MultiLight::SetLight(size_t Index, shared_ptr<Light>& Ptr){
		if (Index >= pImpl->m_LightVec.size()){
			wstring str = L"if (";
			str += Util::UintToWStr(Index);
			str += L" >= pImpl->m_LightVec.size())";
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				str,
				L"MultiLight::SetLight()"
				);
		}
		pImpl->m_LightVec[Index] = Ptr;
	}
	//�z��̎Q��
	vector< shared_ptr<Light> >& MultiLight::GetLightVec(){
		return pImpl->m_LightVec;
	}
	//Light�̒ǉ�
	shared_ptr<Light> MultiLight::AddLight(){
		auto Ptr = Object::CreateObject<Light>();
		pImpl->m_LightVec.push_back(Ptr);
		return Ptr;
	}
	//Light�̍폜
	void MultiLight::RemoveLight(size_t Index){
		if (Index >= pImpl->m_LightVec.size()){
			wstring str = L"if (";
			str += Util::UintToWStr(Index);
			str += L" >= pImpl->m_LightVec.size())";
			throw BaseException(
				L"�C���f�b�N�X���͈͊O�ł�",
				str,
				L"MultiLight::RemoveLight()"
				);
		}
		vector< shared_ptr<Light> >::iterator it;
		size_t count = 0;
		for (it = pImpl->m_LightVec.begin(); it != pImpl->m_LightVec.end(); it++){
			if (count == Index){
				pImpl->m_LightVec.erase(it);
				return;
			}
			count++;
		}
	}

	//�f�t�H���g�̃��C�e�B���O�̐ݒ�
	void MultiLight::SetDefaultLighting(){
		size_t NouCount = pImpl->m_LightVec.size();
		pImpl->m_LightVec.clear();
		//�Œ��̓��C�g��u��
		auto Ptr = Object::CreateObject<Light>(
			Vector3(0.5265408f, -0.5735765f, 0.6275069f),
			Color4(1.0000000f, 0.9607844f, 0.8078432f, 1.0f),
			Color4(1.0000000f, 0.9607844f, 0.8078432f, 1.0f)
			);
		pImpl->m_LightVec.push_back(Ptr);
		if (NouCount >= 2){

			Ptr = Object::CreateObject<Light>(
				Vector3(-0.7198464f, 0.3420201f, -0.6040227f),
				Color4(0.9647059f, 0.7607844f, 0.4078432f, 1.0f),
				Color4(0.0000000f, 0.0000000f, 0.0000000f, 0.0f)
				);
			pImpl->m_LightVec.push_back(Ptr);
		}
		if (NouCount >= 3){
			Ptr = Object::CreateObject<Light>(
				Vector3(0.4545195f, -0.7660444f, 0.4545195f),
				Color4(0.3231373f, 0.3607844f, 0.3937255f, 1.0f),
				Color4(0.3231373f, 0.3607844f, 0.3937255f, 1.0f)
				);
			pImpl->m_LightVec.push_back(Ptr);
		}
	}


	//����
	void MultiLight::Update(){
		for (auto Ptr : pImpl->m_LightVec){
			Ptr->Update();
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct View::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct View::Impl{
		D3D11_VIEWPORT m_ViewPort;	//�r���[�|�[�g
		Color4 m_BackColor;			//�w�i�F
		shared_ptr<Camera> m_Camera;	//�J����
		shared_ptr<MultiLight> m_MultiLight;	//���C�g
		explicit Impl() :
			m_BackColor(0.0f, 0.125f, 0.3f, 1.0f)
		{
			//�r���[�|�[�g�̃Z�b�g�A�b�v(�N���C�A���g�̈�S��)
			ZeroMemory(&m_ViewPort, sizeof(m_ViewPort));
			m_ViewPort.Width = (float)App::GetApp()->GetGameWidth();
			m_ViewPort.Height = (float)App::GetApp()->GetGameHeight();
			m_ViewPort.MinDepth = 0.0f;
			m_ViewPort.MaxDepth = 1.0f;
			m_ViewPort.TopLeftX = 0;
			m_ViewPort.TopLeftY = 0;
			//�J�������쐬���Ă���
			m_Camera = make_shared<Camera>();
			//���C�g���쐬���Ă���
			m_MultiLight = make_shared<MultiLight>();
		}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class View: public Object, public SimpleInterface;
	//	�p�r: �r���[�N���X�i�r���[�|�[�g�ƃ��C�g�A�J�������Ǘ��A�R���|�[�l���g�ł͂Ȃ��j
	//--------------------------------------------------------------------------------------
	//�e���v���[�g����Ă΂��T�u�֐�
	void View::ResetParamatersSub(const D3D11_VIEWPORT& vp, const Color4& Col,
		const shared_ptr<Camera>& CameraPtr, const shared_ptr<MultiLight>& MultiLightPtr){
		pImpl->m_ViewPort = vp;
		pImpl->m_BackColor = Col;
		pImpl->m_Camera = CameraPtr;
		pImpl->m_Camera->SetViewPort(vp);
		pImpl->m_MultiLight = MultiLightPtr;
	}

	//�\�z�Ɣj��
	View::View() :
		Object(),
		SimpleInterface(),
		pImpl(new Impl())
	{
	}
	View::~View(){}

	//�A�N�Z�T
	const D3D11_VIEWPORT& View::GetViewPort()const {
		return pImpl->m_ViewPort;
	}
	const D3D11_VIEWPORT* View::GetViewPortPtr()const {
		return &pImpl->m_ViewPort;
	}
	void View::SetViewPort(const D3D11_VIEWPORT& v){
		pImpl->m_ViewPort = v;
	}
	const Color4& View::GetBackColor() const {
		return pImpl->m_BackColor;
	}
	void View::SetBackColor(const Color4& c){
		pImpl->m_BackColor = c;
	}
	shared_ptr<Camera> View::GetCamera() const {
		return pImpl->m_Camera;
	}
	void View::SetCamera(shared_ptr<Camera>& Ptr){
		pImpl->m_Camera = Ptr;
	}

	shared_ptr<MultiLight> View::GetMultiLight() const {
		return pImpl->m_MultiLight;
	}
	void View::SetMultiLight(shared_ptr<MultiLight>& Lights){
		pImpl->m_MultiLight = Lights;
	}

	shared_ptr<MultiLight> View::SetDefaultLighting(){
		pImpl->m_MultiLight->SetDefaultLighting();
		return pImpl->m_MultiLight;
	}

	shared_ptr<Light> View::AddLight(){
		return pImpl->m_MultiLight->AddLight();
	}



	//����
	void View::Update(){
		if (pImpl->m_Camera){
			pImpl->m_Camera->Update();
		}
		if (pImpl->m_MultiLight){
			pImpl->m_MultiLight->Update();
		}
	}

	//--------------------------------------------------------------------------------------
	//	struct StringSprite::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct StringSprite::Impl{
		// �e�L�X�g �����_�����O�Ɋ֘A���郊�\�[�X�B
		wstring m_text;
		float m_FoneSize;
		float m_TextBlockWidth;
		float m_TextBlockHeight;
		wstring m_FontName;
		Color4 m_FontColor;
		Point2D<float> m_StartPosition;
		StringSprite::TextAlignment m_TextAlignment;
		bool m_KeyInput;	//�L�[���͂��邩�ǂ���
		size_t m_Caret;	//�L�����b�g�ʒu
		float m_CaretCounter;	//�L�����b�g�̃J�E���^
		float m_CaretSpeed;		//�L�����b�g�̓_�ő��x(0�œ_�ł��Ȃ�)
		bool m_CaretState;		//�L�����b�g��`�悷�邩�ǂ����i�_�ł̕\����ԁj
		Color4 m_CaretColor;	//�L�����b�g�̐F
		Color4 m_BackColor;		//�w�i�F
		Point2D<float> m_BackTextMargin;	//�w�i�F��h��Ԃ��e�L�X�g�̃}�[�W��(���E�Ə㉺)



		DWRITE_TEXT_METRICS m_textMetrics;
		ComPtr<ID2D1SolidColorBrush>    m_Brush;
		ComPtr<ID2D1DrawingStateBlock>  m_stateBlock;
		ComPtr<IDWriteTextLayout>       m_textLayout;
		ComPtr<IDWriteTextFormat>		m_textFormat;

		ComPtr<ID2D1SolidColorBrush>    m_CaretBrush;

		ComPtr<ID2D1SolidColorBrush>    m_BackBrush;


		Impl() :
			m_text(),
			m_FoneSize(16.0f),
			m_TextBlockWidth(128.0f),
			m_TextBlockHeight(32.0f),
			m_FontName(L"�l�r�S�V�b�N"),
			m_FontColor(Color4(1.0f, 1.0f, 1.0f, 1.0f)),
			m_StartPosition{ 16.0f, 16.0f },
			m_TextAlignment(StringSprite::TextAlignment::m_Left),
			m_KeyInput(false),
			m_Caret(0),
			m_CaretCounter(0),
			m_CaretSpeed(0.5f),
			m_CaretState(true),
			m_CaretColor(Color4(1.0f, 1.0f, 1.0f, 1.0f)),
			m_BackColor(Color4(0.0f, 0.0f, 0.0f, 0.0f)),
			m_BackTextMargin(4.0f, 0.0f)
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Object, public SimpleInterface;
	//	�p�r: StringSprite
	//	������\���I�u�W�F�N�g
	//--------------------------------------------------------------------------------------
	StringSprite::StringSprite() :
		SimpleInterface(),
		pImpl(new Impl()){
		try{
			ZeroMemory(&pImpl->m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

			// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
			auto Dev = App::GetApp()->GetDeviceResources();
			auto D2DFactory = Dev->GetD2DFactory();
			auto DWriteFactory = Dev->GetDWriteFactory();
			auto D2DDeviceContext = Dev->GetD2DDeviceContext();

			ThrowIfFailed(
				DWriteFactory->CreateTextFormat(
				pImpl->m_FontName.c_str(),
				nullptr,
				DWRITE_FONT_WEIGHT_LIGHT,
				DWRITE_FONT_STYLE_NORMAL,
				DWRITE_FONT_STRETCH_NORMAL,
				pImpl->m_FoneSize,
				L"ja",
				&pImpl->m_textFormat
				),
				L"�t�H���g�쐬�Ɏ��s���܂����B",
				L"DWriteFactory->CreateTextFormat()",
				L"StringSprite::StringSprite()"
				);

			ThrowIfFailed(
				pImpl->m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
				L"�t�H���g�A���C�������g�ݒ�Ɏ��s���܂����B",
				L"DWriteFactory->CreateTextFormat()",
				L"StringSprite::StringSprite()"
				);

			ThrowIfFailed(
				D2DFactory->CreateDrawingStateBlock(&pImpl->m_stateBlock),
				L"�t�H���g�X�e�[�g�u���b�N�ݒ�Ɏ��s���܂����B",
				L"D2DFactory->CreateDrawingStateBlock()",
				L"StringSprite::StringSprite()"
				);

			auto Col = D2D1::ColorF(pImpl->m_FontColor.x, pImpl->m_FontColor.y, pImpl->m_FontColor.z, pImpl->m_FontColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
				Col,
				&pImpl->m_Brush
				),
				L"�t�H���g�u���V�ݒ�Ɏ��s���܂����B",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringSprite::StringSprite()"
				);

			Col = D2D1::ColorF(pImpl->m_CaretColor.x, pImpl->m_CaretColor.y, pImpl->m_CaretColor.z, pImpl->m_CaretColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
				Col,
				&pImpl->m_CaretBrush
				),
				L"�L�����b�g�u���V�ݒ�Ɏ��s���܂����B",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringSprite::StringSprite()"
				);

			Col = D2D1::ColorF(pImpl->m_BackColor.x, pImpl->m_BackColor.y, pImpl->m_BackColor.z, pImpl->m_BackColor.w);

			ThrowIfFailed(
				D2DDeviceContext->CreateSolidColorBrush(
				Col,
				&pImpl->m_BackBrush
				),
				L"�o�b�N�O���E���h�u���V�ݒ�Ɏ��s���܂����B",
				L"D2DDeviceContext->CreateSolidColorBrush()",
				L"StringSprite::StringSprite()"
				);


		}
		catch (...){
			throw;
		}
	}
	StringSprite::~StringSprite(){}
	//�A�N�Z�T
	void StringSprite::SetFont(const wstring& FontName, float FontSize){
		ZeroMemory(&pImpl->m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DFactory = Dev->GetD2DFactory();
		auto DWriteFactory = Dev->GetDWriteFactory();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();

		ThrowIfFailed(
			DWriteFactory->CreateTextFormat(
			FontName.c_str(),
			nullptr,
			DWRITE_FONT_WEIGHT_LIGHT,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			FontSize,
			L"ja",
			&pImpl->m_textFormat
			),
			L"�t�H���g�쐬�Ɏ��s���܂����B",
			L"DWriteFactory->CreateTextFormat()",
			L"StringSprite::SetFont()"
			);
		//�t�H���g�̍쐬�ɐ��������̂Œl��ݒ�
		pImpl->m_FontName = FontName;
		pImpl->m_FoneSize = FontSize;

		ThrowIfFailed(
			pImpl->m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
			L"�t�H���g�A���C�������g�ݒ�Ɏ��s���܂����B",
			L"DWriteFactory->CreateTextFormat()",
			L"StringSprite::SetFont()"
			);

		ThrowIfFailed(
			D2DFactory->CreateDrawingStateBlock(&pImpl->m_stateBlock),
			L"�t�H���g�X�e�[�g�u���b�N�ݒ�Ɏ��s���܂����B",
			L"D2DFactory->CreateDrawingStateBlock()",
			L"StringSprite::SetFont()"
			);


	}
	StringSprite::TextAlignment StringSprite::GetTextAlignment() const{
		return pImpl->m_TextAlignment;
	}
	void StringSprite::SetTextAlignment(StringSprite::TextAlignment Alignment){
		pImpl->m_TextAlignment = Alignment;
	}


	const wstring& StringSprite::GetText() const{
		return pImpl->m_text;
	}
	void StringSprite::SetText(const wstring& str){
		pImpl->m_text = str;
	}


	//�Ō���ɒǉ�
	void StringSprite::AddText(const wstring& str){
		wstring TempText = GetText();
		TempText += str;
		if (IsKeyInput()){
			pImpl->m_Caret = TempText.size();
		}
		SetText(TempText);
	}
	void StringSprite::InsertText(const wstring& str){
		if (!IsKeyInput()){
			return;
		}
		wstring TempText = GetText();
		if (pImpl->m_Caret >= TempText.size()){
			pImpl->m_Caret = TempText.size();
			AddText(str);
		}
		else{
			TempText.insert(pImpl->m_Caret, str);
			pImpl->m_Caret += str.size();
			SetText(TempText);
		}
	}
	void StringSprite::InsertText(const wstring& str, size_t CaretIndex){
		if (!IsKeyInput()){
			return;
		}
		pImpl->m_Caret = CaretIndex;
		InsertText(str);
	}


	const Color4& StringSprite::GetFontColor() const{
		return pImpl->m_FontColor;
	}
	void StringSprite::SetFontColor(const Color4& Col){
		pImpl->m_FontColor = Col;
		auto ColBrush = D2D1::ColorF(pImpl->m_FontColor.x, pImpl->m_FontColor.y, pImpl->m_FontColor.z, pImpl->m_FontColor.w);
		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
			ColBrush,
			&pImpl->m_Brush
			),
			L"�t�H���g�u���V�ݒ�Ɏ��s���܂����B",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"StringSprite::SetFontColor()"
			);

	}

	const Color4& StringSprite::GetBackColor() const{
		return pImpl->m_BackColor;
	}
	void StringSprite::SetBackColor(const Color4& Col){
		pImpl->m_BackColor = Col;
		auto ColBrush = D2D1::ColorF(pImpl->m_BackColor.x, pImpl->m_BackColor.y, pImpl->m_BackColor.z, pImpl->m_BackColor.w);
		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
			ColBrush,
			&pImpl->m_BackBrush
			),
			L"�o�b�N�u���V�ݒ�Ɏ��s���܂����B",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"StringSprite::SetCaretColor()"
			);
	}

	Point2D<float> StringSprite::GetBackTextMargin() const{
		return pImpl->m_BackTextMargin;
	}
	void StringSprite::SetBackTextMargin(Point2D<float> p){
		pImpl->m_BackTextMargin = p;
	}

	const Color4& StringSprite::GetCaretColor() const{
		return pImpl->m_CaretColor;
	}
	void StringSprite::SetCaretColor(const Color4& Col){
		if (!IsKeyInput()){
			return;
		}
		pImpl->m_CaretColor = Col;
		auto ColBrush = D2D1::ColorF(pImpl->m_CaretColor.x, pImpl->m_CaretColor.y, pImpl->m_CaretColor.z, pImpl->m_CaretColor.w);
		// �f�o�C�X�Ɉˑ����郊�\�[�X���쐬���܂��B
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		ThrowIfFailed(
			D2DDeviceContext->CreateSolidColorBrush(
			ColBrush,
			&pImpl->m_CaretBrush
			),
			L"�L�����b�g�u���V�ݒ�Ɏ��s���܂����B",
			L"D2DDeviceContext->CreateSolidColorBrush()",
			L"StringSprite::SetCaretColor()"
			);
	}

	float StringSprite::GetCaretSpeed() const{
		if (!IsKeyInput()){
			return 0.0f;
		}
		return pImpl->m_CaretSpeed;
	}
	void StringSprite::SetCaretSpeed(float f){
		if (!IsKeyInput()){
			return;
		}
		pImpl->m_CaretSpeed = f;
	}


	const wstring& StringSprite::GetFontName() const{
		return pImpl->m_FontName;
	}
	float StringSprite::GetFontSize() const{
		return pImpl->m_FoneSize;
	}

	float StringSprite::GetTextBlockWidth() const{
		return pImpl->m_TextBlockWidth;
	}
	void StringSprite::SetTextBlockWidth(float f){
		pImpl->m_TextBlockWidth = f;
	}
	float StringSprite::GetTextBlockHeight() const{
		return pImpl->m_TextBlockHeight;
	}
	void StringSprite::SetTextBlockHeight(float f){
		pImpl->m_TextBlockHeight = f;
	}

	bool StringSprite::GetKeyInput() const{
		return pImpl->m_KeyInput;
	}
	bool StringSprite::IsKeyInput() const{
		return pImpl->m_KeyInput;

	}
	void StringSprite::SetKeyInput(bool b){
		pImpl->m_KeyInput = b;
	}


	ComPtr<IDWriteTextLayout>& StringSprite::GetTextLayout()const{
		return pImpl->m_textLayout;
	}
	ComPtr<IDWriteTextFormat>&	StringSprite::GetTextFormat()const{
		return pImpl->m_textFormat;
	}

	const DWRITE_TEXT_METRICS& StringSprite::GetDriteTextMetrics() const{
		return pImpl->m_textMetrics;
	}

	const Point2D<float>& StringSprite::GetStartPosition() const{
		return pImpl->m_StartPosition;
	}
	void StringSprite::SetStartPosition(const Point2D<float>& pos){
		pImpl->m_StartPosition = pos;
	}

	Rect2D<float> StringSprite::GetTextRect() const{
		Rect2D<float> ret;
		ret.left = GetStartPosition().x;
		ret.top = GetStartPosition().y;
		ret.right = ret.left + GetTextBlockWidth();
		ret.bottom = ret.top + GetTextBlockHeight();
		return ret;
	}
	void StringSprite::SetTextRect(const Rect2D<float>& rect){
		SetStartPosition(Point2D<float>(rect.left, rect.top));
		SetTextBlockWidth(rect.Width());
		SetTextBlockHeight(rect.Height());
	}



	void StringSprite::OnKeyDown(WPARAM wParam, LPARAM lParam){
		if (!IsKeyInput()){
			return;
		}
		wstring TempText = GetText();
		switch (wParam){
		case VK_BACK:
			if (pImpl->m_Caret > 0 && TempText.size() > 0){
				pImpl->m_Caret--;
				wstring LeftStr = TempText.substr(0, pImpl->m_Caret);
				wstring RightStr = TempText.substr(pImpl->m_Caret + 1);
				TempText = LeftStr + RightStr;
				SetText(TempText);
			}
			break;
		case VK_RIGHT:
			if (pImpl->m_Caret < TempText.size()){
				pImpl->m_Caret++;
			}
			break;
		case VK_LEFT:
			if (pImpl->m_Caret > 0){
				pImpl->m_Caret--;
			}
			break;
		default:
			break;
		}

	}
	void StringSprite::OnChar(WPARAM wParam, LPARAM lParam){
		if (!IsKeyInput()){
			return;
		}
		wstring str(L"");
		switch (wParam){
		case VK_RIGHT:
		case VK_LEFT:
		case VK_BACK:
			break;
		default:
			str += (wchar_t)wParam;
			InsertText(str);
			break;
		}
	}


	void StringSprite::Update(){
		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();

		ThrowIfFailed(
			DWriteFactory->CreateTextLayout(
			pImpl->m_text.c_str(),
			(uint32)pImpl->m_text.length(),
			pImpl->m_textFormat.Get(),
			pImpl->m_TextBlockWidth, // ���̓e�L�X�g�̍ő啝�B
			pImpl->m_TextBlockHeight, // ���̓e�L�X�g�̍ő卂���B
			&pImpl->m_textLayout
			),
			L"�t�H���g���C�A�E�g�ݒ�Ɏ��s���܂����B",
			L"DWriteFactory->CreateTextLayout()",
			L"StringSprite::Update()"
			);
		ThrowIfFailed(
			pImpl->m_textLayout->GetMetrics(&pImpl->m_textMetrics),
			L"�e�L�X�g���g���N�X�擾�Ɏ��s���܂����B",
			L"pImpl->m_textLayout->GetMetrics()",
			L"StringSprite::Update()"
			);

	}



	void StringSprite::Draw(){

		auto Dev = App::GetApp()->GetDeviceResources();
		auto D2DDeviceContext = Dev->GetD2DDeviceContext();
		auto DWriteFactory = Dev->GetDWriteFactory();
		float ElapsedTime = App::GetApp()->GetElapsedTime();


		D2D1_RECT_F BackRectangle = D2D1::RectF(
			pImpl->m_StartPosition.x, pImpl->m_StartPosition.y,
			pImpl->m_StartPosition.x + pImpl->m_TextBlockWidth,
			pImpl->m_StartPosition.y + pImpl->m_TextBlockHeight
			);
		BackRectangle.left -= pImpl->m_BackTextMargin.x;
		BackRectangle.top -= pImpl->m_BackTextMargin.y;
		BackRectangle.right += pImpl->m_BackTextMargin.x;
		BackRectangle.bottom += pImpl->m_BackTextMargin.y;

		D2DDeviceContext->SaveDrawingState(pImpl->m_stateBlock.Get());
		D2DDeviceContext->BeginDraw();

		//�o�b�N�O���E���h�̕`��
		D2DDeviceContext->FillRectangle(&BackRectangle, pImpl->m_BackBrush.Get());


		D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(
			pImpl->m_StartPosition.x,
			pImpl->m_StartPosition.y
			);

		D2DDeviceContext->SetTransform(screenTranslation);

		DWRITE_TEXT_ALIGNMENT Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;

		switch (pImpl->m_TextAlignment){
		case TextAlignment::m_Left:
			Alignment = DWRITE_TEXT_ALIGNMENT_LEADING;
			break;
		case TextAlignment::m_Center:
			Alignment = DWRITE_TEXT_ALIGNMENT_CENTER;
			break;
		case TextAlignment::m_Right:
			Alignment = DWRITE_TEXT_ALIGNMENT_TRAILING;
			break;
		}


		ThrowIfFailed(
			pImpl->m_textFormat->SetTextAlignment(Alignment),
			L"�e�L�X�g�A���C�������g�ݒ�Ɏ��s���܂����B",
			L"pImpl->m_textFormat->SetTextAlignment()",
			L"StringSprite::Draw()"
			);

		D2DDeviceContext->DrawTextLayout(
			D2D1::Point2F(0.f, 0.f),
			pImpl->m_textLayout.Get(),
			pImpl->m_Brush.Get()
			);

		HRESULT hr = D2DDeviceContext->EndDraw();
		if (hr != D2DERR_RECREATE_TARGET)
		{
			if (FAILED(hr)){
				throw BaseException(
					L"�������\���ł��܂���ł���",
					L"if (hr != D2DERR_RECREATE_TARGET)",
					L"StringSprite::Draw()"
					);
			}
		}
		D2DDeviceContext->RestoreDrawingState(pImpl->m_stateBlock.Get());
	}


	//--------------------------------------------------------------------------------------
	//	struct AudioResource::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct AudioResource::Impl{
		WAVEFORMATEX m_WaveFormat;	//�E�F�u�t�H�[�}�b�g
		vector<byte> m_SoundData;	//�f�[�^
		wstring m_FileName;		//�t�@�C���ւ̃p�X
		Impl(const wstring& FileName) :
			m_FileName(FileName){}
		~Impl(){}
		//�~���[�e�b�N�X
		std::mutex Mutex;

	};


	//--------------------------------------------------------------------------------------
	//	class AudioResource : public BaseResource;
	//	�p�r: �I�[�f�B�I���\�[�X�iwav�Ȃǁj�̃��b�s���O�N���X
	//--------------------------------------------------------------------------------------
	AudioResource::AudioResource(const wstring& FileName) :
		pImpl(new Impl(FileName))
	{
		try{

			ThrowIfFailed(
				MFStartup(MF_VERSION),
				L"MediaFoundation�̏������Ɏ��s���܂���",
				L"MFStartup(MF_VERSION)",
				L"AudioResource::AudioResource()"
				);

			ComPtr<IMFSourceReader> reader;

			ThrowIfFailed(
				MFCreateSourceReaderFromURL(FileName.c_str(), nullptr, &reader),
				L"�T�E���h���[�_�[�̍쐬�Ɏ��s���܂���",
				L"MFCreateSourceReaderFromURL(FileName.c_str(),nullptr,&reader)",
				L"AudioResource::AudioResource()"
				);

			// Set the decoded output format as PCM.
			// XAudio2 on Windows can process PCM and ADPCM-encoded buffers.
			// When using MediaFoundation, this sample always decodes into PCM.
			Microsoft::WRL::ComPtr<IMFMediaType> mediaType;

			ThrowIfFailed(
				MFCreateMediaType(&mediaType),
				L"���f�B�A�^�C�v�̍쐬�Ɏ��s���܂���",
				L"MFCreateMediaType(&mediaType)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio),
				L"���f�B�AGUID�̐ݒ�Ɏ��s���܂���",
				L"mediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM),
				L"���f�B�A�T�uGUID�̐ݒ�Ɏ��s���܂���",
				L"mediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM)",
				L"AudioResource::AudioResource()"
				);

			ThrowIfFailed(
				reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get()),
				L"���[�_�[�ւ̃��f�B�A�^�C�v�̐ݒ�Ɏ��s���܂���",
				L"reader->SetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, mediaType.Get())",
				L"AudioResource::AudioResource()"
				);


			// Get the complete WAVEFORMAT from the Media Type.
			Microsoft::WRL::ComPtr<IMFMediaType> outputMediaType;

			ThrowIfFailed(
				reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType),
				L"�o�͗p�̃��f�B�A�^�C�v�̐ݒ�Ɏ��s���܂���",
				L"reader->GetCurrentMediaType(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), &outputMediaType)",
				L"AudioResource::AudioResource()"
				);

			UINT32 size = 0;
			WAVEFORMATEX* waveFormat;

			ThrowIfFailed(
				MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size),
				L"�E�F�u�t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���",
				L"MFCreateWaveFormatExFromMFMediaType(outputMediaType.Get(), &waveFormat, &size)",
				L"AudioResource::AudioResource()"
				);


			CopyMemory(&pImpl->m_WaveFormat, waveFormat, sizeof(pImpl->m_WaveFormat));
			CoTaskMemFree(waveFormat);

			PROPVARIANT propVariant;

			ThrowIfFailed(
				reader->GetPresentationAttribute(static_cast<uint32>(MF_SOURCE_READER_MEDIASOURCE), MF_PD_DURATION, &propVariant),
				L"�A�g���r���[�g�̐ݒ�Ɏ��s���܂���",
				L"reader->GetPresentationAttribute(static_cast<uint32>(MF_SOURCE_READER_MEDIASOURCE), MF_PD_DURATION, &propVariant)",
				L"AudioResource::AudioResource()"
				);

			// 'duration' is in 100ns units; convert to seconds, and round up
			// to the nearest whole byte.
			LONGLONG duration = propVariant.uhVal.QuadPart;
			unsigned int maxStreamLengthInBytes =
				static_cast<unsigned int>(
				((duration * static_cast<ULONGLONG>(pImpl->m_WaveFormat.nAvgBytesPerSec)) + 10000000) /
				10000000
				);

			pImpl->m_SoundData.resize(maxStreamLengthInBytes);

			ComPtr<IMFSample> sample;
			ComPtr<IMFMediaBuffer> mediaBuffer;
			DWORD flags = 0;

			int positionInData = 0;
			bool done = false;
			while (!done)
			{

				ThrowIfFailed(
					reader->ReadSample(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &flags, nullptr, &sample),
					L"�T���v���[�̓ǂݍ��݂Ɏ��s���܂���",
					L"reader->ReadSample(static_cast<uint32>(MF_SOURCE_READER_FIRST_AUDIO_STREAM), 0, nullptr, &flags, nullptr, &sample)",
					L"AudioResource::AudioResource()"
					);

				if (sample != nullptr)
				{
					ThrowIfFailed(
						sample->ConvertToContiguousBuffer(&mediaBuffer),
						L"�T���v���[�̃R���o�[�g�Ɏ��s���܂���",
						L"sample->ConvertToContiguousBuffer(&mediaBuffer)",
						L"AudioResource::AudioResource()"
						);

					BYTE *audioData = nullptr;
					DWORD sampleBufferLength = 0;

					ThrowIfFailed(
						mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength),
						L"�o�b�t�@��Lock�Ɏ��s���܂���",
						L"mediaBuffer->Lock(&audioData, nullptr, &sampleBufferLength)",
						L"AudioResource::AudioResource()"
						);

					for (DWORD i = 0; i < sampleBufferLength; i++)
					{
						pImpl->m_SoundData[positionInData++] = audioData[i];
					}
				}
				if (flags & MF_SOURCE_READERF_ENDOFSTREAM)
				{
					done = true;
				}
			}
		}
		catch (...){
			throw;
		}
	}
	AudioResource::~AudioResource(){}

	const vector<byte>& AudioResource::GetSoundData()const{
		return pImpl->m_SoundData;
	}

	WAVEFORMATEX*  AudioResource::GetOutputWaveFormatEx()const{
		return &pImpl->m_WaveFormat;
	}



	//--------------------------------------------------------------------------------------
	//	struct SoundItem;
	/*!
	�}���`�T�E���h�Ŏg�p����A�C�e��
	*/
	//--------------------------------------------------------------------------------------
	struct SoundItem{
		shared_ptr<AudioResource> m_AudioResource;
		IXAudio2SourceVoice* m_pSourceVoice;
	};

	//--------------------------------------------------------------------------------------
	//	struct MultiAudioObject::Impl;
	//	�p�r: Impl�C�f�B�I��
	//--------------------------------------------------------------------------------------
	struct MultiAudioObject::Impl{
		map<wstring, SoundItem> m_SoundMap;
		Impl()
		{}
		~Impl(){
		}
		SoundItem* GetItem(const wstring& ResKey){
			auto it = m_SoundMap.find(ResKey);
			if (it != m_SoundMap.end()){
				//�������O����������
				return &it->second;
			}
			return nullptr;
		}
		void RemoveItem(const wstring& ResKey){
			auto it = m_SoundMap.find(ResKey);
			if (it != m_SoundMap.end()){
				//�������O����������
				//Map����폜
				m_SoundMap.erase(it);
			}
		}
		void Start(const SoundItem* pItem, const XAUDIO2_BUFFER& Buffer, float Volume = 1.0f);
		void Stop(const SoundItem* pItem);
	};

	void MultiAudioObject::Impl::Start(const SoundItem* pItem, const XAUDIO2_BUFFER& Buffer, float Volume){
		if (!pItem->m_pSourceVoice){
			throw BaseException(
				L"�T�E���h�{�C�X���s��ł�",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiAudioObject::Impl::Start()"
				);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->Stop(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiAudioObject::Impl::Start()"
			);
		ThrowIfFailed(
			pItem->m_pSourceVoice->FlushSourceBuffers(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiAudioObject::Impl::Start()"
			);
		ThrowIfFailed(
			pItem->m_pSourceVoice->SetVolume(Volume),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃{�����[���ݒ�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->SetVolume()",
			L"MultiAudioObject::Impl::Start()"
			);

		ThrowIfFailed(
			pItem->m_pSourceVoice->SubmitSourceBuffer(&Buffer),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃\�[�X�o�b�t�@�ݒ�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->SubmitSourceBuffer(&buffer)",
			L"MultiAudioObject::Impl::Start()"
			);

		ThrowIfFailed(
			pItem->m_pSourceVoice->Start(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�̃X�^�[�g�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->Start()",
			L"MultiAudioObject::Impl::Start()"
			);
	}

	void MultiAudioObject::Impl::Stop(const SoundItem* pItem){
		if (!pItem->m_pSourceVoice){
			throw BaseException(
				L"�T�E���h�{�C�X���s��ł�",
				L"if (!pItem->m_pSourceVoice)",
				L"MultiAudioObject::Impl::Stop()"
				);
		}
		ThrowIfFailed(
			pItem->m_pSourceVoice->Stop(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̒�~�Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->Stop()",
			L"MultiAudioObject::Impl::Stop()"
			);
		ThrowIfFailed(
			pItem->m_pSourceVoice->FlushSourceBuffers(),
			L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̃o�b�t�@�̃t���b�V���Ɏ��s���܂���",
			L"pItem->m_pSourceVoice->FlushSourceBuffers()",
			L"MultiAudioObject::Impl::Stop()"
			);
	}



	//--------------------------------------------------------------------------------------
	//	class MultiAudioObject: public Object;
	//	�p�r: �}���`�I�[�f�B�I
	// ���R���|�[�l���g�ł͂Ȃ��̂ŃV�[���Ɏ������邱�Ƃ��ł���
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	MultiAudioObject::MultiAudioObject() :
		pImpl(new Impl())
	{}
	MultiAudioObject::~MultiAudioObject(){}

	//�A�N�Z�T
	shared_ptr<AudioResource> MultiAudioObject::AddAudioResource(const wstring& FileName,const wstring& ResKey){
		try{
			if (ResKey == L""){
				throw BaseException(
					L"�I�[�f�B�I���\�[�X�����󔒂ł�",
					L"if (ResKey == L\"\")",
					L"MultiAudioObject::AddAudioResource()"
					);
			}
			auto pItem = pImpl->GetItem(ResKey);
			if (pItem){
				//�������O����������
				return pItem->m_AudioResource;
			}
			auto SoundRes = Object::CreateObject<AudioResource>(FileName);
			SoundItem Item;
			Item.m_AudioResource = SoundRes;
			auto Engine = App::GetApp()->GetAudioManager()->GetSoundEffectEngine();
			ThrowIfFailed(
				Engine->CreateSourceVoice(&Item.m_pSourceVoice, SoundRes->GetOutputWaveFormatEx()),
				L"�T�E���h�G�t�F�N�g�p�T�E���h�{�C�X�̍쐬�Ɏ��s���܂���",
				L"Engine->CreateSourceVoice(&pImpl->m_pSourceVoice, SountRes->GetOutputWaveFormatEx())",
				L"MultiAudioObject::AddAudioResource()"
				);
			pImpl->m_SoundMap[ResKey] = Item;
			return SoundRes;
		}
		catch (...){
			throw;
		}
	}
	void MultiAudioObject::RemoveAudioResource(const wstring& ResKey){
		if (ResKey == L""){
			throw BaseException(
				L"�I�[�f�B�I���\�[�X�����󔒂ł�",
				L"if (ResKey == L\"\")",
				L"MultiAudioObject::RemoveAudioResource()"
				);
		}
		pImpl->RemoveItem(ResKey);
	}


	shared_ptr<AudioResource> MultiAudioObject::GetAudioResource(const wstring& ResKey, bool ExceptionActive) const{
		if (ResKey == L""){
			if (ExceptionActive){
				throw BaseException(
					L"�I�[�f�B�I���\�[�X�����󔒂ł�",
					L"if (ResKey == L\"\")",
					L"MultiAudioObject::GetAudioResource()"
					);
			}
			else{
				return nullptr;
			}
		}
		auto pItem = pImpl->GetItem(ResKey);
		if (pItem){
			//�������O����������
			return pItem->m_AudioResource;
		}
		else{
			if (ExceptionActive){
				throw BaseException(
					L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
					ResKey,
					L"MultiAudioObject::GetAudioResource()"
					);
			}
			else{
				return nullptr;
			}
		}
	}

	void MultiAudioObject::Start(const wstring& ResKey, const XAUDIO2_BUFFER& Buffer, float Volume){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
				ResKey,
				L"MultiAudioObject::Start()"
				);
		}
		pImpl->Start(pItem, Buffer, Volume);
	}

	void MultiAudioObject::Start(const wstring& ResKey, size_t LoopCount, float Volume){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
				ResKey,
				L"MultiAudioObject::Start()"
				);
		}
		if (!pItem->m_AudioResource){
			throw BaseException(
				L"�I�[�f�B�I���\�[�X���L���ł͂���܂���",
				ResKey,
				L"MultiAudioObject::Start()"
				);
		}
		XAUDIO2_BUFFER buffer = { 0 };
		auto ResPtr = pItem->m_AudioResource;
		buffer.AudioBytes = ResPtr->GetSoundData().size();
		buffer.LoopCount = LoopCount;
		buffer.pAudioData = &ResPtr->GetSoundData().front();
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		//pImpl�̊֐����Ă�
		pImpl->Start(pItem, buffer, Volume);
	}
	void MultiAudioObject::Stop(const wstring& ResKey){
		auto pItem = pImpl->GetItem(ResKey);
		if (!pItem){
			throw BaseException(
				L"�w��̖��O�̃I�[�f�B�I���\�[�X������܂���",
				ResKey,
				L"MultiAudioObject::Stop()"
				);
		}
		pImpl->Stop(pItem);
	}






}
//endof  basedx11


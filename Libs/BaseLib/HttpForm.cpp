#include "stdafx.h"

namespace basedx11{


	//--------------------------------------------------------------------------------------
	//	struct StaticText::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct StaticText::Impl{
		Impl()
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class StaticText : public InputObject;
	//	�p�r: �\���p�e�L�X�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	StaticText::StaticText(const shared_ptr<Stage>& StagePtr) :
		GameObject(StagePtr),
		pImpl(new Impl())
	{}
	StaticText::~StaticText(){}

	//������
	void StaticText::Create(){
		//�\���p�e�L�X�g�̍쐬
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetStartPosition(Point2D<float>(0, 0));
		PtrString->SetText(L"");
		PtrString->SetKeyInput(false);
	}

	void StaticText::SetFont(const wstring& FontName, float FontSize){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetFont(FontName, FontSize);
	}
	StringSprite::TextAlignment StaticText::GetTextAlignment() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetTextAlignment();
	}
	void StaticText::SetTextAlignment(StringSprite::TextAlignment Alignment){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetTextAlignment(Alignment);
	}

	const Color4& StaticText::GetFontColor() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetFontColor();
	}
	void StaticText::SetFontColor(const Color4& Col){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetFontColor(Col);
	}
	const wstring& StaticText::GetFontName() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetFontName();

	}
	float StaticText::GetFontSize() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetFontSize();
	}

	const Color4& StaticText::GetBackColor() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetBackColor();
	}
	void StaticText::SetBackColor(const Color4& Col){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetBackColor(Col);
	}

	Point2D<float> StaticText::GetBackTextMargin() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetBackTextMargin();
	}
	void StaticText::SetBackTextMargin(Point2D<float> p){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetBackTextMargin(p);
	}

	Rect2D<float> StaticText::GetTextRect() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetTextRect();
	}
	void StaticText::SetTextRect(const Rect2D<float>& rect){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetTextRect(rect);
	}

	const wstring& StaticText::GetText() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetText();
	}
	void StaticText::SetText(const wstring& str){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetText(str);
	}
	void StaticText::AddText(const wstring& str){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->AddText(str);
	}


	//--------------------------------------------------------------------------------------
	//	struct InputObject::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct InputObject::Impl{
		wstring m_Name;
		wstring m_Value;
		//�A�g���r���[�g�}�b�v
		map<wstring, wstring> m_AttributeMap;
		Impl():
			m_Name(L""),
			m_Value(L"")
		{}
		~Impl(){}
	};

	//--------------------------------------------------------------------------------------
	//	class InputObject : public Object;
	//	�p�r: ���̓I�u�W�F�N�g�̐e�N���X
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	InputObject::InputObject(const shared_ptr<Stage>& StagePtr,
		const wstring& Name, const wstring& Value) :
		GameObject(StagePtr),
		pImpl(new Impl())
	{
		pImpl->m_AttributeMap[L"Name"] = Name;
		pImpl->m_AttributeMap[L"Value"] = Value;
	}
	InputObject::~InputObject(){}

	const wstring& InputObject::GetAttribute(const wstring& Key) const{
		return pImpl->m_AttributeMap[Key];
	}
	void InputObject::SetAttribute(const wstring& Key, const wstring& Val){
		pImpl->m_AttributeMap[Key] = Val;
	}



	const wstring& InputObject::GetName(){
		return pImpl->m_AttributeMap[L"Name"];
	}
	void InputObject::SetName(const wstring& Str){
		pImpl->m_AttributeMap[L"Name"] = Str;
	}
	const wstring& InputObject::GetValue(){
		return pImpl->m_AttributeMap[L"Value"];
	}
	void InputObject::SetValue(const wstring& Str){
		pImpl->m_AttributeMap[L"Value"] = Str;
	}


	//--------------------------------------------------------------------------------------
	//	class InputHidden : public InputObject;
	//	�p�r: �\���Ȃ��e�L�X�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	InputHidden::InputHidden(const shared_ptr<Stage>& StagePtr,
		const wstring& Name, const wstring& Value) :
		InputObject(StagePtr, Name, Value)
	{}
	InputHidden::~InputHidden(){}




	//--------------------------------------------------------------------------------------
	//	struct InputText::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct InputText::Impl{
		Impl()
		{}
		~Impl(){}
	};


	//--------------------------------------------------------------------------------------
	//	class InputText : public InputObject;
	//	�p�r: ���̓e�L�X�g
	//--------------------------------------------------------------------------------------
	//�\�z�Ɣj��
	InputText::InputText(const shared_ptr<Stage>& StagePtr,
		const wstring& Name, const wstring& Value) :
		InputObject(StagePtr, Name, Value),
		pImpl(new Impl())
	{}
	InputText::~InputText(){}

	bool InputText::IsFocus()const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->IsFocus();
	}
	bool InputText::GetFocus()const{
		return IsFocus();
	}

	void InputText::SetFocus(bool b){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetFocus(true);
	}

	void InputText::SetFont(const wstring& FontName, float FontSize){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetFont(FontName, FontSize);
	}
	StringSprite::TextAlignment InputText::GetTextAlignment() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetTextAlignment();
	}
	void InputText::SetTextAlignment(StringSprite::TextAlignment Alignment){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetTextAlignment(Alignment);
	}

	const Color4& InputText::GetFontColor() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetFontColor();
	}
	void InputText::SetFontColor(const Color4& Col){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetFontColor(Col);
	}
	const wstring& InputText::GetFontName() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetFontName();

	}
	float InputText::GetFontSize() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetFontSize();
	}

	const Color4& InputText::GetBackColor() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetBackColor();
	}
	void InputText::SetBackColor(const Color4& Col){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetBackColor(Col);
	}

	Point2D<float> InputText::GetBackTextMargin() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetBackTextMargin();
	}
	void InputText::SetBackTextMargin(Point2D<float> p){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetBackTextMargin(p);
	}



	const Color4& InputText::GetCaretColor() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetCaretColor();
	}
	void InputText::SetCaretColor(const Color4& Col){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetCaretColor(Col);
	}
	float InputText::GetCaretSpeed() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetCaretSpeed();
	}
	void InputText::SetCaretSpeed(float f){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetCaretSpeed(f);
	}


	Rect2D<float> InputText::GetTextRect() const{
		auto Ptr = GetComponent<StringSprite>();
		return Ptr->GetTextRect();
	}
	void InputText::SetTextRect(const Rect2D<float>& rect){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetTextRect(rect);
	}

	const wstring& InputText::GetValue(){
		auto Ptr = GetComponent<StringSprite>();
		//���̓e�L�X�g��Value�ɃZ�b�g
		InputObject::SetValue(Ptr->GetText());
		return InputObject::GetValue();
	}
	void InputText::SetValue(const wstring& Str){
		auto Ptr = GetComponent<StringSprite>();
		Ptr->SetText(Str);
		//���̓e�L�X�g�Ɠ������̂�Value�ɃZ�b�g
		InputObject::SetValue(Str);
	}


	//������
	void InputText::Create(){
		//���̓e�L�X�g�̍쐬
		auto PtrString = AddComponent<StringSprite>();
		PtrString->SetStartPosition(Point2D<float>(240.0f, 320.0f));
		PtrString->SetText(InputObject::GetValue());
		PtrString->SetKeyInput(true);
	}


	//--------------------------------------------------------------------------------------
	//	struct HttpForm::Impl;
	//	�p�r: Impl�N���X
	//--------------------------------------------------------------------------------------
	struct HttpForm::Impl{
		HINTERNET m_hSession;	//�Z�b�V�����n���h��
		wstring m_SessionName;	//�Z�b�V������
		HINTERNET m_hConnect;	//�R�l�N�V�����n���h��
		HINTERNET m_hRequest;	//���N�G�X�g�n���h��
		stringstream m_ReadData;	//�ǂݍ��ݍς݃f�[�^

		string m_StrBody;	//POST�p�Ɏg�p����镶����

		wstring m_GroupName;	//�O���[�v�o�^���̖��O
		bool m_SubmitLock;	//�T�u�~�b�g�̃��b�N
		Impl() :
			m_hSession(NULL),
			m_SessionName(L"DxBaseSession"),
			m_hConnect(NULL),
			m_hRequest(NULL),
			m_SubmitLock(false)
		{}
		~Impl(){}
		//���[�e�B���e�B�֐�
		bool IsOkChar(char c) {
			if (isalnum(c) || c == '.' || c == '-' || c == '_' || c == '*'){
				return true;
			}
			return false;
		}
		void EncodeHex(char c, char* Buff) {
			Buff[1] = "0123456789ABCDEF"[(c & 0xF0) >> 4];
			Buff[2] = "0123456789ABCDEF"[c & 0x0F];
		}
		void UrlEnc(const char* pC, string& Ret){
			//������
			Ret = "";
			char Buff[] = { '%', 0, 0, 0 };
			while (*pC != '\0'){
				if (IsOkChar(*pC)){
					Ret += *pC;
				}
				else if (*pC == ' '){
					Ret += '+';
				}
				else{
					EncodeHex(*pC, Buff);
					Ret += Buff;
				}
				pC++;
			}
		}
		void MakeParam(const wstring& Key, const wstring& Value, wstring& Ret,bool FirstFlg,bool Get = true){
			string Utf8Str;
			string UrlEncStr;
			wstring WUrlParam;
			Ret = L"";
			if (FirstFlg ){
				if (Get){
					Ret += L"?";
				}
			}
			else{
				Ret += L"&";
			}
			Util::ConvertWstringtoUtf8(Key, Utf8Str);
			UrlEnc(Utf8Str.c_str(), UrlEncStr);
			Util::MBtoWS(UrlEncStr, WUrlParam);
			Ret += WUrlParam;
			Ret += L"=";
			Util::ConvertWstringtoUtf8(Value, Utf8Str);
			UrlEnc(Utf8Str.c_str(), UrlEncStr);
			Util::MBtoWS(UrlEncStr, WUrlParam);
			Ret += WUrlParam;
		}
		void ReadDataClear(){
			m_ReadData.str("");
			m_ReadData.clear(stringstream::goodbit);
		}
		void Clear(){
			if (m_hRequest){
				WinHttpCloseHandle(m_hRequest);
				m_hRequest = NULL;
			}
			if (m_hConnect){
				WinHttpCloseHandle(m_hConnect);
				m_hConnect = NULL;
			}
			m_SubmitLock = false;
		}
		void SessionClear(){
			if (m_hSession){
				//�Z�b�V�����̃N���[�Y
				WinHttpCloseHandle(m_hSession);
				m_hSession = NULL;
			}
		}
	};


	//--------------------------------------------------------------------------------------
	//	class HttpForm : public GameObject;
	//	�p�r: HttpForm�N���X
	//--------------------------------------------------------------------------------------
	void HttpForm::SessionCreate(){
		try{
			// �Z�b�V�����I�u�W�F�N�g�̍쐬
			WINHTTP_CURRENT_USER_IE_PROXY_CONFIG ProxyConfig;
			ZeroMemory(&ProxyConfig, sizeof(ProxyConfig));
			WinHttpGetIEProxyConfigForCurrentUser(&ProxyConfig);
			if (ProxyConfig.lpszProxy){
				// InternetExplorer�̃v���L�V���g�p���ăZ�b�V�����I�u�W�F�N�g�쐬
				pImpl->m_hSession = WinHttpOpen(
					pImpl->m_SessionName.c_str(),	//���[�U�[�G�[�W�F���g�� 
					WINHTTP_ACCESS_TYPE_NAMED_PROXY,//�v���L�V��ݒ肷��
					ProxyConfig.lpszProxy,	// �v���L�V����
					ProxyConfig.lpszProxyBypass ?
					ProxyConfig.lpszProxyBypass : WINHTTP_NO_PROXY_BYPASS,	//�v���L�V�o�C�p�X�̐ݒ�
					WINHTTP_FLAG_ASYNC
					);
			}
			else{
				pImpl->m_hSession = WinHttpOpen(
					pImpl->m_SessionName.c_str(),	//���[�U�[�G�[�W�F���g�� 
					WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,	//�f�t�H���g�̃v���L�V
					WINHTTP_NO_PROXY_NAME,	// �v���L�V����
					WINHTTP_NO_PROXY_BYPASS,	// �v���L�V�o�C�p�X�̐ݒ�
					WINHTTP_FLAG_ASYNC
					);
			}
			if (!pImpl->m_hSession){
				throw BaseException(
					L"Http�̃Z�b�V�������m���ł��܂���ł���",
					L"if (!pImpl->m_hSession)",
					L"HttpForm::Create()"
					);
			}
		}
		catch (...){
			throw;
		}
	}


	//�\�z�Ə���
	HttpForm::HttpForm(const shared_ptr<Stage>& StagePtr,const wstring& GroupName) :
		GameObject(StagePtr),
		pImpl(new Impl())
	{
		pImpl->m_GroupName = GroupName;
		StagePtr->CreateSharedObjectGroup(pImpl->m_GroupName);
	}
	HttpForm::~HttpForm(){
		pImpl->Clear();
		pImpl->ReadDataClear();
		pImpl->SessionClear();
	}

	//�A�N�Z�T
	const wstring& HttpForm::GetGroupName() const{
		return pImpl->m_GroupName;
	}

	//����
	void HttpForm::MoveFocusNextInput(){
		vector<shared_ptr<InputText>> InputTextVec;
		//�^�u�L�[�������ꂽ��A�t�H�[�J�X��ύX����
		auto Group = GetStage()->GetSharedObjectGroup(GetGroupName());
		auto GroupVec = Group->GetGroupVector();
		for (size_t i = 0; i < GroupVec.size(); i++){
			if (!GroupVec[i].expired()){
				auto ShPtr = dynamic_pointer_cast<InputText>(GroupVec[i].lock());
				if (ShPtr){
					InputTextVec.push_back(ShPtr);
				}
			}
		}
		if (InputTextVec.size() > 0){
			bool FocusActive = false;
			for (size_t i = 0; i < InputTextVec.size(); i++){
				if (InputTextVec[i]->IsFocus()){
					FocusActive = true;
					//�ǂ����Ƀt�H�[�J�X��������
					if ((i + 1) < InputTextVec.size()){
						InputTextVec[i + 1]->SetFocus(true);
						break;
					}
					else{
						InputTextVec[0]->SetFocus(true);
						break;
					}
				}
			}
			if (!FocusActive){
				//�ǂ��ɂ��t�H�[�J�X���Ȃ�
				InputTextVec[0]->SetFocus(true);
			}
		}
	}


	shared_ptr<InputText> HttpForm::AddInputText(const wstring& Name, const wstring& Value){
		auto Ptr = GetStage()->AddGameObject<InputText>(Name, Value);
		Ptr->SetFocus(true);
		//�w�i�͔�
		Ptr->SetBackColor(Color4(1.0f, 1.0f, 1.0f, 1.0f));
		//�t�H���g�͍�
		Ptr->SetFontColor(Color4(0.0f, 0.0f, 0.0f, 1.0f));
		//�L�����b�g����
		Ptr->SetCaretColor(Color4(0.0f, 0.0f, 0.0f, 1.0f));
		auto Group = GetStage()->GetSharedObjectGroup(pImpl->m_GroupName);
		Group->IntoGroup(Ptr);
		return Ptr;
	}

	shared_ptr<InputHidden> HttpForm::AddInputHidden(const wstring& Name, const wstring& Value){
		auto Ptr = GetStage()->AddGameObject<InputHidden>(Name, Value);
		auto Group = GetStage()->GetSharedObjectGroup(pImpl->m_GroupName);
		Group->IntoGroup(Ptr);
		return Ptr;
	}


	//�R�[���o�b�N�֐�
	void CALLBACK HttpFormRequestCallBack(
		HINTERNET hInternet,
		DWORD_PTR PtrContext,
		DWORD InternetStatus,
		LPVOID pStatusInformation,
		DWORD StatusInformationLength
		){
		if (PtrContext == 0){
			return;
		}
		//�C���X�^���X�̃R�[���o�b�N���b�Z�[�W���Ă�
		HttpForm* pHttpForm = reinterpret_cast<HttpForm*>(PtrContext);
		pHttpForm->OnCallback(InternetStatus,pStatusInformation,StatusInformationLength);
	}


	void HttpForm::GetRequest(const wstring& Url){
		Request(Url,false);
	}
	void HttpForm::PostRequest(const wstring& Url){
		Request(Url, true);
	}



	void HttpForm::Request(const wstring& Url, bool Post){
		if (!pImpl->m_hSession){
			SessionCreate();
		}
		const size_t UrlBufSize = 4096;
		if (!pImpl->m_SubmitLock && pImpl->m_hSession){
			pImpl->m_SubmitLock = true;
			auto Group = GetStage()->GetSharedObjectGroup(pImpl->m_GroupName);
			wstring Param;
			wstring AllParams;
			auto GroupVec = Group->GetGroupVector();
			bool FirstFlg = true;
			for (auto Obj : GroupVec){
				if (!Obj.expired()){
					auto ShPtr = dynamic_pointer_cast<InputObject>(Obj.lock());
					if (ShPtr){
						//�p�����[�^�ɒǉ�
						if (Post){
							pImpl->MakeParam(ShPtr->GetName(), ShPtr->GetValue(), Param, FirstFlg,false);
						}
						else{
							pImpl->MakeParam(ShPtr->GetName(), ShPtr->GetValue(), Param, FirstFlg);
						}
						AllParams += Param;
						FirstFlg = false;
					}
				}
			}

			//URL����͂���
			URL_COMPONENTS UrlComponents;
			ZeroMemory(&UrlComponents, sizeof(UrlComponents));
			UrlComponents.dwStructSize = sizeof(UrlComponents);
			wchar_t HostName[UrlBufSize];
			wchar_t UrlPath[UrlBufSize];
			UrlComponents.lpszHostName = HostName;
			UrlComponents.lpszUrlPath = UrlPath;
			UrlComponents.dwHostNameLength = UrlBufSize;
			UrlComponents.dwUrlPathLength = UrlBufSize;
			if (!WinHttpCrackUrl(Url.c_str(),
				0,
				0,
				&UrlComponents)){
				pImpl->Clear();
				throw BaseException(
					L"URL��͂Ɏ��s���܂���",
					Url.c_str(),
					L"HttpForm::Request()"
					);
			}

			wstring StrHost = UrlComponents.lpszHostName;
			wstring StrUrlPath = UrlComponents.lpszUrlPath;
			INTERNET_PORT NumPort = UrlComponents.nPort;
			// HTTP��HTTPS������ȊO��
			DWORD OpenRequestFlag = 0;
			if (UrlComponents.nScheme == INTERNET_SCHEME_HTTPS){
				OpenRequestFlag = WINHTTP_FLAG_SECURE;
			}
			wstring StrMethod;
			wstring StrHeader;
			//POST�{�f�B
			pImpl->m_StrBody = "";
			if (Post){
				StrMethod = L"POST";
				StrHeader = L"Content-Type: application/x-www-form-urlencoded\r\n";
				if (AllParams.size() > 0){
					Util::WStoMB(AllParams, pImpl->m_StrBody);
				}
			}
			else{
				StrMethod = L"GET";
				if (AllParams.size() > 0){
					StrUrlPath += AllParams;
				}
			}

			//HTTP�ڑ�
			pImpl->m_hConnect = WinHttpConnect(
				pImpl->m_hSession,
				StrHost.c_str(),
				NumPort,
				0);
			if (pImpl->m_hConnect == NULL){
				pImpl->Clear();
				throw BaseException(
					L"HTTP�ڑ��Ɏ��s���܂���",
					StrHost.c_str(),
					L"HttpForm::Request()"
					);
			}
			//HTTP���N�G�X�g�I�[�v��
			pImpl->m_hRequest = WinHttpOpenRequest(
				pImpl->m_hConnect,
				StrMethod.c_str(),
				StrUrlPath.c_str(),
				NULL,
				WINHTTP_NO_REFERER,
				WINHTTP_DEFAULT_ACCEPT_TYPES,
				OpenRequestFlag
				);
			if (pImpl->m_hRequest == NULL){
				pImpl->Clear();
				throw BaseException(
					L"HTTP���N�G�X�g�I�[�v���Ɏ��s���܂���",
					StrUrlPath.c_str(),
					L"HttpForm::Request()"
					);
			}

			//�R�[���o�b�N�֐��̎w��
			if ( 
				WinHttpSetStatusCallback(
					pImpl->m_hRequest,
					(WINHTTP_STATUS_CALLBACK)HttpFormRequestCallBack,
					WINHTTP_CALLBACK_FLAG_ALL_COMPLETIONS | WINHTTP_CALLBACK_FLAG_REDIRECT,
					NULL
				) == WINHTTP_INVALID_STATUS_CALLBACK)
			{
				pImpl->Clear();
				throw BaseException(
					L"�R�[���o�b�N�w��Ɏ��s���܂���",
					StrUrlPath.c_str(),
					L"HttpForm::Request()"
					);
			}
			///HTTP���N�G�X�g�̑��M
			if(
				!WinHttpSendRequest(
					pImpl->m_hRequest,
					StrHeader.c_str(),
					StrHeader.length(),
					(LPVOID)pImpl->m_StrBody.c_str(),
					pImpl->m_StrBody.length() * sizeof(char),
					pImpl->m_StrBody.length() * sizeof(char),
					(DWORD_PTR)this
				)
			)
			{
				pImpl->Clear();
				throw BaseException(
					L"HTTP���N�G�X�g�̑��M�Ɏ��s���܂���",
					StrUrlPath.c_str(),
					L"HttpForm::Request()"
					);
			}
		}
	}

	void HttpForm::OnCallback(DWORD InternetStatus, LPVOID pStatusInformation, DWORD StatusInformationLength){
		switch (InternetStatus)
		{
		case WINHTTP_CALLBACK_STATUS_SENDREQUEST_COMPLETE:
			if(!WinHttpReceiveResponse(pImpl->m_hRequest, NULL)){
				//�Z�b�V�����ȊO�̃N���A
				pImpl->Clear();
				OnResponseFailed(L"���N�G�X�g���M�����ł��܂���ł����B");
				return;
			}
			break;
		case WINHTTP_CALLBACK_STATUS_HEADERS_AVAILABLE:
		{
			//�w�b�_�̎擾
			DWORD dwStatusCode = 0;
			DWORD dwStatusCodeSize = sizeof(DWORD);
			if (!WinHttpQueryHeaders(pImpl->m_hRequest,
				WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,	// �X�e�[�^�X�R�[�h��DWORD�^�Ŏ擾����B
				WINHTTP_HEADER_NAME_BY_INDEX,	// �w�b�_�[���̃|�C���^
				&dwStatusCode,				// �o�b�t�@�[
				&dwStatusCodeSize,			// �o�b�t�@�[�T�C�Y
				WINHTTP_NO_HEADER_INDEX))		// �ŏ��ɔ��������w�b�_�[�̂ݎ��o��
			{
				//�Z�b�V�����ȊO�̃N���A
				pImpl->Clear();
				OnResponseFailed(L"�w�b�_�̎擾�Ɏ��s���܂����B");
				return;
			}
			if (HTTP_STATUS_OK != dwStatusCode)
			{
				//�Z�b�V�����ȊO�̃N���A
				pImpl->Clear();
				OnResponseFailed(L"�w�b�_�̃X�e�[�^�X���s���ł����B");
				return;
			}
			// ���X�|���X�f�[�^�f�[�^�₢���킹
			if (!WinHttpQueryDataAvailable(pImpl->m_hRequest, NULL))
			{
				//�Z�b�V�����ȊO�̃N���A
				pImpl->Clear();
				OnResponseFailed(L"�w�b�_�̃f�[�^�₢���킹�Ɏ��s���܂����B");
				return;
			}
			break;
		}
		case WINHTTP_CALLBACK_STATUS_DATA_AVAILABLE:
		{
			// �f�[�^�̎擾�\
			DWORD dwSize = *((LPDWORD)pStatusInformation);
			if (dwSize == 0)
			{
				//�Z�b�V�����ȊO�̃N���A
				pImpl->Clear();
				// �ǂݍ��ݏI��->�R�[���o�b�N�I��
				OnResponseSuccess();
				return;
			}

			// ���X�|���X�f�[�^�ǂݍ���
			DWORD dwLength = dwSize + 1;
			char* pBuffer = new char[dwLength * sizeof(char)];
			if (!WinHttpReadData(pImpl->m_hRequest,
				pBuffer,		// �o�b�t�@�[
				dwSize,			// �ǂݍ��ރo�C�g��
				NULL))
			{
				delete[](pBuffer);
				pBuffer = nullptr;
				//�Z�b�V�����ȊO�̃N���A
				pImpl->Clear();
				OnResponseFailed(L"�f�[�^�̓ǂݍ��݂Ɏ��s���܂����B");
				return;
			}
			break;
		}
		case WINHTTP_CALLBACK_STATUS_READ_COMPLETE:
		{
			//�ǂݍ���
			if (pStatusInformation && StatusInformationLength)
			{
				char* pBuf = (char*)pStatusInformation;
				//�Ō��\0������
				pBuf[StatusInformationLength] = '\0';
				pImpl->m_ReadData << pBuf;
				delete[](pBuf);
				// ���X�|���X�f�[�^�f�[�^�₢���킹
				if (!WinHttpQueryDataAvailable(pImpl->m_hRequest, NULL))
				{
					//�Z�b�V�����ȊO�̃N���A
					pImpl->Clear();
					OnResponseFailed(L"�f�[�^�u���b�N�̓ǂݍ��݊����Ɏ��s���܂����B");
					return;
				}
			}
			break;
		}
		case WINHTTP_CALLBACK_STATUS_REDIRECT:
			//���_�C���N�g�i���y�[�W�ɐ��ځj
			break;
		case WINHTTP_CALLBACK_STATUS_REQUEST_ERROR:
			OnResponseFailed(L"���N�G�X�g���M�Ɏ��s���܂����B");
			return;
		case WINHTTP_CALLBACK_STATUS_INTERMEDIATE_RESPONSE:
			//�Z�b�V�����ȊO�̃N���A
			pImpl->Clear();
			OnResponseFailed(L"���X�|���X��M�Ɏ��s���܂����B");
			break;
		default:
			//�Z�b�V�����ȊO�̃N���A
			pImpl->Clear();
			OnResponseFailed(L"�s��ȃG���[�ł��B");
			return;
		}
	}

	void HttpForm::GetResponseData(wstring& Ret, bool ClearFlg){
		if (pImpl->m_ReadData.str().size() > 0){
			Util::ConvertUtf8toWstring(pImpl->m_ReadData.str(),Ret);
			if (ClearFlg){
				pImpl->ReadDataClear();
			}
		}
		else{
			Ret = L"";
		}
	}

}
//end basedx11

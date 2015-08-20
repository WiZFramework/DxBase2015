/*!
@file HttpForm.h
@brief Http��GET/POST�AForm�I�u�W�F�N�g

@copyright Copyright (c) 2015 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/

#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class StaticText : public GameObject;
	//	�p�r: �\���p�e�L�X�g�iStringSprite�̃R���e�i�j
	//--------------------------------------------------------------------------------------
	class StaticText : public GameObject{
	public:
		//�\�z�Ɣj��
		explicit StaticText(const shared_ptr<Stage>& StagePtr);
		virtual ~StaticText();
		//�A�N�Z�T
		void SetFont(const wstring& FontName, float FontSize);
		StringSprite::TextAlignment GetTextAlignment() const;
		void SetTextAlignment(StringSprite::TextAlignment Alignment);

		const Color4& GetFontColor() const;
		void SetFontColor(const Color4& Col);
		const wstring& GetFontName() const;
		float GetFontSize() const;

		const Color4& GetBackColor() const;
		void SetBackColor(const Color4& Col);


		Point2D<float> GetBackTextMargin() const;
		void SetBackTextMargin(Point2D<float> p);

		Rect2D<float> GetTextRect() const;
		void SetTextRect(const Rect2D<float>& rect);

		const wstring& GetText() const;
		void SetText(const wstring& str);
		void AddText(const wstring& str);

		//������
		virtual void Create() override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};




	//--------------------------------------------------------------------------------------
	//	class InputObject : public GameObject;
	//	�p�r: ���̓I�u�W�F�N�g�̐e�N���X
	//--------------------------------------------------------------------------------------
	class InputObject : public GameObject{
	protected:
		//�\�z�Ɣj��
		InputObject(const shared_ptr<Stage>& StagePtr,
			const wstring& Name, const wstring& Value);
		virtual ~InputObject();
		const wstring& GetAttribute(const wstring& Key) const;
		void SetAttribute(const wstring& Key, const wstring& Val);
	public:
		//�A�N�Z�T(���z�֐�)
		virtual const wstring& GetName();
		virtual void SetName(const wstring& Str);
		virtual const wstring& GetValue();
		virtual void SetValue(const wstring& Str);
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class InputHidden : public InputObject;
	//	�p�r: �\���Ȃ��e�L�X�g
	//--------------------------------------------------------------------------------------
	class InputHidden : public InputObject{
	public:
		//�\�z�Ɣj��
		explicit InputHidden(const shared_ptr<Stage>& StagePtr,
			const wstring& Name, const wstring& Value);
		virtual ~InputHidden();
	};


	//--------------------------------------------------------------------------------------
	//	class InputText : public InputObject;
	//	�p�r: ���̓e�L�X�g
	//--------------------------------------------------------------------------------------
	class InputText : public InputObject{
	public:
		//�\�z�Ɣj��
		explicit InputText(const shared_ptr<Stage>& StagePtr,
			const wstring& Name, const wstring& Value);
		virtual ~InputText();
		//�A�N�Z�T
		bool IsFocus()const;
		bool GetFocus()const;
		void SetFocus(bool b);

		void SetFont(const wstring& FontName, float FontSize);
		StringSprite::TextAlignment GetTextAlignment() const;
		void SetTextAlignment(StringSprite::TextAlignment Alignment);

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


		Rect2D<float> GetTextRect() const;
		void SetTextRect(const Rect2D<float>& rect);

		//�A�N�Z�T(���z�֐�)
		virtual const wstring& GetValue();
		virtual void SetValue(const wstring& Str);
		//������
		virtual void Create() override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class HttpForm : public GameObject;
	//	�p�r: HttpForm�N���X
	//--------------------------------------------------------------------------------------
	class HttpForm : public GameObject{
		void SessionCreate();
		void Request(const wstring& Url, bool Post);
	protected:
		//�\�z�Ə���
		explicit HttpForm(const shared_ptr<Stage>& StagePtr, const wstring& GroupName);
		virtual ~HttpForm();
	public:
		//�A�N�Z�T
		const wstring& GetGroupName() const;
		//����
		shared_ptr<InputText> AddInputText(const wstring& Name, const wstring& Value);
		shared_ptr<InputHidden> AddInputHidden(const wstring& Name, const wstring& Value);
		void MoveFocusNextInput();

		void GetRequest(const wstring& Url);
		void PostRequest(const wstring& Url);

		void OnCallback(DWORD InternetStatus, LPVOID pStatusInformation, DWORD StatusInformationLength);
		void GetResponseData(wstring& Ret,bool ClearFlg = true);
		//�E�B���h�E���b�Z�[�W
		virtual void OnMessage(UINT message, WPARAM wParam, LPARAM lParam){}
		//���X�|���X���s�i�������z�֐��j
		virtual void OnResponseFailed(const wstring& MessageStr) = 0;
		//���X�|���X�����i�������z�֐��j
		virtual void OnResponseSuccess() = 0;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

}
//end basedx11

#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Sprite : public Component;
	//	�p�r: Sprite�`��R���|�[�l���g
	//�@��Sprite�͒��_�ύX����{�Ȃ̂ŁA�e�R���|�[�l���g�Ńo�b�t�@���쐬����
	//--------------------------------------------------------------------------------------
	class Sprite : public Component{
		void CalcSpriteCoordinate(Matrix4X4& Retmat);
	public:

		explicit Sprite(const shared_ptr<GameObject>& GameObjectPtr,
			const Color4& Color, const Vector2& Size = Vector2(1.0f,1.0f));
		virtual ~Sprite();
		//�A�N�Z�T
		//���W�n
		enum Coordinate{
			m_LeftTopZeroPlusDownY,		//���㌴�_��Y���������v���X�i�f�t�H���g�j
			m_LeftBottomZeroPlusUpY,	//�������_��Y��������v���X
			m_CenterZeroPlusUpY,		//�Z���^�[���_��Y��������v���X
		};
		Coordinate GetCoordinate()const;
		void SetCoordinate(Coordinate c);



		ComPtr<ID3D11Buffer>& GetVertexBuffer() const;
		vector<VertexPositionColorTexture>& GetBackupVertex() const;
		ComPtr<ID3D11Buffer>& GetIndexBuffer() const;
		UINT GetNumVertices() const;
		UINT GetNumIndicis() const;

		//���b�s���O���邩�ǂ���
		bool IsSamplerWrap() const;
		bool GetSamplerWrap() const;
		void SetSamplerWrap(bool b);

		//Z�o�b�t�@���g�����ǂ���
		bool IsZBufferUse() const;
		bool GetZBufferUse() const;
		void SetZBufferUse(bool b);

		bool IsAlphaBlendSrcOne()const;
		bool GetAlphaBlendSrcOne()const;
		void SetAlphaBlendSrcOne(bool b);


		//1���[�^������̃s�N�Z����
		float GetPixelParMeter() const;
		void SetPixelParMeter(float f);


		shared_ptr<TextureResource> GetTextureResource(bool ExceptionActive = true) const;
		void SetTextureResource(const wstring& ResKey);
		void SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr);
		//����
		virtual void Update()override{}
		virtual void Draw()override;
		void UpdateVirtexBuffer(const vector<VertexPositionColorTexture>& NewBuffer);
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Component;
	//	�p�r: StringSprite�R���|�[�l���g
	//	������\���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class StringSprite : public Component{
	public:
		explicit StringSprite(const shared_ptr<GameObject>& GameObjectPtr);
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

		bool IsFocus();
		void SetFocus(bool b);

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



}

//end basedx11

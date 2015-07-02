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
		enum TextAligment{
			m_Left,		//�����킹
			m_Center,	//�������킹
			m_Right		//�E���킹
		};
		//�A�N�Z�T
		void CreateFont(const wstring& FontName, float FontSize);
		StringSprite::TextAligment GetTextAligment() const;
		void SetTextAligment(StringSprite::TextAligment Aligment);
		const wstring& GetText() const;
		void SetText(const wstring& str);
		const Color4& GetFontColor() const;
		void SetFoneColor(const Color4& Col);
		const wstring& GetFontName() const;
		float GetFontSize() const;

		float GetTextBlockWidth() const;
		void SetTextBlockWidth(float f);
		float GetTextBlockHeight() const;
		void SetTextBlockHeight(float f);

		ComPtr<IDWriteTextLayout>& GetTextLayout()const;

		const Point2D<float>& GetStartPosition() const;
		void SetStartPosition(const Point2D<float>& pos);
		//����
		virtual void Update()override;
		virtual void Draw()override;
	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;
	};


	//--------------------------------------------------------------------------------------
	//	class InputStringSprite : publicStringSprite;
	//	�p�r: InputStringSprite�R���|�[�l���g
	//	���͕t������\���R���|�[�l���g
	//--------------------------------------------------------------------------------------
	class InputStringSprite : public StringSprite{
	public:
		explicit InputStringSprite(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~InputStringSprite();

		bool GetKeyInput() const;
		bool IsKeyInput() const;
		void SetKeyInput(bool b);

		bool IsFocus();
		void SetFocus(bool b);

		size_t GetCaret() const;
		void SetCaret(size_t CaretIndex);

		void AddText(const wstring& str);
		void InsertText(const wstring& str);
		void InsertText(const wstring& str, size_t CaretIndex);


		//����
		virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
		virtual void OnChar(WPARAM wParam, LPARAM lParam);

		//virtual void Update()override;
		virtual void Draw()override;

	private:
		// pImpl�C�f�B�I��
		struct Impl;
		unique_ptr<Impl> pImpl;

	};





}

//end basedx11

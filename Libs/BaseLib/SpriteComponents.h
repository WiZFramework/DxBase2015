#pragma once
#include "stdafx.h"

namespace basedx11{

	//--------------------------------------------------------------------------------------
	//	class Sprite : public Component;
	//	用途: Sprite描画コンポーネント
	//　＊Spriteは頂点変更が基本なので、各コンポーネントでバッファを作成する
	//--------------------------------------------------------------------------------------
	class Sprite : public Component{
		void CalcSpriteCoordinate(Matrix4X4& Retmat);
	public:

		explicit Sprite(const shared_ptr<GameObject>& GameObjectPtr,
			const Color4& Color, const Vector2& Size = Vector2(1.0f,1.0f));
		virtual ~Sprite();
		//アクセサ
		//座標系
		enum Coordinate{
			m_LeftTopZeroPlusDownY,		//左上原点でY軸下向きプラス（デフォルト）
			m_LeftBottomZeroPlusUpY,	//左下原点でY軸上向きプラス
			m_CenterZeroPlusUpY,		//センター原点でY軸上向きプラス
		};
		Coordinate GetCoordinate()const;
		void SetCoordinate(Coordinate c);



		ComPtr<ID3D11Buffer>& GetVertexBuffer() const;
		vector<VertexPositionColorTexture>& GetBackupVertex() const;
		ComPtr<ID3D11Buffer>& GetIndexBuffer() const;
		UINT GetNumVertices() const;
		UINT GetNumIndicis() const;

		//ラッピングするかどうか
		bool IsSamplerWrap() const;
		bool GetSamplerWrap() const;
		void SetSamplerWrap(bool b);

		//Zバッファを使うかどうか
		bool IsZBufferUse() const;
		bool GetZBufferUse() const;
		void SetZBufferUse(bool b);

		bool IsAlphaBlendSrcOne()const;
		bool GetAlphaBlendSrcOne()const;
		void SetAlphaBlendSrcOne(bool b);


		//1メータあたりのピクセル数
		float GetPixelParMeter() const;
		void SetPixelParMeter(float f);


		shared_ptr<TextureResource> GetTextureResource(bool ExceptionActive = true) const;
		void SetTextureResource(const wstring& ResKey);
		void SetTextureResource(const shared_ptr<TextureResource>& TextureResourcePtr);
		//操作
		virtual void Update()override{}
		virtual void Draw()override;
		void UpdateVirtexBuffer(const vector<VertexPositionColorTexture>& NewBuffer);
	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};

	//--------------------------------------------------------------------------------------
	//	class StringSprite : public Component;
	//	用途: StringSpriteコンポーネント
	//	文字列表示コンポーネント
	//--------------------------------------------------------------------------------------
	class StringSprite : public Component{
	public:
		explicit StringSprite(const shared_ptr<GameObject>& GameObjectPtr);
		virtual ~StringSprite();
		//アクセサ
		enum TextAlignment{
			m_Left,		//左合わせ
			m_Center,	//中央合わせ
			m_Right		//右合わせ
		};
		//アクセサ
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

		//操作
		virtual void OnKeyDown(WPARAM wParam, LPARAM lParam);
		virtual void OnChar(WPARAM wParam, LPARAM lParam);
		virtual void Update()override;
		virtual void Draw()override;

	private:
		// pImplイディオム
		struct Impl;
		unique_ptr<Impl> pImpl;
	};



}

//end basedx11

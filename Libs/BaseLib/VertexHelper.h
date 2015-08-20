
/******************************************************************************
 VertexHelper.h

 ���_��`�A�v���~�e�B�u�쐬�֐���

 Copyright (c) 2014 WiZ Tamura Hiroki,Yamanoi Yasushi.
 DirectXTK and DirectXTex are released under Microsoft Public License (Ms-PL).
********************************************************************************/

#pragma once

#include "StdAfx.h"

namespace basedx11{


    #if (DIRECTXMATH_VERSION < 305) && !defined(XM_CALLCONV)
    #define XM_CALLCONV __fastcall
    typedef const XMVECTOR& HXMVECTOR;
    typedef const XMMATRIX& FXMMATRIX;
    #endif

/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionLayout[];
 �p�r: �ʒu�������̓��C�A�E�g�̒�`
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct  VertexPosition;
 �p�r: �ʒu�������_�̒�`
****************************************************************************/
struct VertexPosition
{
	VertexPosition()
	{ }

	VertexPosition(float x,float y,float z)
		: position(XMFLOAT3(x,y,z))
	{ }


	VertexPosition(XMFLOAT3 const& position)
		: position(position)
	{ }

	VertexPosition(FXMVECTOR position)
	{
		XMStoreFloat3(&this->position, position);
	}

	XMFLOAT3 position;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionLayout );
	}
};


/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionColorLayout[];
 �p�r: �ʒu�ƐF�������̓��C�A�E�g�̒�`
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionColorLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct  VertexPositionColor;
 �p�r: �ʒu�ƐF�������_�̒�`
****************************************************************************/
struct VertexPositionColor
{
	VertexPositionColor()
	{ }

	VertexPositionColor(XMFLOAT3 const& position, XMFLOAT4 const& color)
		: position(position),
		color(color)
	{ }

	VertexPositionColor(FXMVECTOR position, FXMVECTOR color)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat4(&this->color, color);
	}

	XMFLOAT3 position;
	XMFLOAT4 color;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionColorLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionColorLayout );
	}
};

/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionTextureLayout[];
 �p�r: �ʒu�ƃe�N�X�`���������̓��C�A�E�g�̒�`
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionTextureLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct  VertexPositionTexture;
 �p�r: �ʒu�ƃe�N�X�`���������_�̒�`
****************************************************************************/
struct VertexPositionTexture
{
	VertexPositionTexture()
	{ }

	VertexPositionTexture(XMFLOAT3 const& position, XMFLOAT2 const& textureCoordinate)
		: position(position),
		textureCoordinate(textureCoordinate)
	{ }

	VertexPositionTexture(FXMVECTOR position, FXMVECTOR textureCoordinate)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
	}

	XMFLOAT3 position;
	XMFLOAT2 textureCoordinate;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionTextureLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionTextureLayout );
	}
};

/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalLayout[];
 �p�r: �ʒu�Ɩ@���������̓��C�A�E�g�̒�`
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct  VertexPositionNormal;
 �p�r: �ʒu�Ɩ@���������_�̒�`
****************************************************************************/
struct VertexPositionNormal
{
	VertexPositionNormal()
	{ }

	VertexPositionNormal(XMFLOAT3 const& position, XMFLOAT3 const& normal)
		: position(position),
		normal(normal)
	{ }

	VertexPositionNormal(FXMVECTOR position, FXMVECTOR normal)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal, normal);
	}

	XMFLOAT3 position;
	XMFLOAT3 normal;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionNormalLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionNormalLayout );
	}
};




/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionColorTextureLayout[];
 �p�r: �ʒu�ƐF�ƃe�N�X�`���������̓��C�A�E�g�̒�`
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionColorTextureLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct  VertexPositionColorTexture;
 �p�r: �ʒu�ƐF�ƃe�N�X�`���������_�̒�`
****************************************************************************/
struct VertexPositionColorTexture
{
	VertexPositionColorTexture()
	{ }

	VertexPositionColorTexture(XMFLOAT3 const& position, XMFLOAT4 const& color, XMFLOAT2 const& textureCoordinate)
		: position(position),
		color(color),
		textureCoordinate(textureCoordinate)
	{ }

	VertexPositionColorTexture(FXMVECTOR position, FXMVECTOR color, FXMVECTOR textureCoordinate)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat4(&this->color, color);
		XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
	}

	XMFLOAT3 position;
	XMFLOAT4 color;
	XMFLOAT2 textureCoordinate;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionColorTextureLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionColorTextureLayout );
	}
};

/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalColorLayout[];
 �p�r: �ʒu�Ɩ@���ƐF�������̓��C�A�E�g�̒�`
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalColorLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct VertexPositionNormalColor;
 �p�r: �ʒu�Ɩ@���ƐF�������_�̒�`
****************************************************************************/
struct VertexPositionNormalColor
{
	VertexPositionNormalColor()
	{ }

	VertexPositionNormalColor(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& color)
		: position(position),
		normal(normal),
		color(color)
	{ }

	VertexPositionNormalColor(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR color)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal, normal);
		XMStoreFloat4(&this->color, color);
	}

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 color;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionNormalColorLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionNormalColorLayout );
	}
};



/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureLayout[];
 �p�r: �ʒu�Ɩ@���ƃe�N�X�`���������̓��C�A�E�g�̒�`
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct VertexPositionNormalTexture;
 �p�r: �ʒu�Ɩ@���ƃe�N�X�`���������_�̒�`
****************************************************************************/
struct VertexPositionNormalTexture
{
	VertexPositionNormalTexture()
	{ }

	VertexPositionNormalTexture(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT2 const& textureCoordinate)
		: position(position),
		normal(normal),
		textureCoordinate(textureCoordinate)
	{ }

	VertexPositionNormalTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR textureCoordinate)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal, normal);
		XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
	}

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT2 textureCoordinate;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionNormalTextureLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionNormalTextureLayout );
	}
};


/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureSkinningLayout[];
 �p�r: �ʒu�Ɩ@���ƃe�N�X�`���ƃX�L�j���O���������̓��C�A�E�g�̒�`
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTextureSkinningLayout[] =
{
    { "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BLENDINDICES",0, DXGI_FORMAT_R32G32B32A32_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};
struct  VertexPositionNormalTextureSkinning{
    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 textureCoordinate;
	uint32_t indices[4];
	float weights[4];
	VertexPositionNormalTextureSkinning(){}
	
    VertexPositionNormalTextureSkinning(XMFLOAT3 const& pos, XMFLOAT3 const& nor, XMFLOAT2 const& texture,
		uint32_t* index,float* weight)
        : position(pos),
        normal(nor),
        textureCoordinate(texture)
    { 
		for(int i = 0;i < 4;i++){
			indices[i] = index[i];
			weights[i] = weight[i];
		}
	}
	
	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionNormalTextureSkinningLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionNormalTextureSkinningLayout );
	}
};



/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalColorTextureLayout[];
 �p�r: �ʒu�Ɩ@���ƐF�ƃe�N�X�`���������̓��C�A�E�g�̒�`
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalColorTextureLayout[] =
{
    { "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "COLOR",       0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    { "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct VertexPositionNormalColorTexture;
 �p�r: �ʒu�Ɩ@���ƐF�ƃe�N�X�`���������_�̒�`
****************************************************************************/
struct VertexPositionNormalColorTexture
{
	VertexPositionNormalColorTexture()
	{ }

	VertexPositionNormalColorTexture(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& color, XMFLOAT2 const& textureCoordinate)
		: position(position),
		normal(normal),
		color(color),
		textureCoordinate(textureCoordinate)
	{ }

	VertexPositionNormalColorTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR color, CXMVECTOR textureCoordinate)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal, normal);
		XMStoreFloat4(&this->color, color);
		XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
	}

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 color;
	XMFLOAT2 textureCoordinate;

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionNormalColorTextureLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionNormalColorTextureLayout );
	}
};

/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTangentColorTextureLayout[];
 �p�r: �ʒu�Ɩ@���ƃ^���W�F���g�ƐF�ƃe�N�X�`���������̓��C�A�E�g�̒�`
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTangentColorTextureLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",       0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct VertexPositionNormalTangentColorTexture;
 �p�r: �ʒu�Ɩ@���ƃ^���W�F���g�ƐF�ƃe�N�X�`���������_�̒�`
****************************************************************************/
struct VertexPositionNormalTangentColorTexture
{
	VertexPositionNormalTangentColorTexture()
	{ }

	XMFLOAT3 position;
	XMFLOAT3 normal;
	XMFLOAT4 tangent;
	uint32_t color;
	XMFLOAT2 textureCoordinate;

	VertexPositionNormalTangentColorTexture(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& tangent, uint32_t rgba, XMFLOAT2 const& textureCoordinate)
		: position(position),
		normal(normal),
		tangent(tangent),
		color(rgba),
		textureCoordinate(textureCoordinate)
	{
	}

	VertexPositionNormalTangentColorTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR tangent, uint32_t rgba, CXMVECTOR textureCoordinate)
		: color(rgba)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal, normal);
		XMStoreFloat4(&this->tangent, tangent);
		XMStoreFloat2(&this->textureCoordinate, textureCoordinate);
	}

	VertexPositionNormalTangentColorTexture(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& tangent, XMFLOAT4 const& color, XMFLOAT2 const& textureCoordinate)
		: position(position),
		normal(normal),
		tangent(tangent),
		textureCoordinate(textureCoordinate)
	{
		SetColor( color );
	}

	VertexPositionNormalTangentColorTexture(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR tangent, CXMVECTOR color, CXMVECTOR textureCoordinate)
	{
		XMStoreFloat3(&this->position, position);
		XMStoreFloat3(&this->normal, normal);
		XMStoreFloat4(&this->tangent, tangent);
		XMStoreFloat2(&this->textureCoordinate, textureCoordinate);

		SetColor( color );
	}

	void SetColor( XMFLOAT4 const& color ) { SetColor( XMLoadFloat4( &color ) ); }
	void XM_CALLCONV SetColor( FXMVECTOR color );

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionNormalTangentColorTextureLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionNormalTangentColorTextureLayout );
	}
};


/**************************************************************************
 const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTangentColorTextureSkinningLayout[];
 �p�r: �ʒu�Ɩ@���ƃ^���W�F���g�ƐF�ƃe�N�X�`���ƃX�L�j���O���������̓��C�A�E�g�̒�`
****************************************************************************/
const D3D11_INPUT_ELEMENT_DESC VertexPositionNormalTangentColorTextureSkinningLayout[] =
{
	{ "SV_Position", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "NORMAL",      0, DXGI_FORMAT_R32G32B32_FLOAT,    0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TANGENT",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "COLOR",       0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "TEXCOORD",    0, DXGI_FORMAT_R32G32_FLOAT,       0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDINDICES",0, DXGI_FORMAT_R8G8B8A8_UINT,      0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{ "BLENDWEIGHT", 0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
};

/**************************************************************************
 struct VertexPositionNormalTangentColorTextureSkinning : public VertexPositionNormalTangentColorTexture;
 �p�r: �ʒu�Ɩ@���ƃ^���W�F���g�ƐF�ƃe�N�X�`���ƃX�L�j���O���������_�̒�`
****************************************************************************/
struct VertexPositionNormalTangentColorTextureSkinning : public VertexPositionNormalTangentColorTexture
{
	VertexPositionNormalTangentColorTextureSkinning()
	{ }

	uint32_t indices;
	uint32_t weights;

	VertexPositionNormalTangentColorTextureSkinning(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& tangent, uint32_t rgba,
								XMFLOAT2 const& textureCoordinate, XMUINT4 const& indices, XMFLOAT4 const& weights)
		: VertexPositionNormalTangentColorTexture(position,normal,tangent,rgba,textureCoordinate)
	{
		SetBlendIndices( indices );
		SetBlendWeights( weights );
	}

	VertexPositionNormalTangentColorTextureSkinning(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR tangent, uint32_t rgba, CXMVECTOR textureCoordinate,
								XMUINT4 const& indices, CXMVECTOR weights)
		: VertexPositionNormalTangentColorTexture(position,normal,tangent,rgba,textureCoordinate)
	{
		SetBlendIndices( indices );
		SetBlendWeights( weights );
	}

	VertexPositionNormalTangentColorTextureSkinning(XMFLOAT3 const& position, XMFLOAT3 const& normal, XMFLOAT4 const& tangent, XMFLOAT4 const& color,
								XMFLOAT2 const& textureCoordinate, XMUINT4 const& indices, XMFLOAT4 const& weights)
		: VertexPositionNormalTangentColorTexture(position,normal,tangent,color,textureCoordinate)
	{
		SetBlendIndices( indices );
		SetBlendWeights( weights );
	}

	VertexPositionNormalTangentColorTextureSkinning(FXMVECTOR position, FXMVECTOR normal, FXMVECTOR tangent, CXMVECTOR color, CXMVECTOR textureCoordinate,
								XMUINT4 const& indices, CXMVECTOR weights)
		: VertexPositionNormalTangentColorTexture(position,normal,tangent,color,textureCoordinate)
	{
		SetBlendIndices( indices );
		SetBlendWeights( weights );
	}

	void SetBlendIndices( XMUINT4 const& indices );

	void SetBlendWeights( XMFLOAT4 const& weights ) { SetBlendWeights( XMLoadFloat4( &weights ) ); }
	void XM_CALLCONV SetBlendWeights( FXMVECTOR weights );

	static const D3D11_INPUT_ELEMENT_DESC* GetVertexElement(){
		return VertexPositionNormalTangentColorTextureSkinningLayout;
	}
	static UINT GetNumElements(){
		return  ARRAYSIZE( VertexPositionNormalTangentColorTextureSkinningLayout );
	}
};

/**************************************************************************
 class VertexUtil;
 �p�r: ���_�֘A�̃��[�e�B���e�B�֐��Q
 ��static�Ăяo��������
****************************************************************************/
class VertexUtil{
    static const float SQRT2;
    static const float SQRT3;
    static const float SQRT6;
/**************************************************************************
 �p�r:CreateGeoSphere�Ŏg�p�����w���p�[
***************************************************************************/
    static inline void CheckIndexOverflow(size_t value)
    {
        // Use >=, not > comparison, because some D3D level 9_x hardware does not support 0xFFFF index values.
        if (value >= USHRT_MAX){
			throw BaseException(
				L"�����I�[�o�[�t���[���܂���",
				L"if (value >= USHRT_MAX)",
				L"VertexUtil::CheckIndexOverflow()"
				);
		}
    }
/**************************************************************************
 �p�r: Cylinder / Cone�Ŏg�p�����w���p�[
***************************************************************************/
	static inline XMVECTOR GetCircleVector(size_t i, size_t tessellation)
	{
		float angle = i * XM_2PI / tessellation;
		float dx, dz;

		XMScalarSinCos(&dx, &dz, angle);

		XMVECTORF32 v = { dx, 0, dz, 0 };
		return v;
	}
	static inline XMVECTOR GetCircleTangent(size_t i, size_t tessellation)
	{
		float angle = ( i * XM_2PI / tessellation ) + XM_PIDIV2;
		float dx, dz;

		XMScalarSinCos(&dx, &dz, angle);

		XMVECTORF32 v = { dx, 0, dz, 0 };
		return v;
	}
	static void CreateCylinderCap(vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices,
			size_t tessellation, float height, float radius, bool isTop);
public:

/**************************************************************************
static void CreatePrimitiveBuffer(
	ID3D11Device* pDx11Device,	//�f�o�C�X
	const D3D11_BUFFER_DESC& bufferDesc,	//�o�b�t�@�f�X�N�v���^
	const D3D11_SUBRESOURCE_DATA& dataDesc,	//�f�[�^�f�X�N�v���^
	ID3D11Buffer** pBuffer		//���_�o�b�t�@
)
�p�r: �o�b�t�@���쐬����
�߂�l: �Ȃ�
***************************************************************************/
	static void CreatePrimitiveBuffer(ID3D11Device* pDx11Device, const D3D11_BUFFER_DESC* pBufferDesc, const D3D11_SUBRESOURCE_DATA* pDataDesc, ID3D11Buffer** pBuffer){
		if (FAILED(pDx11Device->CreateBuffer(pBufferDesc, pDataDesc, pBuffer))){
			// ���������s
			throw BaseException(
				L"�o�b�t�@�쐬�Ɏ��s���܂����B",
				L"if(FAILED(pDx11Device->CreateBuffer()))",
				L"VertexUtil::CreatePrimitiveBuffer()"
				);
		}
	}
/**************************************************************************
 template<typename T>
 static void CreatePrimitiveBuffer(
	ID3D11Device* pDx11Device,	//�f�o�C�X
	T const& data,				//�f�[�^�̔z��
	D3D11_BIND_FLAG bindFlags,	//�t���O
	ID3D11Buffer** pBuffer		//�o�b�t�@
	)
 �p�r: �o�b�t�@���쐬����
 �߂�l: �Ȃ�
***************************************************************************/
	template<typename T>
	static void CreatePrimitiveBuffer(ID3D11Device* pDx11Device, T const& data, D3D11_BIND_FLAG bindFlags, ID3D11Buffer** pBuffer){
		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = (UINT)data.size() * sizeof(T::value_type);
		bufferDesc.BindFlags = bindFlags;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA dataDesc = { 0 };
		dataDesc.pSysMem = &data.front();

		if(FAILED(pDx11Device->CreateBuffer(&bufferDesc, &dataDesc, pBuffer))){
			// ���������s
			throw BaseException(
				L"�o�b�t�@�쐬�Ɏ��s���܂����B",
				L"if(FAILED(pDx11Device->CreateBuffer()))",
				L"VertexUtil::CreatePrimitiveBuffer()"
				);
		}
	}


	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	static void CreateVertexBuffer(
	//		ComPtr<ID3D11Buffer>& VertexBuffer,	//�߂�o�b�t�@
	//		T const& data						//���_�f�[�^
	//	)
	//	�p�r: ���_�o�b�t�@���쐬����
	//	�߂�l: �Ȃ�
	//--------------------------------------------------------------------------------------
	template<typename T>
	static void CreateVertexBuffer(ComPtr<ID3D11Buffer>& VertexBuffer, T const& data){
		mutex Mutex;
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		Util::DemandCreate(VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
		{
			//���_�o�b�t�@�̍쐬
			VertexUtil::CreatePrimitiveBuffer(pDx11Device, data, D3D11_BIND_VERTEX_BUFFER, pResult);
		});
	}

	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	static void CreateIndexBuffer(
	//		ComPtr<ID3D11Buffer>& IndexBuffer,	//�߂�o�b�t�@
	//		T const& data						//�C���f�b�N�X�f�[�^
	//	)
	//	�p�r: �C���f�b�N�X�o�b�t�@���쐬����
	//	�߂�l: �Ȃ�
	//--------------------------------------------------------------------------------------
	template<typename T>
	static void CreateIndexBuffer(ComPtr<ID3D11Buffer>& IndexBuffer, T const& data){
		mutex Mutex;
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		Util::DemandCreate(IndexBuffer, Mutex, [&](ID3D11Buffer** pResult)
		{
			//�C���f�b�N�X�o�b�t�@�̍쐬
			VertexUtil::CreatePrimitiveBuffer(pDx11Device, data, D3D11_BIND_INDEX_BUFFER, pResult);
		});
	}



/**************************************************************************
 template<typename T>
 static void CreateDynamicPrimitiveVertexBuffer(
	ID3D11Device* pDx11Device,	//�f�o�C�X
	T const& data,				//�f�[�^�̔z��
	ID3D11Buffer** pBuffer		//�o�b�t�@
	)
 �p�r: ���_�f�[�^��ύX�ł��钸�_�o�b�t�@���쐬����
 �߂�l: �Ȃ�
***************************************************************************/
	template<typename T>
	static void CreateDynamicPrimitiveVertexBuffer(ID3D11Device* pDx11Device, T const& data, ID3D11Buffer** pBuffer){
		D3D11_BUFFER_DESC bufferDesc = { 0 };
		bufferDesc.ByteWidth = (UINT)data.size() * sizeof(T::value_type);
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		D3D11_SUBRESOURCE_DATA dataDesc = { 0 };
		dataDesc.pSysMem = &data.front();

		if(FAILED(pDx11Device->CreateBuffer(&bufferDesc, &dataDesc, pBuffer))){
			// ���������s
			throw BaseException(
			L"�o�b�t�@�쐬�Ɏ��s���܂����B",
			L"if(FAILED(pDx11Device->CreateBuffer()))",
			L"VertexUtil::CreateDynamicVertexBuffer()"
			);
		}
	}


	//--------------------------------------------------------------------------------------
	//	template<typename T>
	//	static void CreateDynamicPrimitiveVertexBuffer(
	//		ComPtr<ID3D11Buffer>& VertexBuffer,	//�߂�o�b�t�@
	//		T const& data						//���_�f�[�^
	//	)
	//	�p�r: ���_�f�[�^��ύX�ł��钸�_�o�b�t�@���쐬����
	//	�߂�l: �Ȃ�
	//--------------------------------------------------------------------------------------
	template<typename T>
	static void CreateDynamicVertexBuffer(ComPtr<ID3D11Buffer>& VertexBuffer, T const& data){
		mutex Mutex;
		auto Dev = App::GetApp()->GetDeviceResources();
		auto pDx11Device = Dev->GetD3DDevice();
		Util::DemandCreate(VertexBuffer, Mutex, [&](ID3D11Buffer** pResult)
		{
			//���_�o�b�t�@�̍쐬
			VertexUtil::CreateDynamicPrimitiveVertexBuffer(pDx11Device, data, pResult);
		});
	}



/**************************************************************************
 static void ReverseWinding(
	vector<uint16_t>& indices,
	vector<VertexPositionNormalTexture>& vertices
 )
 �p�r: ���_�ƃC���f�b�N�X�̌����𔽓]������iLH��RH�ϊ��j
 �߂�l: �Ȃ�
***************************************************************************/
    static void ReverseWinding(vector<uint16_t>& indices,vector<VertexPositionNormalTexture>& vertices)
    {
		if((indices.size() % 3) != 0){
			throw BaseException(
				L"�C���f�b�N�X�̐��������܂���",
				L"if((indices.size() % 3) == 0)",
				L"VertexUtil::ReverseWinding()"
				);
		}
        for( auto it = indices.begin(); it != indices.end(); it += 3 )
        {
            std::swap( *it, *(it+2) );
        }

        for( auto it = vertices.begin(); it != vertices.end(); ++it )
        {
			it->textureCoordinate.x = ( 1.f - it->textureCoordinate.x );
        }
    }
/**************************************************************************
 static  void CreateCube(
	float size,	//1�̕ӂ̃T�C�Y
	vector<VertexPositionNormalTexture>& vertices, //���_���쐬���邽�߂̔z��
	vector<uint16_t>& indices	//�C���f�b�N�X���쐬���邽�߂̔z��
 );
 �p�r: �����̂��쐬����(static�֐�)
 �߂�l: �Ȃ�
 ��VertexPosNormalTex�t�H�[�}�b�g�̗����̂��쐬����
***************************************************************************/
	static void CreateCube(float size,
		 vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
  static void CreateSphere(
	float diameter,	//���a�T�C�Y
	size_t tessellation,	//����
	vector<VertexPositionNormalTexture>& vertices, //���_���쐬���邽�߂̔z��
	vector<uint16_t>& indices	//�C���f�b�N�X���쐬���邽�߂̔z��
 );
 �p�r: ���̂��쐬����
 �߂�l: �Ȃ�
 ��VertexPosNormalTex�t�H�[�}�b�g�̋��̂��쐬����
***************************************************************************/
	static void CreateSphere(float diameter,size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
  static void CreateCapsule(
	float diameter,				//���a�T�C�Y
	const Vector3& PointA,		//���ԕ������̊J�n�_
	const Vector3& PointB,		//���ԕ������̏I���_
	size_t tessellation,	//����
	vector<VertexPositionNormalTexture>& vertices, //���_���쐬���邽�߂̔z��
	vector<uint16_t>& indices	//�C���f�b�N�X���쐬���邽�߂̔z��
 );
 �p�r: �J�v�Z�����쐬����
 �߂�l: �Ȃ�
 ��VertexPosNormalTex�t�H�[�}�b�g�̋��̂��쐬����
***************************************************************************/
	static void CreateCapsule(float diameter,
		const Vector3& PointA,const Vector3& PointB,
		size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
  static void CreateGeoSphere(
	float diameter,	//���a�T�C�Y
	size_t tessellation,	//����
	vector<VertexPositionNormalTexture>& vertices, //���_���쐬���邽�߂̔z��
	vector<uint16_t>& indices	//�C���f�b�N�X���쐬���邽�߂̔z��
 );
 �p�r: �W�I���g���b�N���̂��쐬����
 �߂�l: �Ȃ�
 ��VertexPosNormalTex�t�H�[�}�b�g�̃W�I���g���b�N���̂��쐬����
***************************************************************************/
	static void CreateGeoSphere(float diameter, size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
 static void CreateCylinder(
	float height,	//����
	float diameter,	//���a�T�C�Y
	size_t tessellation,	//����
	vector<VertexPositionNormalTexture>& vertices,	//���_���쐬���邽�߂̔z��
	vector<uint16_t>& indices	//�C���f�b�N�X���쐬���邽�߂̔z��
	);
 �p�r: �V�����_�[���쐬����
 �߂�l: �Ȃ�
 ��VertexPosNormalTex�t�H�[�}�b�g�̃V�����_�[���쐬����
***************************************************************************/
	static void CreateCylinder(float height,float diameter,size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
 static void CreateCone(
	float diameter,	//���a�T�C�Y
	float height,	//����
	size_t tessellation,	//������
	vector<VertexPositionNormalTexture>& vertices,	//���_���쐬���邽�߂̔z��
	vector<uint16_t>& indices	//�C���f�b�N�X���쐬���邽�߂̔z��
	)
 �p�r: �R�[�����쐬����
 �߂�l: �Ȃ�
 ��VertexPosNormalTex�t�H�[�}�b�g�̃R�[�����쐬����
***************************************************************************/
	static void CreateCone(float diameter, float height, size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
 static void CreateTorus(
	float diameter,			//�h�[�i�b�c���S�܂ł̒��a
	float thickness,		//�h�[�i�b�c����
	size_t tessellation,	//����
	vector<VertexPositionNormalTexture>& vertices,	//���_���쐬���邽�߂̔z��
	vector<uint16_t>& indices	//�C���f�b�N�X���쐬���邽�߂̔z��
	);
 �p�r: �g�[���X���쐬����
 �߂�l: �Ȃ�
 ��VertexPosNormalTex�t�H�[�}�b�g�̃g�[���X���쐬����
***************************************************************************/
	static void CreateTorus(float diameter, float thickness, size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
 static void CreateTetrahedron(
	float size,		//�T�C�Y
	vector<VertexPositionNormalTexture>& vertices,	//���_���쐬���邽�߂̔z��
	vector<uint16_t>& indices	//�C���f�b�N�X���쐬���邽�߂̔z��
	);
 �p�r: ��4�ʑ̂��쐬����
 �߂�l: �Ȃ�
 ��VertexPosNormalTex�t�H�[�}�b�g�̐�4�ʑ̂��쐬����
***************************************************************************/
	static void CreateTetrahedron(float size,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
 static void CreateOctahedron(
	float size,	//�T�C�Y
	vector<VertexPositionNormalTexture>& vertices,	//���_���쐬���邽�߂̔z��
	vector<uint16_t>& indices	//�C���f�b�N�X���쐬���邽�߂̔z��
	);
 �p�r: ��8�ʑ̂��쐬����
 �߂�l: �Ȃ�
 ��VertexPosNormalTex�t�H�[�}�b�g�̐�8�ʑ̂��쐬����
***************************************************************************/
	static void CreateOctahedron(float size,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
 static void CreateDodecahedron(
	float size,	//�T�C�Y
	vector<VertexPositionNormalTexture>& vertices,	//���_���쐬���邽�߂̔z��
	vector<uint16_t>& indices	//�C���f�b�N�X���쐬���邽�߂̔z��
	);
 �p�r: ��12�ʑ̂��쐬����
 �߂�l: �Ȃ�
 ��VertexPosNormalTex�t�H�[�}�b�g�̐�12�ʑ̂��쐬����
***************************************************************************/
	static void CreateDodecahedron(float size,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);
/**************************************************************************
 static void CreateIcosahedron(
	float size,	//�T�C�Y
	vector<VertexPositionNormalTexture>& vertices,	//���_���쐬���邽�߂̔z��
	vector<uint16_t>& indices	//�C���f�b�N�X���쐬���邽�߂̔z��
	);
 �p�r: ��20�ʑ̂��쐬����
 �߂�l: �Ȃ�
 ��VertexPosNormalTex�t�H�[�}�b�g�̐�20�ʑ̂��쐬����
***************************************************************************/
	static void CreateIcosahedron(float size,
		vector<VertexPositionNormalTexture>& vertices,vector<uint16_t>& indices);

};


}
//end of namespace basedx11.

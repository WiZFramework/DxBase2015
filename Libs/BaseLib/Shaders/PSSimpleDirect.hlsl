//--------------------------------------------------------------------------------------
// File: PSSimpleDirect.hlsl
//
//--------------------------------------------------------------------------------------
cbuffer SimpleDirectConstantBuffer : register(b0)
{
	float4 diffuseColor;
};



//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main( float4 Pos : SV_POSITION ) : SV_Target
{
	return diffuseColor;
}

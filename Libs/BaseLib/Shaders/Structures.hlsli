
// Vertex shader input structures.
struct VSInputNmTx
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD0;
};

struct VSInputNmTxWeights
{
    float4 Position : SV_Position;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD0;
    uint4  Indices  : BLENDINDICES0;
    float4 Weights  : BLENDWEIGHT0;
};

// Vertex shader output structures.
struct VSOutputTx
{
	float4 PositionPS : SV_Position;
	float4 Diffuse    : COLOR0;
    float4 Specular   : COLOR1;
    float2 TexCoord   : TEXCOORD0;
};
//シャドウ付
struct VSOutputTxShadow
{
	float4 PositionPS : SV_Position;
	float4 Diffuse    : COLOR0;
	float4 Specular   : COLOR1;
	float2 TexCoord   : TEXCOORD0;
	float4 posLWVP  : COLOR2;
};

// Pixel shader input structures.
//シャドウ付
struct PSInputTxShadow
{
	float4 PositionPS : SV_Position;
	float4 Diffuse  : COLOR0;
	float4 Specular : COLOR1;
	float2 TexCoord : TEXCOORD0;
	float4 posLWVP  : COLOR2;
};

struct PixelShaderInputShadow
{
	float4 PositionPS : SV_Position;
	float4 Diffuse  : COLOR0;
	float4 Specular : COLOR1;
	float4 posLWVP  : COLOR2;
	float2 TexCoord : TEXCOORD0;
	float4 lightSpacePos : POSITION1;
	float3 Normal   : NORMAL0;
	float3 lRay		: NORMAL1;
	float3 View : NORMAL2;
};


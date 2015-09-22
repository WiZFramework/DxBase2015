//--------------------------------------------------------------------------------------
// File: PSTexture3D.hlsl
//
//--------------------------------------------------------------------------------------
#include "PRTexture3D.hlsli"

Texture2D<float4> SimpleTexture : register(t0);
SamplerState SimpleSampler : register(s0);


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 main(PixelShaderInput input) : SV_TARGET
{
	//���C�g�̌����𓾂�
	float3 LightDirection = normalize(float3(LightDir.xyz));
	float3 InputNormal = normalize(input.Normal);
	//���C�g�ɂ��s�N�Z���̐F������
	float4 LightColor = saturate(dot(InputNormal, LightDirection) + float4(0.7, 0.7, 0.7, 1.0));
	//���C�g�̓��������͂Ȃ�
	LightColor.a = 1;
	//�e�N�X�`���ƍ��킹��
	float4 TextureColor = SimpleTexture.Sample(SimpleSampler, input.TexCoord);
	//�e�N�X�`���F�ƃ��C�g�F������
	return TextureColor *LightColor;
}


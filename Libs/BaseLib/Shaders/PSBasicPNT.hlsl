//--------------------------------------------------------------------------------------
// File: PSBasicPNT.hlsl
//
//--------------------------------------------------------------------------------------

Texture2D<float4> Texture : register(t0);
// 深度マップ
Texture2D g_DepthMap : register(t1);

SamplerState Sampler : register(s0);
SamplerState g_SamplerDepthMap : register(s1);

#include "BasicParameters.hlsli"
#include "Structures.hlsli"
#include "Common.hlsli"
#include "Lighting.hlsli"

//ターゲット
// Pixel shader: vertex lighting + texture, no fog.
float4 main(PixelShaderInputShadow pin) : SV_Target0
{
	float shadowColor = 1.0f;
	if (activeflags.y == 1){
		// テクセルを計算
		float2 texel = float2(pin.posLWVP.x / pin.posLWVP.w * 0.5f + 0.5f, pin.posLWVP.y / pin.posLWVP.w * -0.5f + 0.5f);
		float depth = 0;
		float sm = 0;
		depth = pin.posLWVP.z / pin.posLWVP.w - 0.00005f;
		sm = g_DepthMap.Sample(g_SamplerDepthMap, texel).x;
		if (texel.x >= 0 && texel.x <= 1 && texel.y >= 0 && texel.y <= 1){
			// 現在の深度値と深度マップ上の深度値を比較
			if (sm > depth){
				shadowColor = 1.0f;
			}
			else{
				shadowColor = 0.6f;
			}
		}
	}
	float4 color = 0;
	if (activeflags.x == 1){
		if (activeflags.w == 1){
			//テクスチャオンリー
			//テクスチャと合わせない
			color = Texture.Sample(Sampler, pin.TexCoord);
		}
		else{
			//テクスチャと合わせる
			color = Texture.Sample(Sampler, pin.TexCoord) *pin.Diffuse;
		}
	}
	else{
		color = pin.Diffuse;
	}
	color.rgb = color.rgb * shadowColor;
	AddSpecular(color, pin.Specular.rgb);
	return color;
}


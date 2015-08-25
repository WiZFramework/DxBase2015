

#include "SimpleInc.hlsli"



[maxvertexcount(9)]
void main(
	triangle GeometryShaderInput input[3],
	inout TriangleStream< PixelShaderInput > output
)
{
	for (uint count = 0; count < 3; count++){
		for (uint i = 0; i < 3; i++)
		{
			PixelShaderInput element;
			//���_�̈ʒu��ϊ�
			float4 pos = float4(input[i].pos.xyz, 1.0f);
			float4 Col;
			if (pos.y > 0){
				switch (count){
				case 0:
					Col = float4(1.0f, 0.0f, 0.0f, 0.0f);
					break;
				case 1:
					Col = float4(0.0f, 1.0f, 0.0f, 0.0f);
					break;
				case 2:
					Col = float4(0.0f, 0.0f, 1.0f, 0.0f);
					break;
				default:
					Col = float4(1.0f, 1.0f, 1.0f, 1.0f);
					break;
				}
			}
			else if (pos.y < 0){
				switch (count){
				case 0:
					Col = float4(0.0f, 1.0f, 0.0f, 0.0f);
					break;
				case 1:
					Col = float4(0.0f, 0.0f, 1.0f, 0.0f);
					break;
				case 2:
					Col = float4(1.0f, 0.0f, 0.0f, 0.0f);
					break;
				default:
					Col = float4(1.0f, 1.0f, 1.0f, 1.0f);
					break;
				}
			}
			else{
				Col = float4(1.0f, 1.0f, 1.0f, 1.0f);
			}
			//���[���h�ϊ�
			pos = mul(pos, World[count]);
			//�r���[�ϊ�
			pos = mul(pos, View);
			//�ˉe�ϊ�
			pos = mul(pos, Projection);
			//�s�N�Z���V�F�[�_�ɓn���ϐ��ɐݒ�
			element.pos = pos;
			//���C�e�B���O�p�ɖ@�������[���h�ϊ����Đݒ�
			element.norm = mul(input[i].norm, (float3x3)World[count]);
			//���_�F��ݒ�
			element.color = Col;
			//���_��ǉ�
			output.Append(element);
		}
		//��̎O�p�`���X�g���[���ɑ���
		output.RestartStrip();
	}
}

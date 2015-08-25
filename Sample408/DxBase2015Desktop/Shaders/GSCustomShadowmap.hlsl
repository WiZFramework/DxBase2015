
#include "CustomShadowmapInc.hlsli"

[maxvertexcount(9)]
void main(
	triangle GeometryShaderInput input[3],
	inout TriangleStream< GeometryShaderOutput > output
	)
{
	for (uint count = 0; count < 3; count++){
		for (uint i = 0; i < 3; i++)
		{
			GeometryShaderOutput element;
			element.pos = input[i].pos;
			element.pos.w = 1.0f;
			element.pos = mul(input[i].pos, World[count]);
			element.pos = mul(element.pos, View);
			element.pos = mul(element.pos, Projection);
			//���_��ǉ�
			output.Append(element);
		}
		//��̎O�p�`���X�g���[���ɑ���
		output.RestartStrip();
	}
}

#include "common.hlsl"

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

//=============================================================================
// ���_�V�F�[�_
//=============================================================================
void main( in  float4 inPosition	: POSITION0,	//���_���W
		   in  float4 inNormal		: NORMAL0,		//���_�@��
		   in  float4 inDiffuse		: COLOR0,		//���_�J���[
		   in  float2 inTexCoord	: TEXCOORD0,	//�e�N�X�`�����W

		   out float4 outPosition	: SV_POSITION,	//�ϊ��㒸�_���W
		   out float4 outNormal		: NORMAL0,		//���_�@��
		   out float4 outDiffuse	: COLOR0,		//���_�J���[
		   out float2 outTexCoord	: TEXCOORD0		 //�e�N�X�`�����W
		)
{
	//���_���W��WVP�ϊ����ďo��
	outPosition = mul( inPosition, WorldViewProjection );
	//�e�N�X�`�����W���o��
	outTexCoord = inTexCoord;
	
	//�@�������[���h�ϊ�
    float4 worldNormal, normal;
    normal = float4(inNormal.x, inNormal.y, inNormal.z, 0.0f);
    worldNormal = mul(normal, World);
	//�@���𐳋K��
    worldNormal = normalize(worldNormal);
	
	
	//��]�ςݖ@�����o��
	outNormal =	worldNormal;
	
	
	//�����v�Z
    float light = -dot(worldNormal, Light.Direction);
    light = saturate(light);//���邳���߂�
	
	
	//���_�J���[�ƃ}�e���A���F���������ďo��
	outDiffuse.rgb = inDiffuse.rgb *  light;
	
    outDiffuse.rgb += Light.Ambient.rgb; //���������Z
	
	//���l�͕ʌv�Z
	outDiffuse.a = inDiffuse.a ;
	
    

}



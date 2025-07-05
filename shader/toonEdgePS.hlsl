
#include "common.hlsl"


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D		g_Texture : register( t0 );		//�e�N�X�`���ϐ�
SamplerState	g_SamplerState : register( s0 );//�T���v���[�ϐ�

//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
void main( in float4 inPosition : SV_POSITION,     //�ϊ��㒸�_���W
		   in float4 inWorldPosition : POSITION0,  //���[���h�ϊ��㒸�_���W,�|�W�V�����[��
                     
                     
		   in float4 inNormal : NORMAL0,           //���_�@��
		   in float4 inDiffuse : COLOR0,           //���_�J���[
		   in float2 inTexCoord : TEXCOORD0,       //�e�N�X�`�����W

   		   out float4 outDiffuse	: SV_Target )	//�ŏI�I�ȃs�N�Z���̃J���[
{
	
    outDiffuse = inDiffuse; //���_�J���[�����̂܂܏o��
	
}

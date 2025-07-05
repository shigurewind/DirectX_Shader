
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
	
	//�s�N�Z���̖@���𐳋K��
    float4 normal = normalize(inNormal);
	// lv = ��������s�N�Z���ւ̃x�N�g�� �@�@����--------->�s�N�Z��
    float4 lv = inWorldPosition - Light.Position;
	// ld = �����ƃs�N�Z���̋���	
    float4 ld = length(lv); //ld.x ld.y ld.z ld.w �ɒ���������
	// ofs = �����l�̌v�Z
    float ofs = 1.0f - (1.0f / Light.PointLightParam.x) * ld;
    ofs = max(0, ofs); //0������0�ɂ���@ofs=saturate(ofs)=>ofs��0�`1�̒l�֕␳

	//�����v�Z
    float light = -dot(normal.xyz, normalize(lv.xyz));
    light = saturate(light); //0.0f������0.0�A1.0f�ȏ��1.0f�ɂ���
	//���邳������������
    light *= ofs;
	
	//�Z���V�F�[�f�B���O
    if (light >= 0.7f)
    {
		light = 1.0f; //���邢�����͂��̂܂�
	}
	else if (light >= 0.4f)
    {
        light = 0.7f; //���Ԃ̖��邳��0.7�ɂ���
    }else
    {
        light = 0.4f; //�Â�������0.4�ɂ���
    }

	//�e�N�X�`���̐F���擾
        outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);
	//�F�Ɩ��邳������
    outDiffuse.rgb *= inDiffuse.rgb * Light.Diffuse.rgb * light;
    outDiffuse.rgb += Light.Ambient.rgb;
	//���͕ʏ���
    outDiffuse.a *= inDiffuse.a;
	

	//======================================
    //�G�b�W�ǉ�
    
    //�����x�N�g��
    float3 eyev = inWorldPosition.xyz - CameraPosition.xyz; //�J�����ʒu����s�N�Z���ւ̃x�N�g��
    eyev = normalize(eyev); //���K��
    //�G�b�W����
    float d = dot(inNormal.xyz, eyev); //�@���Ǝ����x�N�g���̓���)
    if(d > -0.45f)
    {
        outDiffuse.rgb *= 0.3f; //�G�b�W�����͐F���Â�����
    }
	
	
}

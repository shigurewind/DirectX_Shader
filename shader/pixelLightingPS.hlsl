
#include "common.hlsl"


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D		g_Texture : register( t0 );		//�e�N�X�`���ϐ�
SamplerState	g_SamplerState : register( s0 );//�T���v���[�ϐ�

//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
void main(
           in float4 inPosition : SV_POSITION, //�ϊ��㒸�_���W
		   in float4 inWorldPosition : POSITION0, //���[���h�ϊ��㒸�_���W,�|�W�V�����[��
                     
                     
		   in float4 inNormal : NORMAL0, //���_�@��
		   in float4 inDiffuse : COLOR0, //���_�J���[
		   in float2 inTexCoord : TEXCOORD0, //�e�N�X�`�����W

   		   out float4 outDiffuse	: SV_Target )	//�ŏI�I�ȃs�N�Z���̃J���[
{
	
	//�s�N�Z���̖@���𐳋K��
    float4 normal = normalize(inNormal);

    //�����v�Z
    float light = -dot(normal.xyz, Light.Direction.xyz);
    light = saturate(light); //0.0f�`1.0f�ɐ���
	
	//�e�N�X�`���̐F���擾
    outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);
	
	//�F�Ɩ��邳������
    outDiffuse.rgb *= inDiffuse.rgb * Light.Diffuse.rgb * light; //���_�J���[����Z
    outDiffuse.rgb += Light.Ambient.rgb; //���������Z
    
	//�X�y�L�����v�Z(���ʔ���)
    //float3 viewDir = normalize(-inPosition.xyz); //�J��������
    //float3 reflectDir = reflect(Light.Direction.xyz, normal.xyz); //���˃x�N�g��
    //float specular = pow(saturate(dot(viewDir, reflectDir)), MaterialBuffer.Shininess);
    //outDiffuse.rgb += specular * MaterialBuffer.Specular.rgb; //�X�y�L���������Z
    
    
	//���͕ʏ���
    outDiffuse.a *= inDiffuse.a; //���_�J���[�̃��l����Z
    
    //------------------
    //�J��������̎����x�N�g��
    float3 cameraView = inPosition.xyz - CameraPosition.xyz; 
    cameraView = normalize(cameraView); //���K��
    
    //���̔��˃x�N�g���v�Z
    float3 reflectDir = reflect(normalize(Light.Direction.xyz), normal.xyz); //���˃x�N�g��
    reflectDir = normalize(reflectDir); //���K��
    
    //���ʔ��˂̌v�Z
    float specular = -dot(cameraView, reflectDir); //�J���������Ɣ��˃x�N�g���̓���
    specular = saturate(specular); //0.0f�`1.0f�ɐ���
    //specular = pow(specular, Shininess); //�V���C�j�l�X�Œ���
    specular = pow(specular, 30.0f); //�V���C�j�l�X�Œ���
    
    outDiffuse.rgb += specular; //�X�y�L���������Z
	
}

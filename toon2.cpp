/*==============================================================================

   ���_�Ǘ� [polygon3D.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "toon2.h"
#include "sprite.h"
#include "Camera.h"
#include "texture.h"
#include "model.h"

#include "keyboard.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************


//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
//�J�����͕�������ƃ_���Ȃ̂ŊO��
static	Toon2	g_Object2;		//3D�|���S���I�u�W�F�N�g�\����
static	ID3D11VertexShader* g_VertexShader;	//���_�V�F�[�_�[�I�u�W�F�N�g�|�C���^
static	ID3D11PixelShader* g_PixelShader;	//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�|�C���^
static	ID3D11InputLayout* g_VertexLayout;	//���_���C�A�E�g�I�u�W�F�N�g�|�C���^

static	ID3D11VertexShader* g_VertexShaderEdge;	//���_�V�F�[�_�[�I�u�W�F�N�g�|�C���^
static	ID3D11PixelShader* g_PixelShaderEdge;	//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�|�C���^
static	ID3D11InputLayout* g_VertexLayoutEdge;	//���_���C�A�E�g�I�u�W�F�N�g�|�C���^

static	MODEL* PlayerModel;


static LIGHT Light; //���C�g�I�u�W�F�N�g

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPolygonToon2(void)
{

	//�e�N�X�`���ǂݍ���
	//int Texture = TextureLoad(L"asset\\texture\\Toon.bmp");
	int Texture = TextureLoad(L"asset\\texture\\toon.png");
	g_Object2.TexID = Texture; //�e�N�X�`��ID��ݒ�


	//�V�F�[�_�[�ǂݍ���
	CreateVertexShader(&g_VertexShader, &g_VertexLayout, "toon2VS.cso");
	CreatePixelShader(&g_PixelShader, "toon2PS.cso");

	CreateVertexShader(&g_VertexShaderEdge, &g_VertexLayoutEdge, "toonEdgeVS.cso");
	CreatePixelShader(&g_PixelShaderEdge, "toonEdgePS.cso");

	//3D�I�u�W�F�N�g�Ǘ��\���̂̏�����
	g_Object2.Position = XMFLOAT3(-0.7f, 0.6f, 1.0f);
	g_Object2.Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Object2.Scale = XMFLOAT3(0.2f, 0.2f, 0.2f);



	//���f���ǂݍ���
	PlayerModel = ModelLoad("asset\\model\\model.fbx");

	//���C�g�\���̂̏�����
	Light.Enable = true;
	Light.Direction = XMFLOAT4(1.0f, -1.0f, 0.0f, 0.0f); //���̕���
	XMVECTOR temp = XMLoadFloat4(&Light.Direction);
	temp = XMVector4Normalize(temp); //���K��
	XMStoreFloat4(&Light.Direction, temp); //���K�������l��߂�

	Light.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.2f, 1.0f); //���F
	Light.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f); //�������F

	Light.Position = XMFLOAT4(0.5f, 1.5f, 0.0f, 1.0f); //���C�g�̈ʒu
	Light.PointLightParam = XMFLOAT4(10.0f, 0.0f, 0.0f, 0.0f); //x�͌��͂�����


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void FinalizePolygonToon2(void)
{
	//��������̂����
	g_VertexShader->Release();
	g_VertexLayout->Release();
	g_PixelShader->Release();

	g_VertexShaderEdge->Release();
	g_PixelShaderEdge->Release();
	g_VertexLayoutEdge->Release();

	ModelRelease(PlayerModel);

}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePolygonToon2(void)
{

	if (Keyboard_IsKeyDown(KK_LEFT))
	{
		g_Object2.Rotate.y += 1.5f;
	}
	else if (Keyboard_IsKeyDown(KK_RIGHT))
	{
		g_Object2.Rotate.y += -1.5f;
	}

	if (Keyboard_IsKeyDown(KK_UP))
	{
		g_Object2.Rotate.x += 1.5f;
	}
	else if (Keyboard_IsKeyDown(KK_DOWN))
	{
		g_Object2.Rotate.x += -1.5f;
	}

	//�ړ�����
	if (Keyboard_IsKeyDown(KK_A))
	{
		g_Object2.Position.x += -0.02f;
	}
	else if (Keyboard_IsKeyDown(KK_D))
	{
		g_Object2.Position.x += 0.02f;
	}

	if (Keyboard_IsKeyDown(KK_W))
	{
		g_Object2.Position.y += 0.02f;
	}
	else if (Keyboard_IsKeyDown(KK_S))
	{
		g_Object2.Position.y += -0.02f;
	}


	//������Shader�e�N�X�`�����W�ύX
	Light.PointLightParam.z += 0.5f/60.0f; //���̋�����ς���
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPolygonToon2(void)
{
	//���_���C�A�E�g��ݒ�
	GetDeviceContext()->IASetInputLayout(g_VertexLayout);
	//���_�V�F�[�_�[���Z�b�g
	GetDeviceContext()->VSSetShader(g_VertexShader, NULL, 0);
	//�s�N�Z���V�F�[�_�[���Z�b�g
	GetDeviceContext()->PSSetShader(g_PixelShader, NULL, 0);

	//3D�p�s��ݒ�
	//ResetWorldViewProjection3D();//3D�`��ɐ؂�ւ���
	SetDepthEnable(true);		//���s�������L��

	//�J�����͋��L�̏�񂪐ݒ肳��Ă���Ƃ���

	{//3D�|���S���P���̏���
		//�e�N�X�`�����Z�b�g
		/*ID3D11ShaderResourceView* tex = GetTexture(g_Object2.TexID);
		GetDeviceContext()->PSSetShaderResources(0, 1, &tex);*/

		//���s�ړ��s��쐬
		XMMATRIX	TranslationMatrix =
			XMMatrixTranslation(
				g_Object2.Position.x,
				g_Object2.Position.y,
				g_Object2.Position.z
			);
		//��]�s��쐬
		XMMATRIX	RotationMatrix =
			XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_Object2.Rotate.x),
				XMConvertToRadians(g_Object2.Rotate.y),
				XMConvertToRadians(g_Object2.Rotate.z)
			);
		//�X�P�[�����O�s��쐬
		XMMATRIX	ScalingMatrix =
			XMMatrixScaling(
				g_Object2.Scale.x,
				g_Object2.Scale.y,
				g_Object2.Scale.z
			);
		//���[���h�s��쐬 ����Z�̏��Ԃɒ���
		XMMATRIX	WorldMatrix =
			ScalingMatrix *
			RotationMatrix *
			TranslationMatrix;
		//DirectX�փZ�b�g
		SetWorldMatrix(WorldMatrix);

		//�v���~�e�B�u�g�|���W�[�̐ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		);
		//�}�e���A���ݒ�
		MATERIAL	material;
		ZeroMemory(&material, sizeof(MATERIAL));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);


		//���C�g�ݒ�
		SetLight(Light);

		//�e�N�X�`���Z�b�g
		ID3D11ShaderResourceView* tex = GetTexture(g_Object2.TexID);
		GetDeviceContext()->PSSetShaderResources(1, 1, &tex);

		SetCullMode(D3D11_CULL_BACK);

		//�`��
		ModelDraw(PlayerModel);

		{
			//�G�b�W�̕`��J�����O���[�h���]
			SetCullMode(D3D11_CULL_FRONT);

			//�V�F�[�_�[�؂�ւ�
			//���_���C�A�E�g��ݒ�
			GetDeviceContext()->IASetInputLayout(g_VertexLayoutEdge);
			//���_�V�F�[�_�[���Z�b�g
			GetDeviceContext()->VSSetShader(g_VertexShaderEdge, NULL, 0);
			//�s�N�Z���V�F�[�_�[���Z�b�g
			GetDeviceContext()->PSSetShader(g_PixelShaderEdge, NULL, 0);

			ModelDraw(PlayerModel);

			//�J�����O���[�h�𕜋A
			SetCullMode(D3D11_CULL_BACK);

		}
		


	}


}

/*==============================================================================

   ���_�Ǘ� [polygon3D.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "Field.h"
#include "sprite.h"
#include "Camera.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

#define	NUM_VERTEX	(4)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************


//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static	Camera		g_CameraObject;	//�J�����\����
static	Object3D	g_Object1;		//3D�|���S���I�u�W�F�N�g�\����
static	ID3D11VertexShader* g_VertexShader;	//���_�V�F�[�_�[�I�u�W�F�N�g�|�C���^
static	ID3D11PixelShader* g_PixelShader;	//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�|�C���^
static	ID3D11InputLayout* g_VertexLayout;	//���_���C�A�E�g�I�u�W�F�N�g�|�C���^

static	ID3D11Buffer* g_VertexBuffer;	//�e�X�g�p���_�o�b�t�@
static	ID3D11Buffer* g_IndexBuffer;	//�e�X�g�p�C���f�b�N�X�o�b�t�@


static LIGHT Light; //���C�g�I�u�W�F�N�g

//���_�f�[�^
/*
      V0------------------V1
      |                  |
     |      �V��        |
    |                  |
   V2-----------------V3

*/




VERTEX_3D	Box[NUM_VERTEX] =
{
	//�V��////////////////////////////////////////////
	{//���_V0 LEFT-TOP
		XMFLOAT3(-5.0f, 0.0f, 5.0f),	//���_���W
		XMFLOAT3(0.0f,1.0f,0.0f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�J���[
		XMFLOAT2(0.0f,0.0f)				//�e�N�X�`�����W
	},
	{//���_V1 RIGHT-TOP
		XMFLOAT3(5.0f, 0.0f, 5.0f),	//���_���W
		XMFLOAT3(0.0f,1.0f,0.0f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�J���[
		XMFLOAT2(5.0f,0.0f)				//�e�N�X�`�����W
	},
	{//���_V2 LEFT-BOTTOM
		XMFLOAT3(-5.0f, 0.0f, -5.0f),	//���_���W
		XMFLOAT3(0.0f,1.0f,0.0f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�J���[
		XMFLOAT2(0.0f,5.0f)				//�e�N�X�`�����W
	},
	{//���_V3 RIGHT-BOTTOM
		XMFLOAT3(5.0f, 0.0f, -5.0f),	//���_���W
		XMFLOAT3(0.0f,1.0f,0.0f),		//�@��
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//�J���[
		XMFLOAT2(5.0f,5.0f)				//�e�N�X�`�����W
	},

};
//=============================================================================
// ����������
//=============================================================================
HRESULT InitPolygon3D(void)
{

	//�e�N�X�`���ǂݍ���
	int Texture = TextureLoad(L"asset\\texture\\sura.jpg");

	//�V�F�[�_�[�ǂݍ���
	CreateVertexShader(&g_VertexShader, &g_VertexLayout, "spotLightingVS.cso");
	CreatePixelShader(&g_PixelShader, "spotLightingPS.cso");

	//�J�����̏�����
	g_CameraObject.Position = XMFLOAT3(0.0f, 1.5f, -2.0f);
	g_CameraObject.UpVector = XMFLOAT3(0.0f, 1.0f, 0.0f);
	g_CameraObject.AtPosition = XMFLOAT3(0.0f, 0.0f, 1.5f);
	g_CameraObject.Fov = 40.0f;
	g_CameraObject.Nearclip = 0.5f;
	g_CameraObject.Farclip = 1000.0f;

	//���_�o�b�t�@�쐬
	{
		D3D11_BUFFER_DESC	bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

		//���_�o�b�t�@�̏������ݐ�̃|�C���^�[���擾
		D3D11_MAPPED_SUBRESOURCE	msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//���_�f�[�^���R�s�[
		CopyMemory(&vertex[0], &Box[0], sizeof(VERTEX_3D) * NUM_VERTEX);
		//�������݊���
		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	//3D�I�u�W�F�N�g�Ǘ��\���̂̏�����
	g_Object1.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Object1.Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Object1.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_Object1.TexID = Texture;



	//���C�g�\���̂̏�����
	Light.Enable = true;
	Light.Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f); //���̕���
	XMVECTOR temp = XMLoadFloat4(&Light.Direction);
	temp = XMVector4Normalize(temp); //���K��
	XMStoreFloat4(&Light.Direction, temp); //���K�������l��߂�

	Light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); //
	Light.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f); //�������F

	Light.Position = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f); //���C�g�̈ʒu
	Light.PointLightParam = XMFLOAT4(10.0f, 1.5f, 0.0f, 0.0f); //x�͌��͂����� y���x����

	Light.Angle = XMFLOAT4((3.1415f / 180.0f) * 30.0f, 0.0f, 0.0f, 0.0f); //�X�|�b�g���C�g�̊p�x


	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void FinalizePolygon3D(void)
{
	//��������̂����
	g_VertexShader->Release();
	g_VertexLayout->Release();
	g_PixelShader->Release();

	g_VertexBuffer->Release();
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePolygon3D(void)
{

}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPolygon3D(void)
{
	//���_���C�A�E�g��ݒ�
	GetDeviceContext()->IASetInputLayout(g_VertexLayout);
	//���_�V�F�[�_�[���Z�b�g
	GetDeviceContext()->VSSetShader(g_VertexShader, NULL, 0);
	//�s�N�Z���V�F�[�_�[���Z�b�g
	GetDeviceContext()->PSSetShader(g_PixelShader, NULL, 0);

	//3D�p�s��ݒ�
	SetDepthEnable(true);		//���s�������L��

	////�v���W�F�N�V�����s����쐬
	//XMMATRIX	ProjectionMatrix =
	//	XMMatrixPerspectiveFovLH(
	//		XMConvertToRadians(g_CameraObject.Fov),//��p
	//		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,//�A�X�y�N�g��
	//		g_CameraObject.Nearclip,//�ߖʃN���b�v����
	//		g_CameraObject.Farclip//���ʃN���b�v����
	//	);
	////DirectX�փZ�b�g
	//SetProjectionMatrix(ProjectionMatrix);

	////�J�����s����쐬
	//XMVECTOR	eyev = XMLoadFloat3(&g_CameraObject.AtPosition);//�ʒu
	//XMVECTOR	pos = XMLoadFloat3(&g_CameraObject.Position);//�����_
	//XMVECTOR	up = XMLoadFloat3(&g_CameraObject.UpVector);//����x�N�g���i1.0�Œ�j
	//XMMATRIX	ViewMatrix =
	//	XMMatrixLookAtLH(pos, eyev, up);//����n���b�N�A�b�g�J����

	////DirectX�փZ�b�g
	//SetViewMatrix(ViewMatrix);

	{//3D�|���S���P���̏���
		//�e�N�X�`�����Z�b�g
		ID3D11ShaderResourceView* tex = GetTexture(g_Object1.TexID);
		GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

		//���s�ړ��s��쐬
		XMMATRIX	TranslationMatrix =
			XMMatrixTranslation(
				g_Object1.Position.x,
				g_Object1.Position.y,
				g_Object1.Position.z
			);
		//��]�s��쐬
		XMMATRIX	RotationMatrix =
			XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_Object1.Rotate.x),
				XMConvertToRadians(g_Object1.Rotate.y),
				XMConvertToRadians(g_Object1.Rotate.z)
			);
		//�X�P�[�����O�s��쐬
		XMMATRIX	ScalingMatrix =
			XMMatrixScaling(
				g_Object1.Scale.x,
				g_Object1.Scale.y,
				g_Object1.Scale.z
			);
		//���[���h�s��쐬 ����Z�̏��Ԃɒ���
		XMMATRIX	WorldMatrix =
			ScalingMatrix *
			RotationMatrix *
			TranslationMatrix;
		//DirectX�փZ�b�g
		SetWorldMatrix(WorldMatrix);

		//���_�o�b�t�@���Z�b�g
		UINT	stride = sizeof(VERTEX_3D);
		UINT	offset = 0;
		GetDeviceContext()->IASetVertexBuffers(
			0,
			1,
			&g_VertexBuffer,
			&stride,
			&offset
		);

		//�v���~�e�B�u�g�|���W�[�̐ݒ�
		GetDeviceContext()->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
		);
		//�}�e���A���ݒ�
		MATERIAL	material;
		ZeroMemory(&material, sizeof(MATERIAL));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);


		SetLight(Light); //���C�g�ݒ�

		//�`��
		GetDeviceContext()->Draw(NUM_VERTEX, 0);//�C���f�b�N�X�����`��
	}


}

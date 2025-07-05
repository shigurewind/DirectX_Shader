#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Sprite.h"
#include "Game.h"
#include "keyboard.h"
#include "Manager.h"
#include "Camera.h"
#include "texture.h"

//#include	"polygon2D.h"
//#include	"Field.h"//<<<<<<<<<<<�ǉ�
#include	"Player.h"//<<<<<<<<<<<�ǉ�
#include "PointLight.h"//<<<<<<<<<<<�ǉ�
#include "spotLighting.h"
#include "limLighting.h"
#include "toon1.h"
#include "toon2.h"

#include "bumpMapField.h"



//===============================================
//�O���[�o���ϐ�
 
//�|�[�Y�t���O
static	bool	pause = false;

//===============================================
//�|�[�Y�t���O�Z�b�g
void	SetPause(bool flg)
{
	pause = flg;
}
//===============================================
//�|�[�Y�t���O�擾
bool	GetPause()
{
	return pause;
}

//===============================================
//�Q�[���V�[��������
void InitGame()
{
	TextureInitialize(GetDevice());
	InitCamera();

//	InitPolygon2D();
	//InitPolygon3D();
	InitPolygonModel();
	InitPolygonModel2();
	InitPolygonModel3();
	InitLimLighting();
	InitPolygonToon1();
	InitPolygonToon2();

	InitBumpMapField();
	

}

//===============================================
//�Q�[���V�[���I��
void FinalizeGame()
{
	FinalizeCamera();
//	FinalizePolygon2D();
	//FinalizePolygon3D();
	FinalizePolygonModel();
	FinalizePolygonModel2();
	FinalizePolygonModel3();
	FinalizeLimLighting();
	FinalizePolygonToon1();
	FinalizePolygonToon2();

	FinalizeBumpMapField();

	TextureFinalize();
}

//===============================================
//�Q�[���V�[���X�V
void UpdateGame()
{

	if (GetPause() == false)//�|�[�Y���łȂ���΍X�V���s
	{
		UpdateCamera();
//		UpdatePolygon2D();
		//UpdatePolygon3D();
		UpdatePolygonModel();
		UpdatePolygonModel2();
		UpdatePolygonModel3();
		UpdateLimLighting();
		UpdatePolygonToon1();
		UpdatePolygonToon2();

		UpdateBumpMapField();

	}

}

//===============================================
//�Q�[���V�[���`��
void DrawGame()
{

	// 2D�p�}�g���N�X�ݒ�
//	SetWorldViewProjection2D();
//	DrawPolygon2D();
	DrawCamera();
	//DrawPolygon3D();
	DrawPolygonModel();
	DrawPolygonModel2();
	DrawPolygonModel3();
	DrawLimLighting();
	DrawPolygonToon1();
	DrawPolygonToon2();
	DrawBumpMapField();
}

/*

	FBX�t�@�C���̓e�N�X�`���𖄂ߍ���ŏo�͂���

*/


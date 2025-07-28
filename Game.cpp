#include "Main.h"
#include "Renderer.h"
#include "Manager.h"
#include "Sprite.h"
#include "Game.h"
#include "keyboard.h"
#include "Manager.h"
#include "Camera.h"
#include "texture.h"

#include	"polygon2D.h"
#include	"Field.h"//<<<<<<<<<<<追加
#include	"Player.h"//<<<<<<<<<<<追加
#include "PointLight.h"//<<<<<<<<<<<追加
#include "spotLighting.h"
#include "limLighting.h"
#include "toon1.h"
#include "toon2.h"

#include "bumpMapField.h"
#include "mosaic.h"



//===============================================
//グローバル変数
 
//ポーズフラグ
static	bool	pause = false;

//===============================================
//ポーズフラグセット
void	SetPause(bool flg)
{
	pause = flg;
}
//===============================================
//ポーズフラグ取得
bool	GetPause()
{
	return pause;
}

//===============================================
//ゲームシーン初期化
void InitGame()
{
	TextureInitialize(GetDevice());
	InitCamera();

	InitPolygon2D();
	InitPolygon3D();
	InitPolygonModel();
	InitPolygonModel2();
	InitPolygonModel3();
	InitLimLighting();
	InitPolygonToon1();
	InitPolygonToon2();

	InitBumpMapField();

	InitMosaic();
	

}

//===============================================
//ゲームシーン終了
void FinalizeGame()
{
	FinalizeCamera();
	FinalizePolygon2D();
	FinalizePolygon3D();
	FinalizePolygonModel();
	FinalizePolygonModel2();
	FinalizePolygonModel3();
	FinalizeLimLighting();
	FinalizePolygonToon1();
	FinalizePolygonToon2();

	FinalizeBumpMapField();

	FinalizeMosaic();

	TextureFinalize();
}

//===============================================
//ゲームシーン更新
void UpdateGame()
{

	if (GetPause() == false)//ポーズ中でなければ更新実行
	{
		UpdateCamera();
		UpdatePolygon2D();
		UpdatePolygon3D();
		UpdatePolygonModel();
		UpdatePolygonModel2();
		UpdatePolygonModel3();
		UpdateLimLighting();
		UpdatePolygonToon1();
		UpdatePolygonToon2();

		UpdateBumpMapField();

		UpdateMosaic();

	}

}

//===============================================
//ゲームシーン描画
void DrawGame()
{
	//1パス目
	BeginPE();

	// 3D用マトリクス設定
	DrawCamera();
	//DrawPolygon3D();
	/*DrawPolygonModel();
	DrawPolygonModel2();*/
	//DrawPolygonModel3();
	//DrawLimLighting();
	DrawPolygonToon1();
	//DrawPolygonToon2();
	DrawBumpMapField();

	//2パス目
	Clear();
	// 2D用マトリクス設定
	SetWorldViewProjection2D();
	//DrawPolygon2D();
	DrawMosaic();

	Present();
}

/*

	FBXファイルはテクスチャを埋め込んで出力する

*/


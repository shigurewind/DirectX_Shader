
//Camera.cpp

#include	"Camera.h"
#include	"keyboard.h"

Camera		g_Camera;	//カメラ管理構造体


void	InitCamera()
{
	//カメラの初期化
	g_Camera.Position = XMFLOAT3(0.0f, 1.5f, -2.0f);		//カメラ座標
	g_Camera.UpVector = XMFLOAT3(0.0f, 1.0f, 0.0f);		//カメラの上方ベクトル
	g_Camera.AtPosition = XMFLOAT3(0.0f, 0.0f, 1.0f);	//カメラの注視点座標
	g_Camera.Fov = 35.0f;								//画角
	g_Camera.Nearclip = 0.5f;							//近面クリップ
	g_Camera.Farclip = 1000.0f;							//遠面クリップ

}

void	FinalizeCamera()
{

}

void	UpdateCamera()
{


}

void	DrawCamera()//3D使用時
{
	//プロジェクション行列を作成
	XMMATRIX	ProjectionMatrix =
		XMMatrixPerspectiveFovLH(
			XMConvertToRadians(g_Camera.Fov),//画角　視野角
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,//アスペクト比
			g_Camera.Nearclip,
			g_Camera.Farclip
		);
	//プロジェクション行列をセット
	SetProjectionMatrix(ProjectionMatrix);

	//カメラ行列を作成
	XMVECTOR	eyev = XMLoadFloat3(&g_Camera.AtPosition);
	XMVECTOR	pos = XMLoadFloat3(&g_Camera.Position);
	XMVECTOR	up = XMLoadFloat3(&g_Camera.UpVector);
	XMMATRIX	ViewMatrix =
		XMMatrixLookAtLH(pos, eyev, up);

	//カメラ行列をセット
	SetViewMatrix(ViewMatrix);

	//カメラ座標セット
	SetCameraPosition(g_Camera.Position); //カメラ位置をシェーダーへ転送

}





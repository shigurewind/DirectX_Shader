/*==============================================================================

   頂点管理 [polygon3D.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "toon1.h"
#include "sprite.h"
#include "Camera.h"
#include "texture.h"
#include "model.h"

#include "keyboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
//カメラは複数あるとダメなので外す
static	Toon1	g_Object2;		//3Dポリゴンオブジェクト構造体
static	ID3D11VertexShader* g_VertexShader;	//頂点シェーダーオブジェクトポインタ
static	ID3D11PixelShader* g_PixelShader;	//ピクセルシェーダーオブジェクトポインタ
static	ID3D11InputLayout* g_VertexLayout;	//頂点レイアウトオブジェクトポインタ

static	MODEL* PlayerModel;


static LIGHT Light; //ライトオブジェクト

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPolygonToon1(void)
{

	//シェーダー読み込み
	CreateVertexShader(&g_VertexShader, &g_VertexLayout, "toon1VS.cso");
	CreatePixelShader(&g_PixelShader, "toon1PS.cso");

	//3Dオブジェクト管理構造体の初期化
	g_Object2.Position = XMFLOAT3(-0.5f, 0.4f, 1.0f);
	g_Object2.Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Object2.Scale = XMFLOAT3(0.2f, 0.2f, 0.2f);



	//モデル読み込み
	PlayerModel = ModelLoad("asset\\model\\model.fbx");

	//ライト構造体の初期化
	Light.Enable = true;
	Light.Direction = XMFLOAT4(1.0f, -1.0f, 0.0f, 0.0f); //光の方向
	XMVECTOR temp = XMLoadFloat4(&Light.Direction);
	temp = XMVector4Normalize(temp); //正規化
	XMStoreFloat4(&Light.Direction, temp); //正規化した値を戻す

	Light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); //白色
	Light.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f); //薄い白色

	Light.Position = XMFLOAT4(0.5f, 1.5f, 0.0f, 1.0f); //ライトの位置
	Light.PointLightParam = XMFLOAT4(10.0f, 0.0f, 0.0f, 0.0f); //xは光届く距離


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void FinalizePolygonToon1(void)
{
	//作ったものを解放
	g_VertexShader->Release();
	g_VertexLayout->Release();
	g_PixelShader->Release();

	ModelRelease(PlayerModel);

}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePolygonToon1(void)
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

	//移動処理
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
}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPolygonToon1(void)
{
	//頂点レイアウトを設定
	GetDeviceContext()->IASetInputLayout(g_VertexLayout);
	//頂点シェーダーをセット
	GetDeviceContext()->VSSetShader(g_VertexShader, NULL, 0);
	//ピクセルシェーダーをセット
	GetDeviceContext()->PSSetShader(g_PixelShader, NULL, 0);

	//3D用行列設定
	//ResetWorldViewProjection3D();//3D描画に切り替える
	SetDepthEnable(true);		//奥行き処理有効

	//カメラは共有の情報が設定されているとする

	{//3Dポリゴン１つずつの処理
		//テクスチャをセット
		ID3D11ShaderResourceView* tex = GetTexture(g_Object2.TexID);
		GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

		//平行移動行列作成
		XMMATRIX	TranslationMatrix =
			XMMatrixTranslation(
				g_Object2.Position.x,
				g_Object2.Position.y,
				g_Object2.Position.z
			);
		//回転行列作成
		XMMATRIX	RotationMatrix =
			XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_Object2.Rotate.x),
				XMConvertToRadians(g_Object2.Rotate.y),
				XMConvertToRadians(g_Object2.Rotate.z)
			);
		//スケーリング行列作成
		XMMATRIX	ScalingMatrix =
			XMMatrixScaling(
				g_Object2.Scale.x,
				g_Object2.Scale.y,
				g_Object2.Scale.z
			);
		//ワールド行列作成 ※乗算の順番に注意
		XMMATRIX	WorldMatrix =
			ScalingMatrix *
			RotationMatrix *
			TranslationMatrix;
		//DirectXへセット
		SetWorldMatrix(WorldMatrix);

		//プリミティブトポロジーの設定
		GetDeviceContext()->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
		);
		//マテリアル設定
		MATERIAL	material;
		ZeroMemory(&material, sizeof(MATERIAL));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);


		//ライト設定
		SetLight(Light);

		//描画
		ModelDraw(PlayerModel);
	}


}

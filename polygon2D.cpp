/*==============================================================================

   頂点管理 [polygon2D.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "polygon2D.h"
#include "sprite.h"
#include "Camera.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static	Object2D	g_Object1;				//2Dポリゴン構造体
static	ID3D11VertexShader* g_VertexShader;	//頂点シェーダーオブジェクトポインタ
static	ID3D11PixelShader* g_PixelShader;	//ピクセルシェーダーオブジェクトポインタ
static	ID3D11InputLayout* g_VertexLayout;	//頂点レイアウトオブジェクトポインタ

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitPolygon2D(void)
{
	// テクスチャ読み込み
	int Texture = TextureLoad(L"asset\\texture\\texture.jpg");

	//シェーダー読み込み
	CreateVertexShader(&g_VertexShader, &g_VertexLayout, "shaderVS.cso");
	CreatePixelShader(&g_PixelShader, "shaderPS.cso");

	//2Dオブジェクト初期化
	g_Object1.Position = XMFLOAT3(SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2, 0.0f);
	g_Object1.Color = XMFLOAT4(1.0f, 0.2f, 0.2f, 1.0f);
	g_Object1.Scale = XMFLOAT2(1.0f, 1.0f);
	g_Object1.Size = XMFLOAT2(200.0f, 200.0f);
	g_Object1.Rotate = 0.0f;
	g_Object1.TexID = Texture;

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void FinalizePolygon2D(void)
{
	//作ったものを解放
	g_VertexShader->Release();
	g_VertexLayout->Release();
	g_PixelShader->Release();


}

//=============================================================================
// 更新処理
//=============================================================================
void UpdatePolygon2D(void)
{
	//適当に回転
	g_Object1.Rotate += 0.2f;

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawPolygon2D(void)
{

	//頂点レイアウトを設定
	GetDeviceContext()->IASetInputLayout(g_VertexLayout);
	//頂点シェーダーをセット
	GetDeviceContext()->VSSetShader(g_VertexShader, NULL, 0);
	//ピクセルシェーダーをセット
	GetDeviceContext()->PSSetShader(g_PixelShader, NULL, 0);

	//奥行き処理をOFF
	SetDepthEnable(false);

	// マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	SetMaterial(material);

	{//2Dポリゴン1枚ずつで必要な処理

		//テクスチャをセット
		ID3D11ShaderResourceView* tex = GetTexture(g_Object1.TexID);
		GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

		//平行移動行列の作成（表示座標を決める）
		XMMATRIX	TranslationMatrix = XMMatrixTranslation(
			g_Object1.Position.x, g_Object1.Position.y, 0.0f);

		//回転行列（Z回転）行列の作成
		XMMATRIX	RotationMatrix = XMMatrixRotationZ(XMConvertToRadians(g_Object1.Rotate));

		//スケーリング行列作成（倍率1.0が等倍、0倍はダメ！）
		XMMATRIX	ScalingMatrix = XMMatrixScaling(g_Object1.Scale.x,g_Object1.Scale.y, 1.0f);

		//ワールド行列の作成（ポリゴンの表示の仕方を指定する最終的な行列
		XMMATRIX	WorldMatrix = ScalingMatrix * RotationMatrix * TranslationMatrix;

		//ワールド行列をDirectXへセット
		SetWorldMatrix(WorldMatrix);

		// ポリゴン描画
		DrawSprite(g_Object1.Size, g_Object1.Color);
	}


}

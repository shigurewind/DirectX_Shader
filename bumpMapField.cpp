/*==============================================================================

   頂点管理 [BumpMapField.cpp]
														 Author :
														 Date   :
--------------------------------------------------------------------------------

==============================================================================*/
#include "bumpMapField.h"
#include "sprite.h"
#include "Camera.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

#define	NUM_VERTEX	(4)

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************


//*****************************************************************************
// グローバル変数
//*****************************************************************************
static	Camera		g_CameraObject;	//カメラ構造体
static	BumpMapField	g_Object1;		//3Dポリゴンオブジェクト構造体
static	ID3D11VertexShader* g_VertexShader;	//頂点シェーダーオブジェクトポインタ
static	ID3D11PixelShader* g_PixelShader;	//ピクセルシェーダーオブジェクトポインタ
static	ID3D11InputLayout* g_VertexLayout;	//頂点レイアウトオブジェクトポインタ

static	ID3D11Buffer* g_VertexBuffer;	//テスト用頂点バッファ
static	ID3D11Buffer* g_IndexBuffer;	//テスト用インデックスバッファ


static LIGHT Light; //ライトオブジェクト

//頂点データ
/*
      V0------------------V1
      |                  |
     |      天井        |
    |                  |
   V2-----------------V3

*/




static VERTEX_3D	Box[NUM_VERTEX] =
{
	//天井////////////////////////////////////////////
	{//頂点V0 LEFT-TOP
		XMFLOAT3(-5.0f, 0.0f, 5.0f),	//頂点座標
		XMFLOAT3(0.0f,1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V1 RIGHT-TOP
		XMFLOAT3(5.0f, 0.0f, 5.0f),	//頂点座標
		XMFLOAT3(0.0f,1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(5.0f,0.0f)				//テクスチャ座標
	},
	{//頂点V2 LEFT-BOTTOM
		XMFLOAT3(-5.0f, 0.0f, -5.0f),	//頂点座標
		XMFLOAT3(0.0f,1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(0.0f,5.0f)				//テクスチャ座標
	},
	{//頂点V3 RIGHT-BOTTOM
		XMFLOAT3(5.0f, 0.0f, -5.0f),	//頂点座標
		XMFLOAT3(0.0f,1.0f,0.0f),		//法線
		XMFLOAT4(1.0f,1.0f,1.0f,1.0f),	//カラー
		XMFLOAT2(5.0f,5.0f)				//テクスチャ座標
	},

};
//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitBumpMapField(void)
{

	//テクスチャ読み込み
	int Texture = TextureLoad(L"asset\\texture\\sura.jpg");

	g_Object1.BumpTexID = TextureLoad(L"asset\\texture\\bump.png");
	//g_Object1.BumpTexID = TextureLoad(L"asset\\texture\\Normal.bmp");

	//シェーダー読み込み
	CreateVertexShader(&g_VertexShader, &g_VertexLayout, "bumpVS.cso");
	CreatePixelShader(&g_PixelShader, "bumpPS.cso");

	//カメラの初期化
	g_CameraObject.Position = XMFLOAT3(0.0f, 1.5f, -2.0f);
	g_CameraObject.UpVector = XMFLOAT3(0.0f, 1.0f, 0.0f);
	g_CameraObject.AtPosition = XMFLOAT3(0.0f, 0.0f, 1.5f);
	g_CameraObject.Fov = 40.0f;
	g_CameraObject.Nearclip = 0.5f;
	g_CameraObject.Farclip = 1000.0f;

	//頂点バッファ作成
	{
		D3D11_BUFFER_DESC	bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.ByteWidth = sizeof(VERTEX_3D) * NUM_VERTEX;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D10_CPU_ACCESS_WRITE;
		GetDevice()->CreateBuffer(&bd, NULL, &g_VertexBuffer);

		//頂点バッファの書き込み先のポインターを取得
		D3D11_MAPPED_SUBRESOURCE	msr;
		GetDeviceContext()->Map(g_VertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &msr);
		VERTEX_3D* vertex = (VERTEX_3D*)msr.pData;

		//頂点データをコピー
		CopyMemory(&vertex[0], &Box[0], sizeof(VERTEX_3D) * NUM_VERTEX);
		//書き込み完了
		GetDeviceContext()->Unmap(g_VertexBuffer, 0);
	}

	//3Dオブジェクト管理構造体の初期化
	g_Object1.Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Object1.Rotate = XMFLOAT3(0.0f, 0.0f, 0.0f);
	g_Object1.Scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	g_Object1.TexID = Texture;



	//ライト構造体の初期化
	Light.Enable = true;
	Light.Direction = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f); //光の方向
	XMVECTOR temp = XMLoadFloat4(&Light.Direction);
	temp = XMVector4Normalize(temp); //正規化
	XMStoreFloat4(&Light.Direction, temp); //正規化した値を戻す

	Light.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f); //
	Light.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f); //薄い白色

	Light.Position = XMFLOAT4(0.0f, 20.0f, 0.0f, 1.0f); //ライトの位置
	Light.PointLightParam = XMFLOAT4(100.0f, 2.5f, 0.0f, 0.0f); //xは光届く距離 y明度調整

	Light.Angle = XMFLOAT4((3.1415f / 180.0f) * 30.0f, 0.0f, 0.0f, 0.0f); //スポットライトの角度


	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void FinalizeBumpMapField(void)
{
	//作ったものを解放
	g_VertexShader->Release();
	g_VertexLayout->Release();
	g_PixelShader->Release();

	g_VertexBuffer->Release();
}

//=============================================================================
// 更新処理
//=============================================================================
void UpdateBumpMapField(void)
{

}

//=============================================================================
// 描画処理
//=============================================================================
void DrawBumpMapField(void)
{
	//頂点レイアウトを設定
	GetDeviceContext()->IASetInputLayout(g_VertexLayout);
	//頂点シェーダーをセット
	GetDeviceContext()->VSSetShader(g_VertexShader, NULL, 0);
	//ピクセルシェーダーをセット
	GetDeviceContext()->PSSetShader(g_PixelShader, NULL, 0);

	//3D用行列設定
	SetDepthEnable(true);		//奥行き処理有効

	////プロジェクション行列を作成
	//XMMATRIX	ProjectionMatrix =
	//	XMMatrixPerspectiveFovLH(
	//		XMConvertToRadians(g_CameraObject.Fov),//画角
	//		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,//アスペクト比
	//		g_CameraObject.Nearclip,//近面クリップ距離
	//		g_CameraObject.Farclip//遠面クリップ距離
	//	);
	////DirectXへセット
	//SetProjectionMatrix(ProjectionMatrix);

	////カメラ行列を作成
	//XMVECTOR	eyev = XMLoadFloat3(&g_CameraObject.AtPosition);//位置
	//XMVECTOR	pos = XMLoadFloat3(&g_CameraObject.Position);//注視点
	//XMVECTOR	up = XMLoadFloat3(&g_CameraObject.UpVector);//上方ベクトル（1.0固定）
	//XMMATRIX	ViewMatrix =
	//	XMMatrixLookAtLH(pos, eyev, up);//左手系ルックアットカメラ

	////DirectXへセット
	//SetViewMatrix(ViewMatrix);

	{//3Dポリゴン１つずつの処理
		//テクスチャをセット
		ID3D11ShaderResourceView* tex = GetTexture(g_Object1.TexID);
		GetDeviceContext()->PSSetShaderResources(0, 1, &tex);

		//ノーマルマップをセット
		ID3D11ShaderResourceView* tex2 = GetTexture(g_Object1.BumpTexID);
		GetDeviceContext()->PSSetShaderResources(1, 1, &tex2);

		//平行移動行列作成
		XMMATRIX	TranslationMatrix =
			XMMatrixTranslation(
				g_Object1.Position.x,
				g_Object1.Position.y,
				g_Object1.Position.z
			);
		//回転行列作成
		XMMATRIX	RotationMatrix =
			XMMatrixRotationRollPitchYaw(
				XMConvertToRadians(g_Object1.Rotate.x),
				XMConvertToRadians(g_Object1.Rotate.y),
				XMConvertToRadians(g_Object1.Rotate.z)
			);
		//スケーリング行列作成
		XMMATRIX	ScalingMatrix =
			XMMatrixScaling(
				g_Object1.Scale.x,
				g_Object1.Scale.y,
				g_Object1.Scale.z
			);
		//ワールド行列作成 ※乗算の順番に注意
		XMMATRIX	WorldMatrix =
			ScalingMatrix *
			RotationMatrix *
			TranslationMatrix;
		//DirectXへセット
		SetWorldMatrix(WorldMatrix);

		//頂点バッファをセット
		UINT	stride = sizeof(VERTEX_3D);
		UINT	offset = 0;
		GetDeviceContext()->IASetVertexBuffers(
			0,
			1,
			&g_VertexBuffer,
			&stride,
			&offset
		);

		//プリミティブトポロジーの設定
		GetDeviceContext()->IASetPrimitiveTopology(
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
		);
		//マテリアル設定
		MATERIAL	material;
		ZeroMemory(&material, sizeof(MATERIAL));
		material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		SetMaterial(material);


		SetLight(Light); //ライト設定

		//描画
		GetDeviceContext()->Draw(NUM_VERTEX, 0);//インデックス無し描画
	}


}

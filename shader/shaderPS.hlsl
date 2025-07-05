
#include "common.hlsl"


//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register( t0 );		//テクスチャ変数
SamplerState	g_SamplerState : register( s0 );//サンプラー変数

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main( in  float4 inPosition	: POSITION0,	//ピクセルの座標
		   in  float4 inNormal		: NORMAL0,		//ピクセルの法線
		   in  float4 inDiffuse		: COLOR0,		//ピクセルのカラー
		   in float2  inTexCoord	: TEXCOORD0,	//ピクセルのテクスチャ座標
   		   out float4 outDiffuse	: SV_Target )	//最終的なピクセルのカラー
{
	//テクセルをフェッチして出力
	outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);

}


#include "common.hlsl"


//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register( t0 );		//テクスチャ変数
SamplerState	g_SamplerState : register( s0 );//サンプラー変数

//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main( in float4 inPosition : SV_POSITION,     //変換後頂点座標
		   in float4 inWorldPosition : POSITION0,  //ワールド変換後頂点座標,ポジションゼロ
                     
                     
		   in float4 inNormal : NORMAL0,           //頂点法線
		   in float4 inDiffuse : COLOR0,           //頂点カラー
		   in float2 inTexCoord : TEXCOORD0,       //テクスチャ座標

   		   out float4 outDiffuse	: SV_Target )	//最終的なピクセルのカラー
{
	
    outDiffuse = inDiffuse; //頂点カラーをそのまま出力
	
}

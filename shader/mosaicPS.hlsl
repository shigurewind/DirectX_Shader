
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
    
    float2 uv = inTexCoord;
    float2 len = float2(0.5f, 0.5f);//円の中心座標
    
    len -= uv; //uvから円の中心座標を引く
    len.y /= (Parameter.x / Parameter.y); //アスペクト比を考慮
    len = length(len);//中心からの長さ
    
    
    if (len.x <= 0.1f)
    {
        uv *= float2(Parameter.x, Parameter.y);
        uv /= Parameter.z;
        uv = floor(uv) * Parameter.z;
        uv /= float2(Parameter.x, Parameter.y);
    
        uv = clamp(uv, 0.01f, 0.99f); //テクスチャ座標を内側に寄せる
    }
    
    
    
    
    outDiffuse = g_Texture.Sample(g_SamplerState, uv);
    outDiffuse *= inDiffuse; 

}

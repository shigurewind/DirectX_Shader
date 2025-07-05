

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer ConstantBuffer : register( b0 )
{
	matrix WorldViewProjection;
}


cbuffer WorldBuffer : register(b2)
{
    matrix World;
}


// マテリアルバッファ
cbuffer MaterialBuffer : register( b1 )
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	float3		Dummy;//16bit境界用
}

struct LIGHT
{
    bool		Enable; //有効フラグ
    bool3		Dummy;
    float4		Direction;
    float4		Diffuse;
    float4		Ambient;
    
    float4      Position; //ライトの位置
    float4      PointLightParam; //x = 光届く距離
    
    float4      Angle;
};


cbuffer LightBuffer : register(b3)
{
    LIGHT Light;
    //int LightCount;
    //float4 CameraPosition;
    //float4 CameraDirection;
    //float4 CameraUp;
    //float4 CameraRight;
};

cbuffer CameraBufffer : register(b4)
{
    float4 CameraPosition;
    
};

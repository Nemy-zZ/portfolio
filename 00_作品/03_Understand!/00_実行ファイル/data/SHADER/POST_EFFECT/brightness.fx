//============================================================
// 明るさ
//============================================================

texture _tex;
float g_Brightness = 1.0f; // 明るさ係数 (1.0 = 等倍, >1.0 = 明るく, <1.0 = 暗く)

sampler SrcSampler = sampler_state
{
    Texture = <_tex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = NONE;
    AddressU = CLAMP;
    AddressV = CLAMP;
};

// 入力頂点フォーマット
struct VS_INPUT
{
    float4 Pos : POSITION; // 頂点座標
    float2 Tex : TEXCOORD0; // UV座標
};

// 頂点シェーダー出力
struct VS_OUTPUT
{
    float4 Pos : POSITION; // スクリーン座標
    float2 Tex : TEXCOORD0; // UV座標
};

// 頂点シェーダー (そのまま渡すだけ)
VS_OUTPUT VS_Main(VS_INPUT In)
{
    VS_OUTPUT Out;
    Out.Pos = In.Pos;
    Out.Tex = In.Tex;
    return Out;
}

// ピクセルシェーダー (明るさ調整)
float4 PS_Brightness(VS_OUTPUT In) : COLOR0
{
    float4 color = tex2D(SrcSampler, In.Tex);
    color.rgb *= g_Brightness; // 明るさ補正
    return color;
}

technique Brightness
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Brightness();
    }
}
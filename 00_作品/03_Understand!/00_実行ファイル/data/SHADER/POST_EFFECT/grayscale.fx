//=========================================================
// グレースケール
//=========================================================

// 入力カラー（シーン）テクスチャ
texture _tex;

// サンプラ
sampler2D ColorSampler = sampler_state
{
    Texture = <_tex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

// 0=元色, 1=完全グレースケール
float GrayscaleAmount = 1.0f;

// 画面用パススルー頂点シェーダ
struct VS_IN
{
    float4 pos : POSITION; // 既にスクリーン空間(XY) or RHW ならそのまま
    float2 uv : TEXCOORD0;
};

struct VS_OUT
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

VS_OUT VS_PassThrough(VS_IN In)
{
    VS_OUT Out;
    Out.pos = In.pos;
    Out.uv = In.uv;
    return Out;
}

// ピクセルシェーダ（グレースケール）
float4 PS_Grayscale(VS_OUT In) : COLOR0
{
    float4 col = tex2D(ColorSampler, In.uv);

    // luma（Rec.709系の係数）
    float luminance = dot(col.rgb, float3(0.2126, 0.7152, 0.0722));

    // 元色とグレースケールのブレンド
    float3 gray = lerp(col.rgb, luminance.xxx, saturate(GrayscaleAmount));

    return float4(gray, col.a);
}

// テクニック
technique Grayscale
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS_PassThrough();
        PixelShader = compile ps_2_0 PS_Grayscale();
    }
}
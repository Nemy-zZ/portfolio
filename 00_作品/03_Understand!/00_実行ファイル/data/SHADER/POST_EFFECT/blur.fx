//=========================================================
// ブラー
//=========================================================

texture _tex;
sampler2D ColorSampler = sampler_state
{
    Texture = <_tex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

// 1ピクセル分のUVオフセット: (1/width, 1/height)
float2 _texel;

// ブラー強さ（係数でスケール。1.0が標準）
float g_BlurScale = 1.0f;

// --- シンプルな 9 タップガウス係数（中心が最大） ---
// weights: [0.05, 0.09, 0.12, 0.15, 0.49] を左右対称に適用
static const float w0 = 0.49; // center
static const float w1 = 0.15;
static const float w2 = 0.12;
static const float w3 = 0.09;
static const float w4 = 0.05;

// 頂点パススルー（RHW 頂点や正射影頂点に合わせて使ってください）
struct VS_IN
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};
struct VS_OUT
{
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
};

VS_OUT VS_Pass(VS_IN In)
{
    VS_OUT Out;
    Out.pos = In.pos;
    Out.uv = In.uv;
    return Out;
}

// 水平方向ブラー（COLOR0 に出力）
float4 PS_BlurH(VS_OUT In) : COLOR0
{
    float2 uv = In.uv;
    float2 off = float2(_texel.x * g_BlurScale, 0.0);
    float3 sum = tex2D(ColorSampler, uv).rgb * w0;

    sum += tex2D(ColorSampler, uv + off * 1.0).rgb * w1;
    sum += tex2D(ColorSampler, uv - off * 1.0).rgb * w1;

    sum += tex2D(ColorSampler, uv + off * 2.0).rgb * w2;
    sum += tex2D(ColorSampler, uv - off * 2.0).rgb * w2;

    sum += tex2D(ColorSampler, uv + off * 3.0).rgb * w3;
    sum += tex2D(ColorSampler, uv - off * 3.0).rgb * w3;

    sum += tex2D(ColorSampler, uv + off * 4.0).rgb * w4;
    sum += tex2D(ColorSampler, uv - off * 4.0).rgb * w4;

    return float4(sum, 1.0);
}

// 垂直方向ブラー（COLOR0 に出力）
float4 PS_BlurV(VS_OUT In) : COLOR0
{
    float2 uv = In.uv;
    float2 off = float2(0.0, _texel.y * g_BlurScale);
    float3 sum = tex2D(ColorSampler, uv).rgb * w0;

    sum += tex2D(ColorSampler, uv + off * 1.0).rgb * w1;
    sum += tex2D(ColorSampler, uv - off * 1.0).rgb * w1;

    sum += tex2D(ColorSampler, uv + off * 2.0).rgb * w2;
    sum += tex2D(ColorSampler, uv - off * 2.0).rgb * w2;

    sum += tex2D(ColorSampler, uv + off * 3.0).rgb * w3;
    sum += tex2D(ColorSampler, uv - off * 3.0).rgb * w3;

    sum += tex2D(ColorSampler, uv + off * 4.0).rgb * w4;
    sum += tex2D(ColorSampler, uv - off * 4.0).rgb * w4;

    return float4(sum, 1.0);
}

technique Blur_Horizontal
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS_Pass();
        PixelShader = compile ps_2_0 PS_BlurH();
    }
}

technique Blur_Vertical
{
    pass P0
    {
        VertexShader = compile vs_2_0 VS_Pass();
        PixelShader = compile ps_2_0 PS_BlurV();
    }
}
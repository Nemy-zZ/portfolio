//==================================================
// VHS風ポストエフェクトシェーダー
//==================================================

// 入力テクスチャ（シーン全体）
texture _tex;

// サンプラ
sampler2D SceneSampler = sampler_state
{
    Texture = <_tex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = Clamp;
    AddressV = Clamp;
};

// 時間（C++側から SetFloat で渡す）
float _elapsedTime : TIME;

// ノイズ強度
float NoiseStrength = 0.05f;

// 走査線の強度
float ScanlineStrength = 0.3f;

// RGBずれの量
float ChromaticAberration = 1.5f / 800.0f; // 横ずれ（画面幅依存）

//========================
// 頂点シェーダー入力
//========================
struct VS_IN
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

// 頂点シェーダー出力
struct VS_OUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD0;
};

//========================
// 頂点シェーダー
//========================
VS_OUT VS_Main(VS_IN In)
{
    VS_OUT Out;
    Out.Pos = In.Pos;
    Out.Tex = In.Tex;
    return Out;
}

//========================
// ピクセルシェーダー
//========================
float4 PS_Main(VS_OUT In) : COLOR0
{
    // 基本のUV
    float2 uv = In.Tex;

    // --- 1. RGBずれ（クロマティックアバレーション） ---
    float2 uvR = uv + float2(ChromaticAberration, 0);
    float2 uvB = uv + float2(-ChromaticAberration, 0);

    float r = tex2D(SceneSampler, uvR).r;
    float g = tex2D(SceneSampler, uv).g;
    float b = tex2D(SceneSampler, uvB).b;
    float3 col = float3(r, g, b);

    // --- 2. ランダムノイズ ---
    float noise = (frac(sin(dot(uv * _elapsedTime, float2(12.9898, 78.233))) * 43758.5453));
    col += (noise - 0.5) * NoiseStrength;

    // --- 3. 走査線効果 ---
    float scan = sin(uv.y * 800.0 + _elapsedTime * 20.0); // 800 = 画面の高さ想定
    col *= 1.0 - ScanlineStrength * (scan * 0.5 + 0.5);

    return float4(col, 1.0);
}

//========================
// テクニック
//========================
technique VHS
{
    pass P0
    {
        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}
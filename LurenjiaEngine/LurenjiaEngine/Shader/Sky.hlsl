#include "Common.hlsl"

struct MeshVertexIn
{
    float3 Position : POSITION;
    float3 Normal   : NORMAL;
    float2 TexCoord : TEXCOORD;
};
struct MeshVertexOut
{
    float4 PositionH : POSITION;
    float4 PositionL : SV_POSITION;
};
MeshVertexOut VertexShaderMain(MeshVertexIn mv)
{
    MeshVertexOut Out = (MeshVertexOut) 0;
    Out.PositionH = float4(mv.Position, 1.0f);
    
    //ÊÀ½ç×ø±ê
    float4x4 mvp = mul(ViewProjectionMatrix, WorldMatrix);
    Out.PositionL = mul(mvp, float4(mv.Position, 1.0f));
    
    return Out;
}
float4 PixelShaderMain(MeshVertexOut mvOut) : SV_Target
{
    return SampleTextureCubeMap.Sample(TextureSampler, mvOut.PositionH.xyz);
    //return float4(0.f, 0.f, 0.f, 1.f);
}
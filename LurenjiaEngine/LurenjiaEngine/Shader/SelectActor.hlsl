#include "Common.hlsl"

struct MeshVertexIn
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
};
struct MeshVertexOut
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : POSITIONT;
};

MeshVertexOut VertexShaderMain(MeshVertexIn mv)
{
    MeshVertexOut MV_out;
    
    float4 WorldPosition = mul(WorldMatrix, float4(mv.Position, 1.0f));
    float3 ViewDirection = normalize(cameraPosition.xyz - WorldPosition.xyz);
    WorldPosition.xyz += ViewDirection;
    
    float4x4 mvp = mul(ViewProjectionMatrix, WorldMatrix);
    MV_out.Position = mul(mvp, float4(mv.Position, 1.0f)); //经过mvp变换到齐次剪裁空间
    
    return MV_out;
}

float4 PixelShaderMain(MeshVertexOut mvOut) : SV_Target
{
    return float4(1.0f, 0.f, 0.f, 1.0f);
}
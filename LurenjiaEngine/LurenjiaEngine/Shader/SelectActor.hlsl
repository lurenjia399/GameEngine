#include "Common.hlsl"

struct MeshVertexIn
{
    float3 Position : POSITION;
    float4 Color : COLOR;
    float3 Normal : NORMAL;
    float3 Tangent : UTANGENT;
    float2 TexCoord : TEXCOORD;
};
struct MeshVertexOut
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : POSITION;
    float3 Normal : NORMAL;
};

MeshVertexOut VertexShaderMain(MeshVertexIn mv)
{
    MeshVertexOut MV_out;
    
    MV_out.WorldPosition = mul(WorldMatrix, float4(mv.Position, 1.0f));
    float3 ViewDirection = normalize(cameraPosition.xyz - MV_out.WorldPosition.xyz);
    MV_out.WorldPosition.xyz += ViewDirection;
    
    MV_out.Position = mul(ViewProjectionMatrix, MV_out.WorldPosition);
    MV_out.Normal = normalize(mul((float3x3) WorldMatrix, mv.Normal));
    
    return MV_out;
}

float4 PixelShaderMain(MeshVertexOut mvOut) : SV_Target
{
    float3 ViewDirection = normalize(cameraPosition.xyz - mvOut.WorldPosition.xyz);
    float DotValue = pow(1.f - max(dot(mvOut.Normal, ViewDirection), 0.0), 3.f);
    
    return float4(1.f, 0.f, 0.f, 1.f) * DotValue;
}
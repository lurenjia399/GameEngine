cbuffer ObjectConstBuffer : register(b0)
{
    float4x4 WorldMatrix;
}
cbuffer ViewportConstBuffer : register(b1)
{
    float4x4 ViewProjectionMatrix;
}
struct MeshVertexIn
{
	float3 Position : POSITION;
	float4 Color : COLOR;
};
struct MeshVertexOut
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
};
MeshVertexOut VertexShaderMain(MeshVertexIn mv)
{
    MeshVertexOut MV_out;
    MV_out.Position = mul(WorldMatrix, float4(mv.Position, 1.0f));
    MV_out.Color = mv.Color;
    return MV_out;
}
float4 PixelShaderMain(MeshVertexOut mvOut) : SV_Target
{
    return mvOut.Color;
}
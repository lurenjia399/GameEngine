
cbuffer ConstBuffer : register(b0)
{
	float4x4 World;
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
    MV_out.Position = mul(float4(mv.Position, 1.0f), World);
    MV_out.Color = float4(1.0f, 0, 0, 1.0f); //mv.Color;
    return MV_out;
}
float4 PixelShaderMain(MeshVertexOut mvOut) : SV_Target
{
    return mvOut.Color;
}

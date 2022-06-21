struct FMaterial
{
    float4 FinalColor;//Í¨ÓÃÑÕÉ«
};

float3 FresnelSchlickMethod(float3 InF0, float3 InVertexNormal, float3 InCameraDirection, float InPow)
{
    return InF0 + (1.f - InF0) * pow(1.0f - saturate(dot(InVertexNormal, InCameraDirection)), InPow);
}
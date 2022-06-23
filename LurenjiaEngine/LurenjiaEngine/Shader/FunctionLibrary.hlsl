
float3x3 GetTBNMatrix(float3 Tangent, float3 Normal)
{
    
    //Tangent = normalize(Tangent - dot(Tangent, Normal) * Tangent); //矫正，若切线和法线不垂直，则需要
    //float3 Bitangent = cross(Tangent, Normal);
    //Bitangent = normalize(Bitangent);
    
    float3 N = Normal;
    float3 T = normalize(Tangent - dot(Tangent, N) * N); //矫正，若切线和法线不垂直，则需要
    float3 B = normalize(cross(N, T));
    //return float3x3(T.x, B.x, N.x, T.y, B.y, N.y, T.z, B.z, N.z);
    return float3x3(T.x, T.y, T.z, B.x, B.y, B.z, N.x, N.y, N.z);
    //return float3x3(T, B, N);
   // return float3x3(B, T, N);
}
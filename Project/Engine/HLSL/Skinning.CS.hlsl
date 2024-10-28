
struct Well {
	float4x4 skeletonSpaceMatrix;
	float4x4 skeletonSpaceInverseTransposeMatrix;
};

struct Vertex {
	float4 position;
	float2 texcoord;
	float3 normal;
	float4 worldPos;
	float3 tangent;
};

struct VertexInfluence {
	float4 weight;
	int4 index;
};

struct SkinningInformation {
	uint numVertices;
};


StructuredBuffer<Well> gMatrixPalette : register(t0);
StructuredBuffer<Vertex> gInputVertices : register(t1);
StructuredBuffer<VertexInfluence> gInfluences : register(t2);
// skinning計算後の頂点データ
RWStructuredBuffer<Vertex> gOutoutVertices : register(u0);
// skinningに関するちょっと情報
ConstantBuffer<SkinningInformation> gSkinningInformation : register(b0);

[numthreads(1024, 1, 1)]
void CSmain(uint3 id : SV_DispatchThreadID) {
	uint vertexIndex = id.x;
	if (vertexIndex < gSkinningInformation.numVertices) {
		// skinning計算
		Vertex input = gInputVertices[vertexIndex];
		VertexInfluence influence = gInfluences[vertexIndex];
		
		// skinnig後の頂点を計算
		Vertex skinned;
		skinned.texcoord = input.texcoord;
		skinned.tangent = input.tangent;
		skinned.worldPos = input.worldPos;
		
		skinned.position = mul(input.position, gMatrixPalette[influence.index.x].skeletonSpaceMatrix) * influence.weight.x;
		skinned.position += mul(input.position, gMatrixPalette[influence.index.y].skeletonSpaceMatrix) * influence.weight.y;
		skinned.position += mul(input.position, gMatrixPalette[influence.index.z].skeletonSpaceMatrix) * influence.weight.z;
		skinned.position += mul(input.position, gMatrixPalette[influence.index.w].skeletonSpaceMatrix) * influence.weight.w;
		skinned.position.w = 1.0f;
	
		// 法線の変換
		skinned.normal = mul(input.normal, (float3x3) gMatrixPalette[influence.index.x].skeletonSpaceInverseTransposeMatrix) * influence.weight.x;
		skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[influence.index.y].skeletonSpaceInverseTransposeMatrix) * influence.weight.y;
		skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[influence.index.z].skeletonSpaceInverseTransposeMatrix) * influence.weight.z;
		skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[influence.index.w].skeletonSpaceInverseTransposeMatrix) * influence.weight.w;
		skinned.normal = normalize(skinned.normal);
		
		gOutoutVertices[vertexIndex] = skinned;
	}

}
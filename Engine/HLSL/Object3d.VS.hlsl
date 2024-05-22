struct TransformationMatrix{
	float4x4 world;
	float4x4 view;
	float4x4 projection;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

struct VertexShaderOutput{
	float4 position : SV_POSITION;
};

struct VertexShaderInput{
	float4 position : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input){
	VertexShaderOutput output;
	// WVPの生成
	float4x4 WVP = mul(gTransformationMatrix.world, mul(gTransformationMatrix.view, gTransformationMatrix.projection));
	output.position = mul(input.position, WVP);
	return output;
}
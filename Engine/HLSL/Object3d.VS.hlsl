struct VertexShaderOutput{
	float4 position : SV_POSITION;
};

struct VertexShaderInput{
	float4 position : POSITION0;
};

VertexShaderOutput main(VertexShaderInput input){
	VertexShaderOutput output;
	output.position = input.position;
	return output;
}
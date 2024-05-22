struct Material{
	float4 color;
};

ConstantBuffer<Material> gMaterial : register(b0);
struct PixelShaderOutput{
	float4 color : SV_TARGET0;
};

PixelShaderOutput main(){
	PixelShaderOutput output;
	output.color = gMaterial.color;
	return output;
}
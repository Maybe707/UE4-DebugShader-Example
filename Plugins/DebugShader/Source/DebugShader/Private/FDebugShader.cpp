// Fill out your copyright notice in the Description page of Project Settings.


#include "FDebugShader.h"
#include "ShaderParameterUtils.h"

IMPLEMENT_UNIFORM_BUFFER_STRUCT(FMyUniformBufferStruct, "MyUniformBuffer");

FDebugShader::FDebugShader() {}

FDebugShader::FDebugShader(
    const ShaderMetaType::CompiledShaderInitializerType &Initializer)
    : FGlobalShader(Initializer) {
	MyUniformBuffer.Bind(Initializer.ParameterMap, TEXT("MyUniformBuffer"));
}

bool FDebugShader::ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
{
	return true; // Always compile
}

void FDebugShader::SetParameters(FRHICommandListImmediate& RHICmdList,
								 const TShaderMapRef<FDebugShader>& VertexShader,
								 const FUniformBufferRHIRef& MyUB)
{
	if (!VertexShader->MyUniformBuffer.IsBound())
		{
			UE_LOG(LogTemp, Error, TEXT("FDebugShader: MyUniformBuffer NOT bound for vertex shader!"));
			return;
		}
	
	SetUniformBufferParameter(RHICmdList, VertexShader.GetVertexShader(), MyUniformBuffer, MyUB);
}

IMPLEMENT_SHADER_TYPE(, FDebugShader, TEXT("/Shaders/Debug.usf"), TEXT("MainVS"), SF_Vertex);

FMyPS::FMyPS() {}

FMyPS::FMyPS(const ShaderMetaType::CompiledShaderInitializerType &Initializer)
    : FGlobalShader(Initializer) {
  	MyUniformBuffer.Bind(Initializer.ParameterMap, TEXT("MyUniformBuffer"));
}

bool FMyPS::ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
{
	return true;
}

void FMyPS::SetParameters(FRHICommandListImmediate& RHICmdList,
						  const TShaderMapRef<FMyPS>& PixelShader,
						  const FUniformBufferRHIRef& MyUB)
{
	if (!PixelShader->MyUniformBuffer.IsBound())
		{
			UE_LOG(LogTemp, Error, TEXT("FDebugShader: MyUniformBuffer NOT bound for pixel shader!"));
			return;
		}
	
	SetUniformBufferParameter(RHICmdList, PixelShader.GetPixelShader(), MyUniformBuffer, MyUB);
}

IMPLEMENT_SHADER_TYPE(, FMyPS, TEXT("/Shaders/Debug.usf"), TEXT("MainPS"), SF_Pixel);

void RenderSimplePass(FRHICommandListImmediate& RHICmdList, FTexture2DRHIRef RenderTarget)
{
	TArray<FSimpleVertex> Vertices;
	Vertices.SetNum(6);

// Set positions manually
	float multiplier = 1.0f;
	Vertices[0].Position = FVector( 1.0f, 1.0f * multiplier, 0.f);
	Vertices[1].Position = FVector( 0.0f * multiplier, 1.0f * multiplier, 0.f);
	Vertices[2].Position = FVector( 0.0f * multiplier, 0.0f * multiplier, 0.f);
	Vertices[3].Position = FVector( 0.0f * multiplier, 0.0f * multiplier, 0.f);
	Vertices[4].Position = FVector( 1.0f * multiplier, 0.0f * multiplier, 0.f);
	Vertices[5].Position = FVector( 1.0f * multiplier, 1.0f * multiplier, 0.f);

	Vertices[0].Color = FColor::Red;
	Vertices[1].Color = FColor::Red;
	Vertices[2].Color = FColor::Red;
	Vertices[3].Color = FColor::Blue;
	Vertices[4].Color = FColor::Blue;
	Vertices[5].Color = FColor::Blue;

	Vertices[0].UV = FVector( 1.0, 1.0, 0.0 );
	Vertices[1].UV = FVector( 0.0, 1.0, 0.0 );
	Vertices[2].UV = FVector( 0.0, 0.0, 0.0 );
	Vertices[3].UV = FVector( 0.0, 0.0, 0.0 );
	Vertices[4].UV = FVector( 1.0, 0.0, 0.0 );
	Vertices[5].UV = FVector( 1.0, 1.0, 0.0 );
	
// Initialize GPU resource
	FRHIResourceCreateInfo CreateInfo(TEXT("SimpleVertexBuffer"));
	FVertexBufferRHIRef VertexBufferRHI;
	VertexBufferRHI = RHICreateVertexBuffer(
		Vertices.Num() * sizeof(FSimpleVertex),
		BUF_Static,
		CreateInfo
        );

	void* Buffer = RHILockVertexBuffer(VertexBufferRHI, 0, Vertices.Num() * sizeof(FSimpleVertex), RLM_WriteOnly);
	FMemory::Memcpy(Buffer, Vertices.GetData(), Vertices.Num() * sizeof(FSimpleVertex));
	RHIUnlockVertexBuffer(VertexBufferRHI);

    // 1. Получаем шейдеры
    TShaderMapRef<FDebugShader> VertexShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
    TShaderMapRef<FMyPS> PixelShader(GetGlobalShaderMap(GMaxRHIFeatureLevel));
	
    // 2. Начинаем рендер пасс
    FRHIRenderPassInfo RenderPassInfo(RenderTarget, ERenderTargetActions::DontLoad_Store);
    RHICmdList.BeginRenderPass(RenderPassInfo, TEXT("SimpleRenderPass"));

    // 3. Настраиваем вьюпорт
	FIntPoint TargetSize(1920, 1080); // Или ваш размер
    RHICmdList.SetViewport(0, 0, 0.0f, TargetSize.X, TargetSize.Y, 1.0f);

    // 4. Настраиваем графический пайплайн
    FGraphicsPipelineStateInitializer GraphicsPSOInit;
    RHICmdList.ApplyCachedRenderTargets(GraphicsPSOInit);
    
    GraphicsPSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI();
    GraphicsPSOInit.BlendState = TStaticBlendState<>::GetRHI();
    GraphicsPSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI();
    GraphicsPSOInit.PrimitiveType = PT_TriangleList;

	FVertexDeclarationElementList Elements;
	Elements.Add(FVertexElement(0, STRUCT_OFFSET(FSimpleVertex, Position), VET_Float3, 0, sizeof(FSimpleVertex)));
	Elements.Add(FVertexElement(0, STRUCT_OFFSET(FSimpleVertex, Color), VET_Color, 1, sizeof(FSimpleVertex)));
	Elements.Add(FVertexElement(0, STRUCT_OFFSET(FSimpleVertex, UV), VET_Float2, 2, sizeof(FSimpleVertex)));
	FVertexDeclarationRHIRef VertexDecl = RHICreateVertexDeclaration(Elements);
	
//    GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GEmptyVertexDeclaration.VertexDeclarationRHI;
	GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = VertexDecl;
    GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
    GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();

    SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

	FMyUniformBufferStruct UniformData;
//	UniformData.SomeVector = FVector4(1,0.5,0,1);
	for( int i = 0; i < 4; ++i ) {
		UniformData.ModelMatrices[i] = FMatrix(
			FPlane(i * 0.1f, 0.0f, 0.0f, 0.0f), // Row 0
			FPlane(0.0f, i * 0.1f, 0.0f, 0.0f), // Row 1
			FPlane(0.0f, 0.0f, i * 0.1f, 0.0f), // Row 2
			FPlane(i * 0.2, 0.3f, 0.0f, 1.0f)  // Row 3
			);
//		UniformData.ModelMatrices[i] = (float)i * 0.2f;
	}

	FUniformBufferRHIRef UniformBuffer =
		RHICreateUniformBuffer(&UniformData, FMyUniformBufferStruct::StaticStructMetadata.GetLayout(), UniformBuffer_SingleDraw);
	
	VertexShader->SetParameters(RHICmdList, VertexShader, UniformBuffer);
	PixelShader->SetParameters(RHICmdList, PixelShader, UniformBuffer);

// Set stream source
	RHICmdList.SetStreamSource(0, VertexBufferRHI, 0);
	
    // 5. Рисуем 3 вершины (полноэкранный треугольник)
    RHICmdList.DrawPrimitive(0, 2, 1);

    // 6. Заканчиваем рендер пасс
    RHICmdList.EndRenderPass();
	UE_LOG(LogTemp, Log, TEXT("RENDER SIMPLE PASS"));
}

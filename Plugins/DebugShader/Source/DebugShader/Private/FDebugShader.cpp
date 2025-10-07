// Fill out your copyright notice in the Description page of Project Settings.


#include "FDebugShader.h"

FDebugShader::FDebugShader() {}

FDebugShader::FDebugShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
	: FGlobalShader(Initializer) {}

bool FDebugShader::ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
{
	return true; // Always compile
}

IMPLEMENT_SHADER_TYPE(, FDebugShader, TEXT("/Shaders/Debug.usf"), TEXT("MainVS"), SF_Vertex);

FMyPS::FMyPS() {}

FMyPS::FMyPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
	: FGlobalShader(Initializer)
{}

bool FMyPS::ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
{
	return true;
}

IMPLEMENT_SHADER_TYPE(, FMyPS, TEXT("/Shaders/Debug.usf"), TEXT("MainPS"), SF_Pixel);

void RenderSimplePass(FRHICommandListImmediate& RHICmdList, FTexture2DRHIRef RenderTarget)
{
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
    
    GraphicsPSOInit.BoundShaderState.VertexDeclarationRHI = GEmptyVertexDeclaration.VertexDeclarationRHI;
    GraphicsPSOInit.BoundShaderState.VertexShaderRHI = VertexShader.GetVertexShader();
    GraphicsPSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader();

    SetGraphicsPipelineState(RHICmdList, GraphicsPSOInit);

    // 5. Рисуем 3 вершины (полноэкранный треугольник)
    RHICmdList.DrawPrimitive(0, 1, 3);

    // 6. Заканчиваем рендер пасс
    RHICmdList.EndRenderPass();
	UE_LOG(LogTemp, Log, TEXT("RENDER SIMPLE PASS"));
}

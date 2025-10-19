// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "CommonRenderResources.h"
#include "ShaderParameters.h"

BEGIN_UNIFORM_BUFFER_STRUCT(FMyUniformBufferStruct, )
	SHADER_PARAMETER_ARRAY(FMatrix, ModelMatrices, [4])
END_UNIFORM_BUFFER_STRUCT()

class DEBUGSHADER_API FDebugShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FDebugShader, Global);
  public:
	FDebugShader();
	FDebugShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

	/* static bool ShouldCache(EShaderPlatform Platform) */
	/* { */
	/* 	return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5); */
	/* } */
	
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters);
	void SetParameters(FRHICommandListImmediate& RHICmdList,
					   const TShaderMapRef<FDebugShader>& VertexShader,
					   const FUniformBufferRHIRef& MyUB);

  private:
	LAYOUT_FIELD(FShaderUniformBufferParameter, MyUniformBuffer);
};

class DEBUGSHADER_API FMyPS : public FGlobalShader
{
    DECLARE_SHADER_TYPE(FMyPS, Global);

public:
    FMyPS();
    FMyPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters);
	void SetParameters(FRHICommandListImmediate& RHICmdList,
					   const TShaderMapRef<FMyPS>& PixelShader,
					   const FUniformBufferRHIRef& MyUB);

  private:
	LAYOUT_FIELD(FShaderUniformBufferParameter, MyUniformBuffer);
};

struct FSimpleVertex
{
	FVector Position;
	FColor  Color;
	FVector UV;
};

void RenderSimplePass(FRHICommandListImmediate& RHICmdList, FTexture2DRHIRef RenderTarget);

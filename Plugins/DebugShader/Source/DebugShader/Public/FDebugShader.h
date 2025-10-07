// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
#include "CommonRenderResources.h"
/* #include "Engine/TextureRenderTarget2D.h" */
/* #include "CanvasTypes.h" */
/* #include "Engine/Canvas.h" */

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
};

/* IMPLEMENT_SHADER_TYPE(, FDebugShader, TEXT("/Plugins/Resources/Debug.usf"), TEXT("MainVS"), SF_Vertex); */

class DEBUGSHADER_API FMyPS : public FGlobalShader
{
    DECLARE_SHADER_TYPE(FMyPS, Global);

public:
    FMyPS();
    FMyPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer);

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters);
};

/* UTextureRenderTarget2D* CreateRenderTarget(UObject* Outer, int32 Width, int32 Height) */
/* { */
/*     UTextureRenderTarget2D* RenderTarget = NewObject<UTextureRenderTarget2D>(Outer); */
/*     RenderTarget->RenderTargetFormat = RTF_RGBA8; */
/*     RenderTarget->InitAutoFormat(Width, Height); */
/*     RenderTarget->UpdateResourceImmediate(true); */

/* 	// Must be in another place */
/* 	FTextureRenderTargetResource* RTResource = RenderTarget->GameThread_GetRenderTargetResource(); */
/* 	FRHITexture* TextureRHI = RTResource->GetRenderTargetTexture(); */
	
/*     return RenderTarget; */
/* } */

void RenderSimplePass(FRHICommandListImmediate& RHICmdList, FTexture2DRHIRef RenderTarget);

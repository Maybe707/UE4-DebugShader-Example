// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GlobalShader.h"
/* #include "Engine/TextureRenderTarget2D.h" */
/* #include "CanvasTypes.h" */
/* #include "Engine/Canvas.h" */

class DEBUGSHADER_API FDebugShader : public FGlobalShader
{
	DECLARE_SHADER_TYPE(FDebugShader, Global);
public:
	FDebugShader() {}
	FDebugShader(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer) {}

	/* static bool ShouldCache(EShaderPlatform Platform) */
	/* { */
	/* 	return IsFeatureLevelSupported(Platform, ERHIFeatureLevel::SM5); */
	/* } */
	
	static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return true; // Always compile
    }
};

IMPLEMENT_SHADER_TYPE(, FDebugShader, TEXT("/Shaders/Debug.usf"), TEXT("MainVS"), SF_Vertex);

/* IMPLEMENT_SHADER_TYPE(, FDebugShader, TEXT("/Plugins/Resources/Debug.usf"), TEXT("MainVS"), SF_Vertex); */

class DEBUGSHADER_API FMyPS : public FGlobalShader
{
    DECLARE_SHADER_TYPE(FMyPS, Global);

public:
    FMyPS() {}
    FMyPS(const ShaderMetaType::CompiledShaderInitializerType& Initializer)
        : FGlobalShader(Initializer)
    {}

    static bool ShouldCompilePermutation(const FGlobalShaderPermutationParameters& Parameters)
    {
        return true;
    }
};

IMPLEMENT_SHADER_TYPE(, FMyPS, TEXT("/Shaders/Debug.usf"), TEXT("MainPS"), SF_Pixel);

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

/* void DrawToRenderTarget(UObject* WorldContextObject, UTextureRenderTarget2D* RenderTarget) */
/* { */
/* 	if (!RenderTarget) return; */
	    
/* 	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject); */
/* 	FTextureRenderTargetResource* RTResource = RenderTarget->GameThread_GetRenderTargetResource(); */
	 
/* 	FCanvas Canvas(RTResource, nullptr, World, World->FeatureLevel); */
	    
/* 	// Clear to blue */
/* 	Canvas.Clear(FLinearColor::Blue); */
	 
/* 	// Draw white rectangle in center */
/* 	FVector2D Position(RenderTarget->SizeX / 4, RenderTarget->SizeY / 4); */
/* 	FVector2D Size(RenderTarget->SizeX / 2, RenderTarget->SizeY / 2); */
/* 	FCanvasTileItem TileItem(Position, Size, FLinearColor::White); */
/* 	TileItem.BlendMode = SE_BLEND_Translucent; */
/* 	Canvas.DrawItem(TileItem); */
	 
/* 	Canvas.Flush_GameThread(); */
/* 	RenderTarget->UpdateResourceImmediate(false); */
/* } */

/* ENQUEUE_RENDER_COMMAND(RenderToTexture)( */
/*     [TextureRHI, RTResource](FRHICommandListImmediate& RHICmdList) */
/*     { */
/*         // Setup render pass info: clear on start, store results */
/*         FRHIRenderPassInfo RPInfo(TextureRHI, ERenderTargetActions::Clear_Store); */
/*         RHICmdList.BeginRenderPass(RPInfo, TEXT("RenderToTexturePass")); */

/*         // Set viewport to texture size */
/*         RHICmdList.SetViewport(0, 0, 0, RTResource->GetSizeX(), RTResource->GetSizeY(), 1.0f); */

/*         // Here you bind shaders and set pipeline */
/*         // Example uses engine fullscreen vertex shader + custom pixel shader */

/*         TShaderMapRef<FMyCustomPS> PixelShader(GetGlobalShaderMap(GMaxRHIFeatureLevel)); */

/*         FGraphicsPipelineStateInitializer PSOInit; */
/*         RHICmdList.ApplyCachedRenderTargets(PSOInit); */
/*         PSOInit.BlendState = TStaticBlendState<>::GetRHI(); */
/*         PSOInit.RasterizerState = TStaticRasterizerState<>::GetRHI(); */
/*         PSOInit.DepthStencilState = TStaticDepthStencilState<false, CF_Always>::GetRHI(); */

/*         PSOInit.BoundShaderState.VertexDeclarationRHI = GFilterVertexDeclaration.VertexDeclarationRHI; */
/*         PSOInit.BoundShaderState.VertexShaderRHI = nullptr; // default fullscreen VS */
/*         PSOInit.BoundShaderState.PixelShaderRHI = PixelShader.GetPixelShader(); */
/*         PSOInit.PrimitiveType = PT_TriangleList; */

/*         SetGraphicsPipelineState(RHICmdList, PSOInit); */

/*         RHICmdList.SetStreamSource(0, GFilterVertexBuffer.VertexBufferRHI, 0); */
        
/*         // Draw fullscreen quad (2 triangles) */
/*         RHICmdList.DrawPrimitive(0, 2, 1); */

/*         RHICmdList.EndRenderPass(); */
/*     }); */

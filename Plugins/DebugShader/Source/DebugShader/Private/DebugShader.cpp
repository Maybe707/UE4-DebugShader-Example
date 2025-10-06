// Copyright Epic Games, Inc. All Rights Reserved.

#include "DebugShader.h"
#include "Interfaces/IPluginManager.h"
#include "EngineModule.h"
#include "FDebugShader.h"
#include "RendererInterface.h"
#include "RenderGraphUtils.h"
#include "Engine/TextureRenderTarget2D.h"
#include "RHIResources.h"

#define LOCTEXT_NAMESPACE "FDebugShaderModule"

static UTextureRenderTarget2D* GMyRenderTarget = nullptr;
static FTexture2DRHIRef GMyRenderTargetRHI;

void CreateMyRenderTarget()
{
    if (GMyRenderTarget)
        return;

    GMyRenderTarget = NewObject<UTextureRenderTarget2D>(GetTransientPackage());
    GMyRenderTarget->RenderTargetFormat = RTF_RGBA8;
    GMyRenderTarget->InitAutoFormat(1920, 1080);
    GMyRenderTarget->ClearColor = FLinearColor::Black;
    GMyRenderTarget->UpdateResource();

    // Grab the underlying RHI texture
//    GMyRenderTargetRHI = GMyRenderTarget->GetRenderTargetResource()->GetRenderTargetTexture();
}

void FDebugShaderModule::StartupModule()
{
	    // Get the plugin's base directory
    FString PluginShaderDir = FPaths::Combine(
        IPluginManager::Get().FindPlugin(TEXT("DebugShader"))->GetBaseDir(),
        TEXT("Shaders")
    );
    
    // Map the virtual shader path to the physical directory
    AddShaderSourceDirectoryMapping(TEXT("/Shaders"), PluginShaderDir);

	//   // 1. Create a static delegate handle (optional if you want to unregister later)
    // static FDelegateHandle PostOpaqueHandle;

    // // 2. Register delegate
    // GetRendererModule().RegisterPostOpaqueRenderDelegate(
    //     FPostOpaqueRenderDelegate::CreateLambda([](FPostOpaqueRenderParameters& Parameters)
	// 		{
	// 			FRHICommandListImmediate* RHICmdList = Parameters.RHICmdList;
	// 			FRHITexture2D* Target = Parameters.NormalTexture;
	// 			if (!Target)
	// 				return;

	// 			FTexture2DRHIRef RenderTarget = Target;
	// 			RenderSimplePass(*RHICmdList, RenderTarget);
	// 		})
	// 	);

	FCoreDelegates::OnPostEngineInit.AddLambda([]()
		{
			CreateMyRenderTarget();
		});
	
    GetRendererModule().RegisterPostOpaqueRenderDelegate(
        FPostOpaqueRenderDelegate::CreateLambda([](FPostOpaqueRenderParameters& Parameters)
        {
            if (!GMyRenderTargetRHI)
                return;

            FRHICommandListImmediate& RHICmdList = *Parameters.RHICmdList;
			FTextureRenderTargetResource* RTResource = GMyRenderTarget->GetRenderTargetResource();
			GMyRenderTargetRHI = RTResource->GetRenderTargetTexture();
            // Run your custom render code
            RenderSimplePass(RHICmdList, GMyRenderTargetRHI);
        })
    );

	FCoreDelegates::OnPostEngineInit.AddLambda([]()
		{
			// Load your base material
			UMaterial* BaseMaterial = LoadObject<UMaterial>(
				nullptr, TEXT("/Game/M_DebugView.M_DebugView_Inst") // path to your material
				);

			// Create a dynamic instance
			UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BaseMaterial, GetTransientPackage());
			MID->SetTextureParameterValue(FName("RenderTexture"), GMyRenderTarget);
		});
	
	UE_LOG(LogTemp, Log, TEXT("DebugShader module started"));
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FDebugShaderModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDebugShaderModule, DebugShader)

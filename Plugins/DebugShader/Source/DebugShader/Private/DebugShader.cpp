// Copyright Epic Games, Inc. All Rights Reserved.

#include "DebugShader.h"
#include "Interfaces/IPluginManager.h"
#include "EngineModule.h"
#include "FDebugShader.h"
#include "RendererInterface.h"
#include "RenderGraphUtils.h"
#include "Engine/TextureRenderTarget2D.h"
#include "RHIResources.h"
#include "UObject/Package.h"
#include "Misc/PackageName.h"
#include "HAL/FileManager.h"
#include "UObject/SavePackage.h"
#include "AssetRegistryModule.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Modules/ModuleManager.h"
#include "Misc/FileHelper.h"
#include "ImageUtils.h"

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
	GMyRenderTarget->AddToRoot();

	
	// FString PackageName = TEXT("/Game/MyFolder/MyAssetName");

	// UPackage* Package = CreatePackage(nullptr, *PackageName);
	// Package->FullyLoad();

	// FString AssetName = FPackageName::GetLongPackageAssetName(PackageName);
    // NewTexture->Rename(*AssetName, Package);
    // NewTexture->SetFlags(RF_Public | RF_Standalone);
	
	// // Mark dirty and notify
	// Package->MarkPackageDirty();
	// FAssetRegistryModule::AssetCreated(NewTexture);

	// // Save the asset to disk
	// FString FilePath = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
	// UPackage::SavePackage(Package, NewTexture, RF_Public | RF_Standalone, *FilePath);
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
	UE_LOG(LogTemp, Log, TEXT("DEBUG SHADER MODULE STARTED"));
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

// Use OnBeginFrame instead of OnPostEngineInit for better timing
    FCoreDelegates::OnBeginFrame.AddLambda([]() 
		{
			static bool bInitialized = false;
			if (!bInitialized)
				{
					bInitialized = true;
            
					// Create render target first
					CreateMyRenderTarget();

					ENQUEUE_RENDER_COMMAND(MyRenderCommand)(
						[](FRHICommandListImmediate& RHICmdList)
							{
								GMyRenderTarget->RenderTargetFormat = RTF_RGBA16f;
								FTextureRenderTargetResource* RTResource0 = GMyRenderTarget->GetRenderTargetResource();
								GMyRenderTargetRHI = RTResource0->GetRenderTargetTexture();
								// Run your custom render code
								RenderSimplePass(RHICmdList, GMyRenderTargetRHI);
							}
						);
					
					FTextureRenderTargetResource* RTResource = GMyRenderTarget->GameThread_GetRenderTargetResource();

					TArray<FColor> Bitmap;
					RTResource->ReadPixels(Bitmap);

// Now you can build a UTexture2D
					UTexture2D* NewTexture = UTexture2D::CreateTransient(1920, 1080, PF_B8G8R8A8);
					NewTexture->SRGB = false;
					NewTexture->Filter = TF_Nearest;
					NewTexture->AddToRoot();
					void* TextureData = NewTexture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
					FMemory::Memcpy(TextureData, Bitmap.GetData(), Bitmap.Num() * sizeof(FColor));
					NewTexture->PlatformData->Mips[0].BulkData.Unlock();
					NewTexture->UpdateResource();

					// FTexture2DMipMap& Mip = NewTexture->PlatformData->Mips[0];
					// void* TextureData = Mip.BulkData.Lock(LOCK_READ_WRITE);

					// int32 MaxTileIndex = 1920 * 1080;
					// float* DestPtr = static_cast<float*>(TextureData);
					// for ( int32 i = 0; i < 1920 * 1080; ++i ) {
					// 	float Index = FMath::RandRange( 0.0, 4 );
					// 	DestPtr[i] = Index;   // R
					// 	// DestPtr[i * 4 + 1] = 0;       // G
					// 	// DestPtr[i * 4 + 2] = 0;       // B
					// 	// DestPtr[i * 4 + 3] = 0;       // A
					// }

					// Mip.BulkData.Unlock();
					// NewTexture->UpdateResource();

					// Make sure texture is fully loaded


					int32 Width = 1920;
					int32 Height = 1080;

					TArray<FLinearColor> ReferencePixels;
					ReferencePixels.SetNum(Width * Height);

					for (int32 Y = 0; Y < Height; ++Y)
						{
							for (int32 X = 0; X < Width; ++X)
								{
									float U = float(X) / (Width - 1);  // 0..1
									float V = float(Y) / (Height - 1); // 0..1
									// В UE4 текстура обычно Y=0 снизу, Y=1 сверху
									// Если нужно инвертировать: V = 1.0f - V;
									// UE_LOG(LogTemp, Log, TEXT("U: %f"), U);
									// UE_LOG(LogTemp, Log, TEXT("V: %f"), V);
									ReferencePixels[Y * Width + X] = FLinearColor(U, V, 0, 1);
								}
						}					

					float MaxDiff = 0.0f;
					for (int32 i = 0; i < Width * Height; ++i)
						{
							FVector2D RefUV(ReferencePixels[i].R, ReferencePixels[i].G);
							FVector2D RendUV(Bitmap[i].R, Bitmap[i].G);

							// UE_LOG(LogTemp, Log, TEXT("RefUV: %f"), RefUV[0]);
							// UE_LOG(LogTemp, Log, TEXT("RendUV: %f"), RendUV[0]);
							
							float Diff = FVector2D::Distance(RefUV, RendUV);
							MaxDiff = FMath::Max(MaxDiff, Diff);
						}

					UE_LOG(LogTemp, Log, TEXT("Max UV difference: %f"), MaxDiff);
					
					// Access pixel data
					FTexture2DMipMap& Mip1 = NewTexture->PlatformData->Mips[0];
					void* Data = Mip1.BulkData.Lock(LOCK_READ_ONLY);

					TArray<FColor> Pixels;
					Pixels.SetNumUninitialized(Width * Height);
					FMemory::Memcpy(Pixels.GetData(), Data, Pixels.Num() * sizeof(FColor));

					Mip1.BulkData.Unlock();

					for ( int i = 0; i < Width * Height; ++i ) {
						if( Pixels[i].G > 0 || Pixels[i].R > 0 ) {
							UE_LOG(LogTemp, Log, TEXT("PIXEL: %d"), Pixels[i].G);
							UE_LOG(LogTemp, Log, TEXT("PIXEL: %d"), Pixels[i].R);
						}
					}
					
					// Use Image Wrapper to encode PNG
					IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
					TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

					ImageWrapper->SetRaw(Pixels.GetData(), Pixels.GetAllocatedSize(), Width, Height, ERGBFormat::BGRA, 8);

					const TArray<unsigned char, TSizedDefaultAllocator<64>>& PNGData = ImageWrapper->GetCompressed();

					FFileHelper::SaveArrayToFile(PNGData, *(FPaths::ProjectDir() / TEXT("Saved/MyTexture.png")));

					
					// // Load your base material
					// UMaterial* BaseMaterial = LoadObject<UMaterial>(
					// 	nullptr, TEXT("/Game/M_DebugView") // path to your material
					// 	);

					// // Create a dynamic instance
					// UMaterialInstanceDynamic* MID = UMaterialInstanceDynamic::Create(BaseMaterial, GetTransientPackage());
					// MID->SetTextureParameterValue(FName("RenderTexture"), GMyRenderTarget);
				}
		});
	
	UE_LOG(LogTemp, Log, TEXT("DEBUG SHADER MODULE FINISHED"));
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FDebugShaderModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDebugShaderModule, DebugShader)

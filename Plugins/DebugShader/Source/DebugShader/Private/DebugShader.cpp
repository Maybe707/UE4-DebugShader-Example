// Copyright Epic Games, Inc. All Rights Reserved.

#include "DebugShader.h"
#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FDebugShaderModule"

void FDebugShaderModule::StartupModule()
{
	    // Get the plugin's base directory
    FString PluginShaderDir = FPaths::Combine(
        IPluginManager::Get().FindPlugin(TEXT("DebugShader"))->GetBaseDir(),
        TEXT("Shaders")
    );
    
    // Map the virtual shader path to the physical directory
    AddShaderSourceDirectoryMapping(TEXT("/Shaders"), PluginShaderDir);
	
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

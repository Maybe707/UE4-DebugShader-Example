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

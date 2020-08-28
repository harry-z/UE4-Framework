// Copyright Epic Games, Inc. All Rights Reserved.

#include "Math.h"

DEFINE_LOG_CATEGORY(LogMath)

#define LOCTEXT_NAMESPACE "FMathModule"

void FMathModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	bool bLogVerbosity = FParse::Param(FCommandLine::Get(), TEXT("LogVerbosity"));
	if (bLogVerbosity)
		UE_SET_LOG_VERBOSITY(LogMath, Verbose)
	else
		UE_SET_LOG_VERBOSITY(LogMath, Display)
}

void FMathModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMathModule, Math)
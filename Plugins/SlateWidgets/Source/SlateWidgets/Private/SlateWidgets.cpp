// Copyright Epic Games, Inc. All Rights Reserved.

#include "SlateWidgets.h"
#include "Misc/CommandLine.h"

DEFINE_LOG_CATEGORY(LogSlateWidgets)

#define LOCTEXT_NAMESPACE "FSlateWidgetsModule"

void FSlateWidgetsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	bool bLogVerbosity = FParse::Param(FCommandLine::Get(), TEXT("LogVerbosity"));
	if (bLogVerbosity)
		UE_SET_LOG_VERBOSITY(LogSlateWidgets, Verbose)
	else
		UE_SET_LOG_VERBOSITY(LogSlateWidgets, Display)
}

void FSlateWidgetsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FSlateWidgetsModule, SlateWidgets)



// Copyright Epic Games, Inc. All Rights Reserved.

#include "Network.h"
#include "HttpRequestBase.h"
#include "Misc/CommandLine.h"

DEFINE_LOG_CATEGORY(LogNetwork)

#define LOCTEXT_NAMESPACE "FNetworkModule"

void FNetworkModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	bool bLogVerbosity = FParse::Param(FCommandLine::Get(), TEXT("LogVerbosity"));
	if (bLogVerbosity)
		UE_SET_LOG_VERBOSITY(LogNetwork, Verbose)
	else
		UE_SET_LOG_VERBOSITY(LogNetwork, Display)
}

void FNetworkModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	Network::FHttpRequestBase::EmptyRequestList();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FNetworkModule, Network)



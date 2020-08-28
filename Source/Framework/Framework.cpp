// Copyright Epic Games, Inc. All Rights Reserved.

#include "Framework.h"
#include "ElementDrawer.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, Framework, "Framework" );

FFrameworkGameContent::FFrameworkGameContent()
: M_Type(EGameContentType::TypeUnknown) 
{}

FFrameworkGameContent::FFrameworkGameContent(EGameContentType InGameContentType, const FText& InDisplayName, const FString& InMapUrl)
: M_Type(InGameContentType)
, M_DisplayName(InDisplayName)
, M_MapUrl(InMapUrl)
{}

FFrameworkGameContent::~FFrameworkGameContent()
{}

static FFrameworkGameContentPtr S_GameContents[(int32)EGameContentType::TypeNum];

void InitializeGameContents() {
	for (int32 i = 0; i < (int32)EGameContentType::TypeNum; ++i)
		S_GameContents[i] = CreateGameContentInstance((EGameContentType)i);
}

void UninitializeGameContents() {
	for (int32 i = 0; i < (int32)EGameContentType::TypeNum; ++i)
		S_GameContents[i].Reset();
}

FFrameworkGameContentRef GetGameContent(EGameContentType InGameContentType) {
	return S_GameContents[(int32)InGameContentType].ToSharedRef();
}


TSharedPtr<FFrameworkGameContentFactory>* GetOrCreateFactories() {
	static TSharedPtr<FFrameworkGameContentFactory> S_GameContentFactory[(int32)EGameContentType::TypeNum];
	return &S_GameContentFactory[0];
}

void RegisterGameContentFactory(EGameContentType InGameContentType, FFrameworkGameContentFactory* InFactory) {
	GetOrCreateFactories()[(int32)InGameContentType] = MakeShareable(InFactory);
}

FFrameworkGameContentRef CreateGameContentInstance(EGameContentType InGameContentType) {
	return GetOrCreateFactories()[(int32)InGameContentType]->CreateInstance();
}

FFrameworkGlobalDelegates::FOnPreSwitchGameContent FFrameworkGlobalDelegates::S_OnPreSwitchGameContent;
FFrameworkGlobalDelegates::FOnPostSwitchGameContent FFrameworkGlobalDelegates::S_OnPostSwitchGameContent;
FFrameworkGlobalDelegates::FOnExitApp FFrameworkGlobalDelegates::S_OnExitApp;
Kernel::FKernelInputDelegates* FFrameworkGlobalDelegates::S_InputDelegates = nullptr;
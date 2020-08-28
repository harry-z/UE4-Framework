// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "KernelDelegates.h"

namespace FuncUtilities
{
	class FElementDrawer;
}

enum class EGameContentType {
	TypeUnknown = -1,
	Type2D = 0,
	Type3D,
	TypeNum
};

class FFrameworkGameContent;
using FFrameworkGameContentPtr = TSharedPtr<FFrameworkGameContent, ESPMode::ThreadSafe>;
using FFrameworkGameContentRef = TSharedRef<FFrameworkGameContent, ESPMode::ThreadSafe>;

class FFrameworkGameContent
{
public:
	FFrameworkGameContent();
	FFrameworkGameContent(EGameContentType InGameContentType, const FText& InDisplayName, const FString& InMapUrl);
	~FFrameworkGameContent();

	FORCEINLINE EGameContentType	GetType() const { return M_Type; }
	FORCEINLINE const FText&		GetDisplayName() const { return M_DisplayName; }
	FORCEINLINE const FString&		GetMapUrl() const { return M_MapUrl; }

private:
	EGameContentType				M_Type;
	FText							M_DisplayName;
	FString							M_MapUrl;
};

void InitializeGameContents();
void UninitializeGameContents();
FFrameworkGameContentRef GetGameContent(EGameContentType InGameContentType);

class FFrameworkGameContentFactory
{
public:
	virtual ~FFrameworkGameContentFactory() {}
	virtual FFrameworkGameContentRef CreateInstance() = 0;
};

void RegisterGameContentFactory(EGameContentType InGameContentType, FFrameworkGameContentFactory* InFactory);
FFrameworkGameContentRef CreateGameContentInstance(EGameContentType InGameContentType);

#define DECLARE_GAMECONTENT_FACTORY(GameContentType, Factory) \
	struct Factory##_Register { \
		Factory##_Register() { \
			RegisterGameContentFactory(GameContentType, new Factory); \
		} \
	};

#define DEFINE_GAMECONTENT_FACTORY(Factory) Factory##_Register Factory##Register;

class FFrameworkGlobalDelegates
{
public:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPreSwitchGameContent, EGameContentType)
	static FOnPreSwitchGameContent S_OnPreSwitchGameContent;
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnPostSwitchGameContent, EGameContentType)
	static FOnPostSwitchGameContent S_OnPostSwitchGameContent;
	DECLARE_MULTICAST_DELEGATE(FOnExitApp)
	static FOnExitApp S_OnExitApp;

	static Kernel::FKernelInputDelegates *S_InputDelegates;
};
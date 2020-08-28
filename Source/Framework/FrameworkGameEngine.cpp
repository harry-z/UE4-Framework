#include "FrameworkGameEngine.h"
#include "FrameworkGameInstance.h"
#include "FrameworkGameViewportClient.h"
#include "KernelDelegates.h"
#include "Subsystems/InputDelegateSubsystem.h"
#include "UI/SMainFrame.h"
#include "Framework/Application/SlateApplication.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/PlayerController.h"
#include "Misc/CoreDelegates.h"
#include "Engine/LocalPlayer.h"
#include "Engine/GameViewportClient.h"
#include "Slate/SceneViewport.h"

void UFrameworkGameEngine::Init(IEngineLoop* InEngineLoop)
{
	GetMutableDefault<UInputSettings>()->bCaptureMouseOnLaunch = false;

	GameViewportClientClass = UFrameworkGameViewportClient::StaticClass();
	UGameEngine::Init(InEngineLoop);

	InitializeGameContents();
	// 加载地图
	LoadGameContents();

	M_MainFrame = SNew(SMainFrame)
		.OnSwitchGameContent_UObject(this, &UFrameworkGameEngine::SwitchGameContent);

	GameViewportWindow.Pin()->SetOnWindowClosed(FOnWindowClosed::CreateUObject(this, &UFrameworkGameEngine::Callback_AppWindowClosed));
	FCoreDelegates::OnFEngineLoopInitComplete.AddUObject(this, &UFrameworkGameEngine::Callback_EngineLoopInitialized);
}

void UFrameworkGameEngine::PreExit()
{
	FFrameworkGlobalDelegates::S_OnExitApp.Broadcast();
	UninitializeGameContents();
	Super::PreExit();
}

void UFrameworkGameEngine::SwitchGameContent(EGameContentType InGameContentType)
{
	UWorld* DestWorld = M_GameContentWorldContext[(int32)InGameContentType]->World();
	if (DestWorld != GameInstance->GetWorldContext()->World())
	{
		FFrameworkGlobalDelegates::S_OnPreSwitchGameContent.Broadcast(InGameContentType);

		// 将当前的地图设置为不tick
		GameInstance->GetWorldContext()->World()->SetShouldTick(false);
		// 设置新的地图需要tick
		DestWorld->SetShouldTick(true);
		// 将新的地图设置到WorldContext中
		GameInstance->GetWorldContext()->SetCurrentWorld(DestWorld);
		GWorld = DestWorld;

		// 切换Player关联的PlayerController
		APlayerController* PC = DestWorld->GetFirstPlayerController();
		PC->SetPlayer(PC->GetLocalPlayer());

		// 切换输入响应代理
		FFrameworkGlobalDelegates::S_InputDelegates = GWorld->GetSubsystem<UFuncUtilInputDelegateSubsystem>()->GetInputDelegates();

		if (FSlateApplication::IsInitialized())
		{
			auto& SlateApp = FSlateApplication::Get();
			SlateApp.ReleaseAllPointerCapture();
			SlateApp.SetAllUserFocus(GameViewportWindow.Pin(), EFocusCause::SetDirectly);
		}

		FFrameworkGlobalDelegates::S_OnPostSwitchGameContent.Broadcast(InGameContentType);
	}
}

void UFrameworkGameEngine::LoadGameContents()
{
	int32 NumContents = (int32)EGameContentType::TypeNum;

	// 创建新的WorldContext容纳地图，不能用现有的WorldContext，因为现有的WorldContext会将现有World
	// 删除掉
	// LoadMap同时会创建GameMode和PlayerController，不同的Map创建的GameMode不同，设置在DefaultEngine.ini中
	FString Error;
	for (int32 i = 0; i < NumContents; ++i)
	{
		FFrameworkGameContentRef GameContent = GetGameContent((EGameContentType)i);
		FWorldContext& NewWorldContext = CreateNewWorldContext(EWorldType::Game);
		M_GameContentWorldContext[i] = &NewWorldContext;
		NewWorldContext.OwningGameInstance = GameInstance;
		// 因为我们的地图URL中没有任何IP和命令字段，所以实际上相当于Server Travel，所以设置TRAVEL_Absolute
		// 或TRAVEL_Relative没有太大的区别
		LoadMap(NewWorldContext, 
			FURL(nullptr, *(GameContent->GetMapUrl()), TRAVEL_Absolute),
			nullptr, 
			Error);
		NewWorldContext.World()->SetShouldTick(false);
		APlayerController *PC = NewWorldContext.World()->GetFirstPlayerController();
		if (PC)
			PC->bShowMouseCursor = 1;
	}
}

void UFrameworkGameEngine::Callback_EngineLoopInitialized()
{
	M_MainFrame->SetViewport(GameViewportWidget.ToSharedRef());
	auto Window = GameViewportWindow.Pin();
	Window->SetContent(M_MainFrame.ToSharedRef());

	SceneViewport->SetOnSceneViewportResizeDel(FOnSceneViewportResize::CreateLambda(
		[](FVector2D NewSize) {
		Kernel::FKernelDelegates::S_OnViewportResized.Broadcast(NewSize);
		}
	));

	Window->SlatePrepass();

	int32 Index = 0;
	SwitchGameContent((EGameContentType)Index);
}

void UFrameworkGameEngine::Callback_AppWindowClosed(const TSharedRef<SWindow>& WindowBeingClosed)
{
	M_MainFrame.Reset();
	Super::OnGameWindowClosed(WindowBeingClosed);
}
#include "FrameworkGameViewportClient.h"
#include "Subsystems/ElementDrawerSubsystem.h"
#include "Engine/World.h"

UFrameworkGameViewportClient::UFrameworkGameViewportClient()
{
	OnInputKey().AddUObject(this, &UFrameworkGameViewportClient::TranslateInputKey);
	OnInputAxis().AddUObject(this, &UFrameworkGameViewportClient::TranslateInputAxis);
}

void UFrameworkGameViewportClient::Init(FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice)
{
    Super::Init(WorldContext, OwningGameInstance, bCreateNewAudioDevice);
    SetCaptureMouseOnClick(EMouseCaptureMode::CaptureDuringMouseDown);
}

void UFrameworkGameViewportClient::MouseMove(FViewport* InViewport, int32 X, int32 Y)
{
	Super::MouseMove(InViewport, X, Y);
	FFrameworkGlobalDelegates::S_InputDelegates->M_OnMouseMove.Broadcast(InViewport, FIntPoint(X, Y), FIntPoint(X, Y) + InViewport->GetInitialPositionXY(), Kernel::MakeModifierKeyFlag(InViewport));
}

void UFrameworkGameViewportClient::CapturedMouseMove(FViewport* InViewport, int32 InMouseX, int32 InMouseY)
{
	Super::CapturedMouseMove(InViewport, InMouseX, InMouseY);
	FFrameworkGlobalDelegates::S_InputDelegates->M_OnCapturedMouseMove.Broadcast(InViewport, FIntPoint(InMouseX, InMouseY), FIntPoint(InMouseX, InMouseY) + InViewport->GetInitialPositionXY(), Kernel::MakeModifierKeyFlag(InViewport));
}

void UFrameworkGameViewportClient::FinalizeViews(FSceneViewFamily* ViewFamily, const TMap<ULocalPlayer*, FSceneView*>& PlayerViewMap)
{
    for (auto &View : ViewFamily->Views)
        const_cast<FSceneView*>(View)->Drawer = GetWorld()->GetSubsystem<UElementDrawerSubsystem>()->GetElementDrawer();
}

void UFrameworkGameViewportClient::TranslateInputKey(const FInputKeyEventArgs& EventArgs)
{
	int32 ModifierKeyFlag = Kernel::MakeModifierKeyFlag(EventArgs.Viewport);

    Kernel::FKernelInputDelegates* RESTRICT InputDelegates = FFrameworkGlobalDelegates::S_InputDelegates;
    if (EventArgs.Key == EKeys::LeftMouseButton)
    {
        if (EventArgs.Event == IE_Pressed)
            InputDelegates->M_OnLMouseButtonDown.Broadcast(EventArgs.Viewport, FIntPoint(EventArgs.Viewport->GetMouseX(), EventArgs.Viewport->GetMouseY()), FIntPoint(EventArgs.Viewport->GetMouseX(), EventArgs.Viewport->GetMouseY()) + EventArgs.Viewport->GetInitialPositionXY(), ModifierKeyFlag);
        else if (EventArgs.Event == IE_Released)
            InputDelegates->M_OnLMouseButtonUp.Broadcast(EventArgs.Viewport, FIntPoint(EventArgs.Viewport->GetMouseX(), EventArgs.Viewport->GetMouseY()), FIntPoint(EventArgs.Viewport->GetMouseX(), EventArgs.Viewport->GetMouseY()) + EventArgs.Viewport->GetInitialPositionXY(), ModifierKeyFlag);
    }
    else if (EventArgs.Key == EKeys::RightMouseButton)
    {
        if (EventArgs.Event == IE_Pressed)
            InputDelegates->M_OnRMouseButtonDown.Broadcast(EventArgs.Viewport, FIntPoint(EventArgs.Viewport->GetMouseX(), EventArgs.Viewport->GetMouseY()), FIntPoint(EventArgs.Viewport->GetMouseX(), EventArgs.Viewport->GetMouseY()) + EventArgs.Viewport->GetInitialPositionXY(), ModifierKeyFlag);
        else if (EventArgs.Event == IE_Released)
            InputDelegates->M_OnRMouseButtonUp.Broadcast(EventArgs.Viewport, FIntPoint(EventArgs.Viewport->GetMouseX(), EventArgs.Viewport->GetMouseY()), FIntPoint(EventArgs.Viewport->GetMouseX(), EventArgs.Viewport->GetMouseY()) + EventArgs.Viewport->GetInitialPositionXY(), ModifierKeyFlag);
    }
    else if (EventArgs.Key == EKeys::MiddleMouseButton)
    {
        if (EventArgs.Event == IE_Pressed)
            InputDelegates->M_OnMiddleMouseButtonDown.Broadcast(EventArgs.Viewport, FIntPoint(EventArgs.Viewport->GetMouseX(), EventArgs.Viewport->GetMouseY()), FIntPoint(EventArgs.Viewport->GetMouseX(), EventArgs.Viewport->GetMouseY()) + EventArgs.Viewport->GetInitialPositionXY(), ModifierKeyFlag);
        else if (EventArgs.Event == IE_Released)
            InputDelegates->M_OnMiddleMouseButtonUp.Broadcast(EventArgs.Viewport, FIntPoint(EventArgs.Viewport->GetMouseX(), EventArgs.Viewport->GetMouseY()), FIntPoint(EventArgs.Viewport->GetMouseX(), EventArgs.Viewport->GetMouseY()) + EventArgs.Viewport->GetInitialPositionXY(), ModifierKeyFlag);
    }
    else
    {
        if (EventArgs.Event == IE_Pressed)
            InputDelegates->M_OnKeyDown.Broadcast(EventArgs.Viewport, EventArgs.Key, ModifierKeyFlag);
        else if (EventArgs.Event == IE_Released)
            InputDelegates->M_OnKeyUp.Broadcast(EventArgs.Viewport, EventArgs.Key, ModifierKeyFlag);
    }
}

void UFrameworkGameViewportClient::TranslateInputAxis(FViewport* InViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	int32 ModifierKeyFlag = Kernel::MakeModifierKeyFlag(InViewport);

    Kernel::FKernelInputDelegates* RESTRICT InputDelegates = FFrameworkGlobalDelegates::S_InputDelegates;
    if (Key == EKeys::MouseWheelAxis)
        InputDelegates->M_OnMouseWheel.Broadcast(InViewport, Delta, ModifierKeyFlag);
}
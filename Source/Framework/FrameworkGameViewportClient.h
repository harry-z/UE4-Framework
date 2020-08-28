#pragma once
#include "Engine/GameViewportClient.h"
#include "Framework.h"
#include "FrameworkGameViewportClient.generated.h"

UCLASS()
class UFrameworkGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

public:
	UFrameworkGameViewportClient();
	virtual void Init(struct FWorldContext& WorldContext, UGameInstance* OwningGameInstance, bool bCreateNewAudioDevice) override;
	virtual void MouseMove(FViewport* InViewport, int32 X, int32 Y) override;
    virtual void CapturedMouseMove(FViewport* InViewport, int32 InMouseX, int32 InMouseY) override;
    virtual void FinalizeViews(class FSceneViewFamily* ViewFamily, const TMap<ULocalPlayer*, FSceneView*>& PlayerViewMap) override;

private:
	void TranslateInputKey(const FInputKeyEventArgs& EventArgs);
	void TranslateInputAxis(FViewport* InViewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad);
};
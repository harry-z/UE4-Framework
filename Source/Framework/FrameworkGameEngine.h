#pragma once

#include "Engine/GameEngine.h"
#include "Framework.h"
#include "FrameworkGameEngine.generated.h"

class SMainFrame;

UCLASS()
class UFrameworkGameEngine : public UGameEngine
{
	GENERATED_BODY()

public:
	virtual void Init(class IEngineLoop* InEngineLoop) override;
	virtual void PreExit() override;

private:
	void SwitchGameContent(EGameContentType InGameContentType);
	void LoadGameContents();

private:
	void Callback_EngineLoopInitialized();
	void Callback_AppWindowClosed(const TSharedRef<SWindow>& WindowBeingClosed);

private:
	TSharedPtr<SMainFrame>		M_MainFrame;
	FWorldContext*				M_GameContentWorldContext[(int32)EGameContentType::TypeNum];
};
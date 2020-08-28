#include "FrameworkDesignMode2D.h"
#include "Test2D.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"

DEFINE_AUTO_REG_DESIGN_MODE(UFrameworkDesignMode2D)

void UFrameworkDesignMode2D::PostInitProperties()
{
	Super::PostInitProperties();
	FFrameworkGlobalDelegates::S_OnPostSwitchGameContent.AddUObject(this, &UFrameworkDesignMode2D::Callback_PostSwitchGameContent);
	M_Test2D = MakeShareable(new FTest2D(GetWorld()));
}

void UFrameworkDesignMode2D::BeginDestroy()
{
	Super::BeginDestroy();
	M_Test2D.Reset();
}

void UFrameworkDesignMode2D::Callback_PostSwitchGameContent(EGameContentType InGameContentType)
{
	if (InGameContentType == EGameContentType::Type2D)
	{
		UGameViewportClient* ViewportClient = GWorld->GetGameInstance()->GetGameViewportClient();
		FEngineShowFlags* EngineShowFlags = ViewportClient->GetEngineShowFlags();
		EngineShowFlags->SetLensFlares(false);
		EngineShowFlags->SetEyeAdaptation(false);
		EngineShowFlags->SetColorGrading(false);
		EngineShowFlags->SetCameraImperfections(false);
		EngineShowFlags->SetDepthOfField(false);
		EngineShowFlags->SetVignette(false);
		EngineShowFlags->SetGrain(false);
		EngineShowFlags->SetSeparateTranslucency(false);
		EngineShowFlags->SetScreenPercentage(false);
		EngineShowFlags->SetScreenSpaceReflections(false);
		EngineShowFlags->SetAmbientOcclusion(false);
		EngineShowFlags->SetIndirectLightingCache(false);
		EngineShowFlags->SetLightShafts(false);
		EngineShowFlags->SetPostProcessMaterial(false);
		EngineShowFlags->SetDistanceFieldAO(false);
		EngineShowFlags->SetDistanceFieldGI(false);
	}
}
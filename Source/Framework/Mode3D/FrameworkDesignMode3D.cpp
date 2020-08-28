#include "FrameworkDesignMode3D.h"
#include "Test3D.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"

DEFINE_AUTO_REG_DESIGN_MODE(UFrameworkDesignMode3D)

void UFrameworkDesignMode3D::PostInitProperties()
{
	Super::PostInitProperties();
	FFrameworkGlobalDelegates::S_OnPostSwitchGameContent.AddUObject(this, &UFrameworkDesignMode3D::Callback_PostSwitchGameContent);
	M_Test3D = MakeShareable(new FTest3D);
}

void UFrameworkDesignMode3D::BeginDestroy()
{
	Super::BeginDestroy();
	M_Test3D.Reset();
}

void UFrameworkDesignMode3D::Callback_PostSwitchGameContent(EGameContentType InGameContentType)
{
	if (InGameContentType == EGameContentType::Type3D)
	{
		UGameViewportClient* ViewportClient = GWorld->GetGameInstance()->GetGameViewportClient();
		FEngineShowFlags* EngineShowFlags = ViewportClient->GetEngineShowFlags();
		EngineShowFlags->SetLensFlares(true);
		EngineShowFlags->SetEyeAdaptation(true);
		EngineShowFlags->SetColorGrading(true);
		EngineShowFlags->SetCameraImperfections(true);
		EngineShowFlags->SetDepthOfField(true);
		EngineShowFlags->SetVignette(true);
		EngineShowFlags->SetGrain(true);
		EngineShowFlags->SetSeparateTranslucency(true);
		EngineShowFlags->SetScreenPercentage(true);
		EngineShowFlags->SetScreenSpaceReflections(true);
		EngineShowFlags->SetAmbientOcclusion(true);
		EngineShowFlags->SetIndirectLightingCache(true);
		EngineShowFlags->SetLightShafts(true);
		EngineShowFlags->SetPostProcessMaterial(true);
		EngineShowFlags->SetDistanceFieldAO(true);
		EngineShowFlags->SetDistanceFieldGI(true);

		M_Test3D->NotifySwitchGameContent();
	}
}
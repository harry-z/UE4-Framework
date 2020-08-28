#pragma once
#include "../DesignMode.h"
#include "../Framework.h"
#include "FrameworkDesignMode3D.generated.h"

UCLASS()
class UFrameworkDesignMode3D final : public UFrameworkDesignMode
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;
	virtual void BeginDestroy() override;

public:
	void Callback_PostSwitchGameContent(EGameContentType InGameContentType);

private:
	TSharedPtr<class FTest3D> M_Test3D;
};

DECLARE_AUTO_REG_DESIGN_MODE(TEXT("Map3D"), UFrameworkDesignMode3D)
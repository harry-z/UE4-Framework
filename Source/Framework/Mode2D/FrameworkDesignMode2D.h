#pragma once
#include "../DesignMode.h"
#include "../Framework.h"
#include "FrameworkDesignMode2D.generated.h"

UCLASS()
class UFrameworkDesignMode2D final : public UFrameworkDesignMode
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;
	virtual void BeginDestroy() override;

public:
	void Callback_PostSwitchGameContent(EGameContentType InGameContentType);

private:
	TSharedPtr<class FTest2D> M_Test2D;
};

DECLARE_AUTO_REG_DESIGN_MODE(TEXT("Map2D"), UFrameworkDesignMode2D)
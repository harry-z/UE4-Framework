#pragma once

#include "Engine/GameInstance.h"
#include "FrameworkGameInstance.generated.h"

UCLASS()
class UFrameworkGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void OnStart() override;
};
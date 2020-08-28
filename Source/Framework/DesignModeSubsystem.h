#pragma once
#include "Subsystems/WorldSubsystem.h"
#include "DesignMode.h"
#include "DesignModeSubsystem.generated.h"

UCLASS()
class UFrameworkDesignModeSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override {}

private:
	UPROPERTY()
	UFrameworkDesignMode* M_DesignMode;
};
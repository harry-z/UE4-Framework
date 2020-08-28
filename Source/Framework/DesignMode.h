#pragma once
#include "UObject/Object.h"
#include "Engine/EngineBaseTypes.h"
#include "Templates/SubclassOf.h"
#include "DesignMode.generated.h"

UCLASS(Abstract)
class UFrameworkDesignMode : public UObject
{
	GENERATED_BODY()
};

typedef UFrameworkDesignMode*(*CreateDesignModeFunc)(UObject*);
void RegisterDesignMode(const FString& MapName, CreateDesignModeFunc CreateFunc);
CreateDesignModeFunc GetDesignModeClass(const FString& MapName);

#define DECLARE_AUTO_REG_DESIGN_MODE(MapName, DesignModeClass) \
	inline DesignModeClass* Create##DesignModeClass(UObject* InOuter) { \
		return NewObject<DesignModeClass>(InOuter, DesignModeClass::StaticClass()); } \
	struct FAutoReg##DesignModeClass { \
		FAutoReg##DesignModeClass() { RegisterDesignMode(MapName, (CreateDesignModeFunc)Create##DesignModeClass); } \
	};

#define DEFINE_AUTO_REG_DESIGN_MODE(DesignModeClass) FAutoReg##DesignModeClass AutoReg##DesignModeClass;
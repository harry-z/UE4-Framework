#pragma once
#include "Subsystems/WorldSubsystem.h"
#include "../ElementDrawer.h"
#include "ElementDrawerSubsystem.generated.h"

UCLASS()
class FUNCUTILITIES_API UElementDrawerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UElementDrawerSubsystem() : UWorldSubsystem() {}
	virtual void Initialize(FSubsystemCollectionBase& Collection) override {
		M_ElementDrawer = new FuncUtilities::FElementDrawer;
	}
	virtual void Deinitialize() override {
		if (M_ElementDrawer != nullptr)
			delete M_ElementDrawer;
	}
	FORCEINLINE FuncUtilities::FElementDrawer* GetElementDrawer() { return M_ElementDrawer; }

private:
	FuncUtilities::FElementDrawer* M_ElementDrawer = nullptr;
};
#pragma once
#include "Subsystems/WorldSubsystem.h"
#include "KernelDelegates.h"
#include "InputDelegateSubsystem.generated.h"

UCLASS()
class FUNCUTILITIES_API UFuncUtilInputDelegateSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UFuncUtilInputDelegateSubsystem() : UWorldSubsystem() {}
	virtual void Initialize(FSubsystemCollectionBase& Collection) override {
		M_InputDelegates = new Kernel::FKernelInputDelegates;
	}
	virtual void Deinitialize() override {
		if (M_InputDelegates != nullptr)
			delete M_InputDelegates;
	}
	FORCEINLINE Kernel::FKernelInputDelegates* GetInputDelegates() { return M_InputDelegates; }

private:
	Kernel::FKernelInputDelegates* M_InputDelegates = nullptr;
};
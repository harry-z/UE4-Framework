#include "DesignModeSubsystem.h"

void UFrameworkDesignModeSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	CreateDesignModeFunc CreateFunc = GetDesignModeClass(GetWorld()->GetFName().ToString());
	if (CreateFunc && M_DesignMode == nullptr)
	{
		M_DesignMode = (*CreateFunc)(this);
	}
}
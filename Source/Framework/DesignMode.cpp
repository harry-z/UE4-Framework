#include "DesignMode.h"

using DesignModeInfoMap = TMap<FString, CreateDesignModeFunc>;

DesignModeInfoMap& GetDesignModeInfoMap() {
	static DesignModeInfoMap S_DesignModeInfoMap;
	return S_DesignModeInfoMap;
}

void RegisterDesignMode(const FString& MapName, CreateDesignModeFunc CreateFunc)
{
	DesignModeInfoMap& InfoMap = GetDesignModeInfoMap();
	if (!InfoMap.Contains(MapName))
		InfoMap.Add(MapName, CreateFunc);
}

CreateDesignModeFunc GetDesignModeClass(const FString& MapName)
{
	CreateDesignModeFunc* Result = GetDesignModeInfoMap().Find(MapName);
	return Result ? *Result : nullptr;
}
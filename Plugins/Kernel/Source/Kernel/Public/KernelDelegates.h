#pragma once
#include "Delegates/DelegateCombinations.h"


namespace Kernel
{
	class KERNEL_API FKernelDelegates
	{
	public:
		DECLARE_MULTICAST_DELEGATE_OneParam(FOnViewportResized, FVector2D)
		static FOnViewportResized S_OnViewportResized;
	};

	enum EModifierKey {
		EModifierKey_Ctrl = 0x01,
		EModifierKey_Shift = 0x02,
		EModifierKey_Alt = 0x04
	};

	KERNEL_API int32 MakeModifierKeyFlag(FViewport* InViewport);

	class FKernelInputDelegates
	{
	public:
		// 下面所有的代理，第一个FIntPoint指相对于视口的鼠标坐标，第二个FIntPoint是加上了视口位置的鼠标坐标，即相对于窗口的鼠标坐标
		DECLARE_MULTICAST_DELEGATE_FourParams(FOnMouseMove, FViewport*, FIntPoint, FIntPoint, int32)
		DECLARE_MULTICAST_DELEGATE_FourParams(FOnCapturedMouseMove, FViewport*, FIntPoint, FIntPoint, int32)
		DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnMouseWheel, FViewport*, float, int32)
		DECLARE_MULTICAST_DELEGATE_FourParams(FOnLMouseButtonDown, FViewport*, FIntPoint, FIntPoint, int32)
		DECLARE_MULTICAST_DELEGATE_FourParams(FOnLMouseButtonUp, FViewport*, FIntPoint, FIntPoint, int32)
		DECLARE_MULTICAST_DELEGATE_FourParams(FOnRMouseButtonDown, FViewport*, FIntPoint, FIntPoint, int32)
		DECLARE_MULTICAST_DELEGATE_FourParams(FOnRMouseButtonUp, FViewport*, FIntPoint, FIntPoint, int32)
		DECLARE_MULTICAST_DELEGATE_FourParams(FOnMiddleMouseButtonDown, FViewport*, FIntPoint, FIntPoint, int32)
		DECLARE_MULTICAST_DELEGATE_FourParams(FOnMiddleMouseButtonUp, FViewport*, FIntPoint, FIntPoint, int32)
		DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnKeyDown, FViewport*, FKey, int32)
		DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnKeyUp, FViewport*, FKey, int32)

		FOnMouseMove M_OnMouseMove;
		FOnCapturedMouseMove M_OnCapturedMouseMove;
		FOnMouseWheel M_OnMouseWheel;
		FOnLMouseButtonDown M_OnLMouseButtonDown;
		FOnLMouseButtonUp M_OnLMouseButtonUp;
		FOnRMouseButtonDown M_OnRMouseButtonDown;
		FOnRMouseButtonUp M_OnRMouseButtonUp;
		FOnMiddleMouseButtonDown M_OnMiddleMouseButtonDown;
		FOnMiddleMouseButtonUp M_OnMiddleMouseButtonUp;
		FOnKeyDown M_OnKeyDown;
		FOnKeyUp M_OnKeyUp;
	};
}
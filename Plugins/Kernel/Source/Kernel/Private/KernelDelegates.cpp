#include "KernelDelegates.h"
#include "KernelDefines.h"

class FViewport;

namespace Kernel
{
	FKernelDelegates::FOnViewportResized FKernelDelegates::S_OnViewportResized;

	int32 MakeModifierKeyFlag(FViewport* InViewport)
	{
		int32 ModifierKeyFlag = 0;
		if (IsCtrlDown(InViewport))
			BIT_ADD(ModifierKeyFlag, EModifierKey_Ctrl);
		if (IsShiftDown(InViewport))
			BIT_ADD(ModifierKeyFlag, EModifierKey_Shift);
		if (IsAltDown(InViewport))
			BIT_ADD(ModifierKeyFlag, EModifierKey_Alt);
		return ModifierKeyFlag;
	}
}
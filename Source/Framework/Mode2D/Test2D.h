#pragma once
#include "ElementDrawer.h"
#include "FixedVector.h"

class FViewport;
class APlayerController;
class UWorld;
class FTest2D
{
public:
	FTest2D(UWorld *InWorld);

private:
	void OnMouseMove(FViewport *InViewport, FIntPoint MouseRelToViewport, FIntPoint MouseRelToWindow, int32 ModifierKeyFlag);
	void OnMouseWheel(FViewport *InViewport, float Delta, int32 ModifierKeyFlag);
	void OnLMouseButtonUp(FViewport *InViewport, FIntPoint MouseRelToViewport, FIntPoint MouseRelToWindow, int32 ModifierKeyFlag);
	void OnRMouseButtonUp(FViewport *InViewport, FIntPoint MouseRelToViewport, FIntPoint MouseRelToWindow, int32 ModifierKeyFlag);
	void OnKeyUp(FViewport *InViewport, FKey InKey, int32 ModifierKeyFlag);

private:
	void CreateOrUpdateOutline();
	void CreateOrUpdateRubberLine(const Math::FFixedVector2D &InStart, const Math::FFixedVector2D &InEnd);
	void CreateOrUpdateAssistantLine(const Math::FFixedVector2D &InStart, const Math::FFixedVector2D &InEnd);

	FORCEINLINE void SetViewportMinMaxNeedUpdate(bool bNeedUpdate) { M_ViewportMinMaxNeedUpdate = bNeedUpdate ? 1 : 0; }
	FORCEINLINE bool IsViewportMinMaxNeedUpdate() const { return M_ViewportMinMaxNeedUpdate == 1; }
	void CalculateViewportMinMax(APlayerController *InPC, FViewport *InViewport);

	void ClearScene();

private:
	uint8 M_ViewportMinMaxNeedUpdate : 1;
	uint8 M_TestCase = 1;

	Math::FFixedVector2D M_Min, M_Max;

	Math::FloatingPointList2D M_CachedPoints;
	Math::FixedPointList2D M_CachedFixedPoints;

	FuncUtilities::FElementDrawer::LineHandle M_OutlineHandle = FuncUtilities::FElementDrawer::S_InvalidHandle;
	FuncUtilities::FElementDrawer::LineHandle M_RubberLineHandle = FuncUtilities::FElementDrawer::S_InvalidHandle;
	FuncUtilities::FElementDrawer::LineHandle M_AssistantLineHandle = FuncUtilities::FElementDrawer::S_InvalidHandle;
};
#pragma once
#include "CoreMinimal.h"

namespace Math
{
	using FloatingPointList2D = TArray<FVector2D>;
	using FloatingPointList = TArray<FVector>;

	inline FloatingPointList2D FloatingPointList3DTo2D(const FloatingPointList& InPointList3D) {
		FloatingPointList2D PointList2D; PointList2D.Reserve(InPointList3D.Num());
		for (const auto& Point : InPointList3D)
			PointList2D.Emplace(FVector2D(Point));
		return PointList2D;
	}

	inline FloatingPointList FloatingPointList2DTo3D(const FloatingPointList2D& InPointList2D, float InZ = 0.0f) {
		FloatingPointList PointList; PointList.Reserve(InPointList2D.Num());
		for (const auto& Point : InPointList2D)
			PointList.Emplace(FVector(Point, InZ));
		return PointList;
	}
}

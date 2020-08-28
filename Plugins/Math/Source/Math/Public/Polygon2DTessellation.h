#pragma once
#include "MathTypes.h"
#include "FixedVector.h"

namespace Math
{
	MATH_API TArray<int32> TriangulateFromFixedPointList(const FixedPointList2D& InOutline, const TArray<FixedPointList2D>& InHoles);
	MATH_API TArray<int32> TriangulateFromFixedPointList(const FixedPointList& InOutline, const TArray<FixedPointList>& InHoles);
	MATH_API TArray<int32> TriangulateFromFloatingPointList(const FloatingPointList2D& InOutline, const TArray<FloatingPointList2D>& InHoles);
	MATH_API TArray<int32> TriangulateFromFloatingPointList(const FloatingPointList& InOutline, const TArray<FloatingPointList>& InHoles);
}

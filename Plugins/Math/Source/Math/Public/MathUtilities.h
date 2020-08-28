#pragma once
#include "FixedVector.h"
#include "MathTypes.h"

namespace Math
{
	namespace Utilities 
	{
		MATH_API void ReverseFixedPointList2DInplace(FixedPointList2D& InOutPointList);
		MATH_API FixedPointList2D ReverseFixedPointList2D(const FixedPointList2D& InPointList);

		MATH_API int32 GetIndex(int32 Current, int32 NumElements, int32 Offset);
	}
}

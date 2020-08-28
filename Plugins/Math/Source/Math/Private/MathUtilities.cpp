#include "MathUtilities.h"

namespace Math
{
	namespace Utilities 
	{
		void ReverseFixedPointList2DInplace(FixedPointList2D& InOutPointList)
		{
			typename FixedPointList2D::SizeType NumPoints = InOutPointList.Num();
			typename FixedPointList2D::SizeType IterCount = NumPoints / 2;
			FFixedVector2D Temp;
			for (typename FixedPointList2D::SizeType i = 0; i < IterCount; ++i)
			{
				Temp = InOutPointList[i];
				InOutPointList[i] = InOutPointList[NumPoints - 1 - i];
				InOutPointList[NumPoints - 1 - i] = Temp;
			}
		}

		FixedPointList2D ReverseFixedPointList2D(const FixedPointList2D& InPointList)
		{
			FixedPointList2D Result(InPointList);
			ReverseFixedPointList2DInplace(Result);
			return Result;
		}

		int32 GetIndex(int32 Current, int32 NumElements, int32 Offset)
		{
			return (Current + Offset + NumElements) % NumElements;
		}
	}
}
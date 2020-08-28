#pragma once
#include "MathTypes.h"
#include "FixedVector.h"

namespace Math
{
	MATH_API TArray<FixedPointList2D> PolygonUnionFixedPointList(const TArray<FixedPointList2D>& InPolygons);
	MATH_API TArray<FixedPointList2D> PolygonUnionFixedPointList(const TArray<FixedPointList>& InPolygons);
	MATH_API TArray<FixedPointList2D> PolygonUnionFloatingPointList(const TArray<FloatingPointList2D>& InPolygons);
	MATH_API TArray<FixedPointList2D> PolygonUnionFloatingPointList(const TArray<FloatingPointList>& InPolygons);

	MATH_API TArray<FixedPointList2D> PolygonIntersectionFixedPointList(const TArray<FixedPointList2D>& InSubjectPolygons, const TArray<FixedPointList2D>& InClippingPolygons);
	MATH_API TArray<FixedPointList2D> PolygonIntersectionFixedPointList(const TArray<FixedPointList>& InSubjectPolygons, const TArray<FixedPointList>& InClippingPolygons);
	MATH_API TArray<FixedPointList2D> PolygonIntersectionFloatingPointList(const TArray<FloatingPointList2D>& InSubjectPolygons, const TArray<FloatingPointList2D>& InClippingPolygons);
	MATH_API TArray<FixedPointList2D> PolygonIntersectionFloatingPointList(const TArray<FloatingPointList>& InSubjectPolygons, const TArray<FloatingPointList>& InClippingPolygons);

	MATH_API TArray<FixedPointList2D> PolygonDifferenceFixedPointList(const TArray<FixedPointList2D>& InSubjectPolygons, const TArray<FixedPointList2D>& InClippingPolygons);
	MATH_API TArray<FixedPointList2D> PolygonDifferenceFixedPointList(const TArray<FixedPointList>& InSubjectPolygons, const TArray<FixedPointList>& InClippingPolygons);
	MATH_API TArray<FixedPointList2D> PolygonDifferenceFloatingPointList(const TArray<FloatingPointList2D>& InSubjectPolygons, const TArray<FloatingPointList2D>& InClippingPolygons);
	MATH_API TArray<FixedPointList2D> PolygonDifferenceFloatingPointList(const TArray<FloatingPointList>& InSubjectPolygons, const TArray<FloatingPointList>& InClippingPolygons);
}

#pragma once
#include "MathTypes.h"

namespace Math
{
	using Fixed = int32;
	using FixedTemp = int64;
	using FFixedVector = FIntVector;
	using FFixedVector2D = FIntPoint;
	using FFixedBox2D = FIntRect;
	using FixedPointList2D = TArray<FFixedVector2D>;
	using FixedPointList = TArray<FFixedVector>;

	constexpr int32 C_NumDecimalDigits = 16;
	constexpr float C_Pow2 = 65536.0f;
	constexpr int64 C_FixedMax = (int64)0x7FFFFFFF;
	constexpr int64 C_FixedMin = -(((int64)1) << 31);

	extern MATH_API const Fixed C_FixedConst1;
	extern MATH_API const Fixed C_FixedConstHalf;
	extern MATH_API const Fixed C_FixedConstError;
	extern MATH_API const Fixed C_FixedConst1MinusError;

	extern MATH_API const FFixedVector2D C_FixedVector2DZero;
	extern MATH_API const FFixedVector2D C_FixedVector2DUnitX;
	extern MATH_API const FFixedVector2D C_FixedVector2DUnitY;
	extern MATH_API const FFixedVector C_FixedVectorZero;
	extern MATH_API const FFixedVector C_FixedVectorUnitX;
	extern MATH_API const FFixedVector C_FixedVectorUnitY;
	extern MATH_API const FFixedVector C_FixedVectorUnitZ;

	FORCEINLINE Fixed FixedFromInt(int32 i) {
		return ((Fixed)i) << C_NumDecimalDigits;
	}

	FORCEINLINE Fixed FixedFromFloat(float f) {
		return (Fixed)(f * C_Pow2);
	}

	FORCEINLINE Fixed FixedFromDouble(double d) {
		return (Fixed)(d * C_Pow2);
	}

	FORCEINLINE int32 FixedToInt(Fixed f) {
		return f >> C_NumDecimalDigits;
	}

	FORCEINLINE float FixedToFloat(Fixed f) {
		return (float)(f / C_Pow2);
	}

	FORCEINLINE double FixedToDouble(Fixed f) {
		return (double)(f / C_Pow2);
	}

	FORCEINLINE Fixed FixedFrac(Fixed f) {
		return f & C_FixedConst1MinusError;
	}

	FORCEINLINE Fixed FixedFloor(Fixed f) {
		return f & (~C_FixedConst1MinusError);
	}

	FORCEINLINE Fixed FixedCeil(Fixed f) {
		return FixedFloor(f + 0xFFFF);
	}

	FORCEINLINE Fixed FixedMul(Fixed x, Fixed y) {
		return (Fixed)((((FixedTemp)x) * y) >> 16);
	}

	FORCEINLINE Fixed FixedDiv(Fixed x, Fixed y) {
		return (Fixed)((((FixedTemp)x) << 16) / y);
	}

	FORCEINLINE FFixedVector FromFloatingVector(const FVector& InVector) {
		return FFixedVector(FixedFromFloat(InVector.X), FixedFromFloat(InVector.Y), FixedFromFloat(InVector.Z));
	}

	FORCEINLINE FFixedVector2D FromFloatingVector2D(const FVector2D& InVector2D) {
		return FFixedVector2D(FixedFromFloat(InVector2D.X), FixedFromFloat(InVector2D.Y));
	}

	FORCEINLINE FVector ToFloatingVector(const FFixedVector& InVector) {
		return FVector(FixedToFloat(InVector.X), FixedToFloat(InVector.Y), FixedToFloat(InVector.Z));
	}

	FORCEINLINE FVector2D ToFloatingVector2D(const FFixedVector2D& InVector2D) {
		return FVector2D(FixedToFloat(InVector2D.X), FixedToFloat(InVector2D.Y));
	}

	FORCEINLINE FFixedVector2D Fixed3DTo2D(const FFixedVector& InVector) {
		return FFixedVector2D(InVector.X, InVector.Y);
	}

	FORCEINLINE FFixedVector Fixed2DTo3D(const FFixedVector2D& InVector2D) {
		return FFixedVector(InVector2D.X, InVector2D.Y, 0);
	}

	inline FixedPointList2D FixedPointList3DTo2D(const FixedPointList& InPointList3D) {
		FixedPointList2D PointList2D; PointList2D.Reserve(InPointList3D.Num());
		for (const auto& Point : InPointList3D)
			PointList2D.Emplace(Fixed3DTo2D(Point));
		return PointList2D;
	}

	inline FixedPointList FixedPointList2DTo3D(const FixedPointList2D& InPointList2D) {
		FixedPointList PointList3D; PointList3D.Reserve(InPointList2D.Num());
		for (const auto& Point : InPointList2D)
			PointList3D.Emplace(Fixed2DTo3D(Point));
		return PointList3D;
	}

	inline FixedPointList2D FromFloatingPointList2D(const FloatingPointList2D& InFloatingPointList) {
		FixedPointList2D FixedPointList; FixedPointList.Reserve(InFloatingPointList.Num());
		for (const auto& Point : InFloatingPointList)
			FixedPointList.Emplace(FixedFromFloat(Point.X), FixedFromFloat(Point.Y));
		return FixedPointList;
	}

	inline FixedPointList FromFloatingPointList(const FloatingPointList& InFloatingPointList) {
		FixedPointList FixedPointList; FixedPointList.Reserve(InFloatingPointList.Num());
		for (const auto& Point : InFloatingPointList)
			FixedPointList.Emplace(FixedFromFloat(Point.X), FixedFromFloat(Point.Y), FixedFromFloat(Point.Z));
		return FixedPointList;
	}

	inline FloatingPointList2D ToFloatingPointList2D(const FixedPointList2D& InFixedPointList) {
		FloatingPointList2D FloatingPointList; FloatingPointList.Reserve(InFixedPointList.Num());
		for (const auto Point : InFixedPointList)
			FloatingPointList.Emplace(FixedToFloat(Point.X), FixedToFloat(Point.Y));
		return FloatingPointList;
	}

	inline FloatingPointList ToFloatingPointList(const FixedPointList& InFixedPointList) {
		FloatingPointList FloatingPointList; FloatingPointList.Reserve(InFixedPointList.Num());
		for (const auto& Point : InFixedPointList)
			FloatingPointList.Emplace(FixedToFloat(Point.X), FixedToFloat(Point.Y), FixedToFloat(Point.Z));
		return FloatingPointList;
	}

	FORCEINLINE void FixedVector2DMulInplace(FFixedVector2D& InOutVector2D, Fixed InFixedScale) {
		InOutVector2D.X = FixedMul(InOutVector2D.X, InFixedScale);
		InOutVector2D.Y = FixedMul(InOutVector2D.Y, InFixedScale);
	}

	FORCEINLINE FFixedVector2D FixedVector2DMul(const FFixedVector2D& InVector2D, Fixed InFixedScale) {
		FFixedVector2D MulVector(InVector2D);
		FixedVector2DMulInplace(MulVector, InFixedScale);
		return MulVector;
	}

	FORCEINLINE void FixedVectorMulInplace(FFixedVector& InOutVector, Fixed InFixedScale) {
		InOutVector.X = FixedMul(InOutVector.X, InFixedScale);
		InOutVector.Y = FixedMul(InOutVector.Y, InFixedScale);
		InOutVector.Z = FixedMul(InOutVector.Z, InFixedScale);
	}

	FORCEINLINE FFixedVector FixedVectorMul(const FFixedVector& InVector, Fixed InFixedScale) {
		FFixedVector MulVector(InVector);
		FixedVectorMulInplace(MulVector, InFixedScale);
		return MulVector;
	}

	FORCEINLINE void FixedVector2DDivInplace(FFixedVector2D& InOutVector2D, Fixed InFixedScale) {
		check(InFixedScale != 0);
		InOutVector2D.X = FixedDiv(InOutVector2D.X, InFixedScale);
		InOutVector2D.Y = FixedDiv(InOutVector2D.Y, InFixedScale);
	}

	FORCEINLINE FFixedVector2D FixedVector2DDiv(const FFixedVector2D& InVector2D, Fixed InFixedScale) {
		FFixedVector2D DivVector(InVector2D);
		FixedVector2DDivInplace(DivVector, InFixedScale);
		return DivVector;
	}

	FORCEINLINE void FixedVectorDivInplace(FFixedVector& InOutVector, Fixed InFixedScale) {
		check(InFixedScale != 0);
		InOutVector.X = FixedDiv(InOutVector.X, InFixedScale);
		InOutVector.Y = FixedDiv(InOutVector.Y, InFixedScale);
		InOutVector.Z = FixedDiv(InOutVector.Z, InFixedScale);
	}

	FORCEINLINE FFixedVector FixedVectorDiv(const FFixedVector& InVector, Fixed InFixedScale) {
		FFixedVector DivVector(InVector);
		FixedVectorDivInplace(DivVector, InFixedScale);
		return DivVector;
	}

	FORCEINLINE FFixedVector2D MinusFixedVector2D(const FFixedVector2D& InVector2D) {
		return FFixedVector2D(-InVector2D.X, -InVector2D.Y);
	}

	FORCEINLINE FFixedVector MinusFixedVector(const FFixedVector& InVector) {
		return FFixedVector(-InVector.X, -InVector.Y, -InVector.Z);
	}

	FORCEINLINE Fixed FixedVector2DLength(const FFixedVector2D& InVector2D) {
		return InVector2D.Size();
	}

	FORCEINLINE Fixed FixedVectorLength(const FFixedVector& InVector) {
		return InVector.Size();
	}

	FORCEINLINE Fixed FixedVector2DDotProduct(const FFixedVector2D& InVector1, const FFixedVector2D& InVector2) {
		return FixedMul(InVector1.X, InVector2.X) + FixedMul(InVector1.Y, InVector2.Y);
	}

	FORCEINLINE Fixed FixedVectorDotProduct(const FFixedVector& InVector1, const FFixedVector& InVector2) {
		return FixedMul(InVector1.X, InVector2.X) + FixedMul(InVector1.Y, InVector2.Y) + FixedMul(InVector1.Z, InVector2.Z);
	}

	FORCEINLINE Fixed FixedVector2DCrossProduct(const FFixedVector2D& InVector1, const FFixedVector2D& InVector2)
	{
		return FixedMul(InVector1.X, InVector2.Y) - FixedMul(InVector1.Y, InVector2.X);
	}

	FORCEINLINE FFixedVector FixedVectorCrossProduct(const FFixedVector& InVector1, const FFixedVector& InVector2) {
		return FFixedVector(
			FixedMul(InVector1.Y, InVector2.Z) - FixedMul(InVector1.Z, InVector2.Y),
			FixedMul(InVector1.Z, InVector2.X) - FixedMul(InVector1.X, InVector2.Z),
			FixedMul(InVector1.X, InVector2.Y) - FixedMul(InVector1.Y, InVector2.X)
		);
	}

	inline void FixedVector2DNormalizeInplace(FFixedVector2D& InOutVector) {
		Fixed Size = FixedVector2DLength(InOutVector);
		if (Size > 0)
		{
			FixedVector2DDivInplace(InOutVector, Size);
		}
		else
		{
			InOutVector.X = 0;
			InOutVector.Y = 0;
		}
	}

	inline FFixedVector2D FixedVector2DNormalize(const FFixedVector2D& InVector) {
		FFixedVector2D NormalizedVector(InVector);
		FixedVector2DNormalizeInplace(NormalizedVector);
		return NormalizedVector;
	}

	inline void FixedVectorNormalizeInplace(FFixedVector& InOutVector) {
		Fixed Size = FixedVectorLength(InOutVector);
		if (Size > 0)
		{
			FixedVectorDivInplace(InOutVector, Size);
		}
		else
		{
			InOutVector.X = 0;
			InOutVector.Y = 0;
			InOutVector.Z = 0;
		}
	}

	inline FFixedVector FixedVectorNormalize(const FFixedVector& InVector) {
		FFixedVector NormalizedVector(InVector);
		FixedVectorNormalizeInplace(NormalizedVector);
		return NormalizedVector;
	}

	FORCEINLINE void FixedVector2DPerpendicularInplace(FFixedVector2D& InOutVector) {
		InOutVector = FFixedVector2D(-InOutVector.Y, InOutVector.X);
	}

	FORCEINLINE FFixedVector2D FixedVector2DPerpendicular(const FFixedVector2D& InVector) {
		FFixedVector2D Perpendicular(InVector);
		FixedVector2DPerpendicularInplace(Perpendicular);
		return Perpendicular;
	}
}
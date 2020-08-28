#include "Polygon2DTessellation.h"
#include "Earcut/earcut.hpp"

namespace Math
{
	using EarcutPoint = std::pair<double, double>;

	std::vector<EarcutPoint> ConvertToSTDVector(const FixedPointList2D& InPointList)
	{
		std::vector<EarcutPoint> Result;
		Result.reserve((typename std::vector<EarcutPoint>::size_type)InPointList.Num());
		for (const auto& Point : InPointList)
			Result.emplace_back(std::make_pair(FixedToDouble(Point.X), FixedToDouble(Point.Y)));
		return Result;
	}

	TArray<int32> TriangulateFromFixedPointList(const FixedPointList2D& InOutline, const TArray<FixedPointList2D>& InHoles)
	{
		std::vector<std::vector<EarcutPoint>> Polygons; Polygons.reserve(1 + InHoles.Num());
		Polygons.emplace_back(ConvertToSTDVector(InOutline));
		for (const auto& Hole : InHoles)
			Polygons.emplace_back(ConvertToSTDVector(Hole));
		auto Indices = mapbox::earcut(Polygons);
		// 耳切法返回的索引是顺时针的，因为UE4默认Cull的Backface是顺时针，所以需要反转索引
		TArray<int32> IndicesCCW(reinterpret_cast<int32*>(Indices.data()), (typename TArray<uint32>::SizeType)Indices.size());
		typename TArray<int32>::SizeType IterCount = IndicesCCW.Num() / 3;
		for (typename TArray<int32>::SizeType i = 0; i < IterCount; ++i)
		{
			int32 Temp = IndicesCCW[i * 3];
			IndicesCCW[i * 3] = IndicesCCW[i * 3 + 2];
			IndicesCCW[i * 3 + 2] = Temp;
		}
		return IndicesCCW;
	}

	TArray<int32> TriangulateFromFixedPointList(const FixedPointList& InOutline, const TArray<FixedPointList>& InHoles)
	{
		TArray<FixedPointList2D> FixedPointHoles2D; FixedPointHoles2D.Reserve(InHoles.Num());
		for (const auto& Hole : InHoles)
			FixedPointHoles2D.Emplace(FixedPointList3DTo2D(Hole));
		return TriangulateFromFixedPointList(FixedPointList3DTo2D(InOutline), FixedPointHoles2D);
	}

	TArray<int32> TriangulateFromFloatingPointList(const FloatingPointList2D& InOutline, const TArray<FloatingPointList2D>& InHoles)
	{
		TArray<FixedPointList2D> FixedPointHoles; FixedPointHoles.Reserve(InHoles.Num());
		for (const auto& Hole : InHoles)
			FixedPointHoles.Emplace(FromFloatingPointList2D(Hole));
		return TriangulateFromFixedPointList(FromFloatingPointList2D(InOutline), FixedPointHoles);
	}

	TArray<int32> TriangulateFromFloatingPointList(const FloatingPointList& InOutline, const TArray<FloatingPointList>& InHoles)
	{
		TArray<FloatingPointList2D> Holes2D; Holes2D.Reserve(InHoles.Num());
		for (const auto& Hole : InHoles)
			Holes2D.Emplace(FloatingPointList3DTo2D(Hole));
		return TriangulateFromFloatingPointList(FloatingPointList3DTo2D(InOutline), Holes2D);
	}
}


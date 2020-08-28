#include "Polygon2DClippingOperation.h"
#include "Templates/UnrealTemplate.h"
#include "Clipper/clipper.hpp"

namespace Math
{
	ClipperLib::Path ConvertToClipperPath(const FixedPointList2D& InPolygon)
	{
		ClipperLib::Path Result; Result.reserve((typename ClipperLib::Path::size_type)InPolygon.Num());
		for (const auto& Point : InPolygon)
			Result.emplace_back((ClipperLib::cInt)Point.X, (ClipperLib::cInt)Point.Y);
		return Result;
	}

	TArray<FixedPointList2D> ClipperSolutionToFixedPointLists(const ClipperLib::Paths& InSolution)
	{
		TArray<FixedPointList2D> Result;
		if (InSolution.size() > 0)
		{
			for (const auto& SolutionPath : InSolution)
			{
				FixedPointList2D SolutionPathFixedPoint;
				SolutionPathFixedPoint.Reserve((typename FixedPointList2D::SizeType)SolutionPath.size());
				for (const auto& Point : SolutionPath)
					SolutionPathFixedPoint.Emplace((Fixed)Point.X, (Fixed)Point.Y);
				Result.Emplace(MoveTemp(SolutionPathFixedPoint));
			}
		}
		return Result;
	}

	TArray<FixedPointList2D> PolygonUnionFixedPointList(const TArray<FixedPointList2D>& InPolygons)
	{
		if (InPolygons.Num() == 0)
			return TArray<FixedPointList2D>();
		ClipperLib::Clipper c;
		int32 n = 0;
		for (const auto& Polygon : InPolygons)
		{
			c.AddPath(ConvertToClipperPath(Polygon), (n++) == 0 ? ClipperLib::ptSubject : ClipperLib::ptClip, true);
		}
		ClipperLib::Paths Solution;
		c.Execute(ClipperLib::ctUnion, Solution, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);
		return ClipperSolutionToFixedPointLists(Solution);
	}

	TArray<FixedPointList2D> PolygonUnionFixedPointList(const TArray<FixedPointList>& InPolygons)
	{
		TArray<FixedPointList2D> FixedPointPolygons2D; FixedPointPolygons2D.Reserve(InPolygons.Num());
		for (const auto& Polygon : InPolygons)
			FixedPointPolygons2D.Emplace(FixedPointList3DTo2D(Polygon));
		return PolygonUnionFixedPointList(FixedPointPolygons2D);
	}

	TArray<FixedPointList2D> PolygonUnionFloatingPointList(const TArray<FloatingPointList2D>& InPolygons)
	{
		TArray<FixedPointList2D> FixedPointPolygons; FixedPointPolygons.Reserve(InPolygons.Num());
		for (const auto& Polygon : InPolygons)
			FixedPointPolygons.Emplace(FromFloatingPointList2D(Polygon));
		return PolygonUnionFixedPointList(FixedPointPolygons);
	}

	TArray<FixedPointList2D> PolygonUnionFloatingPointList(const TArray<FloatingPointList>& InPolygons)
	{
		TArray<FloatingPointList2D> Polygons2D; Polygons2D.Reserve(InPolygons.Num());
		for (const auto& Polygon : InPolygons)
			Polygons2D.Emplace(FloatingPointList3DTo2D(Polygon));
		return PolygonUnionFloatingPointList(Polygons2D);
	}

	TArray<FixedPointList2D> PolygonIntersectionFixedPointList(const TArray<FixedPointList2D>& InSubjectPolygons, const TArray<FixedPointList2D>& InClippingPolygons)
	{
		if (InSubjectPolygons.Num() == 0)
			return TArray<FixedPointList2D>();
		if (InClippingPolygons.Num() == 0)
			return InSubjectPolygons;
		ClipperLib::Clipper c;
		ClipperLib::Paths SubjectPaths;
		for (const auto SubjectPolygon : InSubjectPolygons)
			SubjectPaths.emplace_back(ConvertToClipperPath(SubjectPolygon));
		c.AddPaths(SubjectPaths, ClipperLib::ptSubject, true);
		ClipperLib::Paths ClippingPaths;
		for (const auto ClippingPolygon : InClippingPolygons)
			ClippingPaths.emplace_back(ConvertToClipperPath(ClippingPolygon));
		c.AddPaths(ClippingPaths, ClipperLib::ptClip, true);
		ClipperLib::Paths Solution;
		c.Execute(ClipperLib::ctIntersection, Solution, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);
		return ClipperSolutionToFixedPointLists(Solution);
	}

	TArray<FixedPointList2D> PolygonIntersectionFixedPointList(const TArray<FixedPointList>& InSubjectPolygons, const TArray<FixedPointList>& InClippingPolygons)
	{
		TArray<FixedPointList2D> FixedPointSubjectPolygons2D; FixedPointSubjectPolygons2D.Reserve(InSubjectPolygons.Num());
		for (const auto& SubjectPolygon : InSubjectPolygons)
			FixedPointSubjectPolygons2D.Emplace(FixedPointList3DTo2D(SubjectPolygon));
		TArray<FixedPointList2D> FixedPointClippingPolygons2D; FixedPointClippingPolygons2D.Reserve(InClippingPolygons.Num());
		for (const auto& ClippingPolygon : InClippingPolygons)
			FixedPointClippingPolygons2D.Emplace(FixedPointList3DTo2D(ClippingPolygon));
		return PolygonIntersectionFixedPointList(FixedPointSubjectPolygons2D, FixedPointClippingPolygons2D);
	}

	TArray<FixedPointList2D> PolygonIntersectionFloatingPointList(const TArray<FloatingPointList2D>& InSubjectPolygons, const TArray<FloatingPointList2D>& InClippingPolygons)
	{
		TArray<FixedPointList2D> FixedPointSubjectPolygons; FixedPointSubjectPolygons.Reserve(InSubjectPolygons.Num());
		for (const auto& SubjectPolygon : InSubjectPolygons)
			FixedPointSubjectPolygons.Emplace(FromFloatingPointList2D(SubjectPolygon));
		TArray<FixedPointList2D> FixedPointClippingPolygons; FixedPointClippingPolygons.Reserve(InClippingPolygons.Num());
		for (const auto& ClippingPolygon : InClippingPolygons)
			FixedPointClippingPolygons.Emplace(FromFloatingPointList2D(ClippingPolygon));
		return PolygonIntersectionFixedPointList(FixedPointSubjectPolygons, FixedPointClippingPolygons);
	}

	TArray<FixedPointList2D> PolygonIntersectionFloatingPointList(const TArray<FloatingPointList>& InSubjectPolygons, const TArray<FloatingPointList>& InClippingPolygons)
	{
		TArray<FloatingPointList2D> SubjectPolygons2D; SubjectPolygons2D.Reserve(InSubjectPolygons.Num());
		for (const auto& SubjectPolygon : InSubjectPolygons)
			SubjectPolygons2D.Emplace(FloatingPointList3DTo2D(SubjectPolygon));
		TArray<FloatingPointList2D> ClippingPolygons2D; ClippingPolygons2D.Reserve(InClippingPolygons.Num());
		for (const auto& ClippingPolygon : InClippingPolygons)
			ClippingPolygons2D.Emplace(FloatingPointList3DTo2D(ClippingPolygon));
		return PolygonIntersectionFloatingPointList(SubjectPolygons2D, ClippingPolygons2D);
	}

	TArray<FixedPointList2D> PolygonDifferenceFixedPointList(const TArray<FixedPointList2D>& InSubjectPolygons, const TArray<FixedPointList2D>& InClippingPolygons)
	{
		if (InSubjectPolygons.Num() == 0)
			return TArray<FixedPointList2D>();
		if (InClippingPolygons.Num() == 0)
			return InSubjectPolygons;
		ClipperLib::Clipper c;
		ClipperLib::Paths SubjectPaths;
		for (const auto SubjectPolygon : InSubjectPolygons)
			SubjectPaths.emplace_back(ConvertToClipperPath(SubjectPolygon));
		c.AddPaths(SubjectPaths, ClipperLib::ptSubject, true);
		ClipperLib::Paths ClippingPaths;
		for (const auto ClippingPolygon : InClippingPolygons)
			ClippingPaths.emplace_back(ConvertToClipperPath(ClippingPolygon));
		c.AddPaths(ClippingPaths, ClipperLib::ptClip, true);
		ClipperLib::Paths Solution;
		c.Execute(ClipperLib::ctDifference, Solution, ClipperLib::pftEvenOdd, ClipperLib::pftEvenOdd);
		return ClipperSolutionToFixedPointLists(Solution);
	}

	TArray<FixedPointList2D> PolygonDifferenceFixedPointList(const TArray<FixedPointList>& InSubjectPolygons, const TArray<FixedPointList>& InClippingPolygons)
	{
		TArray<FixedPointList2D> FixedPointSubjectPolygons2D; FixedPointSubjectPolygons2D.Reserve(InSubjectPolygons.Num());
		for (const auto& SubjectPolygon : InSubjectPolygons)
			FixedPointSubjectPolygons2D.Emplace(FixedPointList3DTo2D(SubjectPolygon));
		TArray<FixedPointList2D> FixedPointClippingPolygons2D; FixedPointClippingPolygons2D.Reserve(InClippingPolygons.Num());
		for (const auto& ClippingPolygon : InClippingPolygons)
			FixedPointClippingPolygons2D.Emplace(FixedPointList3DTo2D(ClippingPolygon));
		return PolygonDifferenceFixedPointList(FixedPointSubjectPolygons2D, FixedPointClippingPolygons2D);
	}

	TArray<FixedPointList2D> PolygonDifferenceFloatingPointList(const TArray<FloatingPointList2D>& InSubjectPolygons, const TArray<FloatingPointList2D>& InClippingPolygons)
	{
		TArray<FixedPointList2D> FixedPointSubjectPolygons; FixedPointSubjectPolygons.Reserve(InSubjectPolygons.Num());
		for (const auto& SubjectPolygon : InSubjectPolygons)
			FixedPointSubjectPolygons.Emplace(FromFloatingPointList2D(SubjectPolygon));
		TArray<FixedPointList2D> FixedPointClippingPolygons; FixedPointClippingPolygons.Reserve(InClippingPolygons.Num());
		for (const auto& ClippingPolygon : InClippingPolygons)
			FixedPointClippingPolygons.Emplace(FromFloatingPointList2D(ClippingPolygon));
		return PolygonDifferenceFixedPointList(FixedPointSubjectPolygons, FixedPointClippingPolygons);
	}

	TArray<FixedPointList2D> PolygonDifferenceFloatingPointList(const TArray<FloatingPointList>& InSubjectPolygons, const TArray<FloatingPointList>& InClippingPolygons)
	{
		TArray<FloatingPointList2D> SubjectPolygons2D; SubjectPolygons2D.Reserve(InSubjectPolygons.Num());
		for (const auto& SubjectPolygon : InSubjectPolygons)
			SubjectPolygons2D.Emplace(FloatingPointList3DTo2D(SubjectPolygon));
		TArray<FloatingPointList2D> ClippingPolygons2D; ClippingPolygons2D.Reserve(InClippingPolygons.Num());
		for (const auto& ClippingPolygon : InClippingPolygons)
			ClippingPolygons2D.Emplace(FloatingPointList3DTo2D(ClippingPolygon));
		return PolygonDifferenceFloatingPointList(SubjectPolygons2D, ClippingPolygons2D);
	}
}


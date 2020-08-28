#include "SCustomTitleBar.h"
#include "Types/PaintArgs.h"
#include "Layout/ArrangedChildren.h"
#include "Layout/LayoutUtils.h"

SCustomTitleBar::SCustomTitleBar()
	: ChildSlot(this)
{
	SetCanTick(false);
}

void SCustomTitleBar::Construct(const FArguments& InArgs)
{
	ChildSlot
		.HAlign(InArgs._HAlign)
		.VAlign(InArgs._VAlign)
		.Padding(InArgs._Padding)
		[
			InArgs._Content.Widget
		];
}

void SCustomTitleBar::SetContent(const TSharedRef<SWidget>& InContent)
{
	ChildSlot
		[
			InContent
		];

	Invalidate(EInvalidateWidget::Layout);
}

void SCustomTitleBar::OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const
{
	const EVisibility ChildVisibility = ChildSlot.GetWidget()->GetVisibility();
	if (ArrangedChildren.Accepts(ChildVisibility))
	{
		const FMargin SlotPadding(ChildSlot.SlotPadding.Get());
		bool bAlignChildren = true;

		AlignmentArrangeResult XAlignmentResult = AlignChild<Orient_Horizontal>(AllottedGeometry.GetLocalSize().X, ChildSlot, SlotPadding);
		AlignmentArrangeResult YAlignmentResult = AlignChild<Orient_Vertical>(AllottedGeometry.GetLocalSize().Y, ChildSlot, SlotPadding);

		const float AlignedSizeX = XAlignmentResult.Size;
		const float AlignedSizeY = YAlignmentResult.Size;

		ArrangedChildren.AddWidget(
			AllottedGeometry.MakeChild(
				ChildSlot.GetWidget(),
				FVector2D(XAlignmentResult.Offset, YAlignmentResult.Offset),
				FVector2D(AlignedSizeX, AlignedSizeY)
			)
		);
	}
}

FChildren* SCustomTitleBar::GetChildren()
{
	return &ChildSlot;
}

int32 SCustomTitleBar::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	FArrangedChildren ArrangedChildren(EVisibility::Visible);
	this->ArrangeChildren(AllottedGeometry, ArrangedChildren);

	// 检查Children是否有可见的
	if (ArrangedChildren.Num() > 0)
	{
		check(ArrangedChildren.Num() == 1);
		FArrangedWidget& TheChild = ArrangedChildren[0];

		return TheChild.Widget->Paint(Args.WithNewParent(this), TheChild.Geometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, ShouldBeEnabled(bParentEnabled));
	}
	return LayerId;
}

void SCustomTitleBar::SetHAlign(EHorizontalAlignment HAlign)
{
	if (ChildSlot.HAlignment != HAlign)
	{
		ChildSlot.HAlignment = HAlign;
		Invalidate(EInvalidateWidget::Layout);
	}
}

void SCustomTitleBar::SetVAlign(EVerticalAlignment VAlign)
{
	if (ChildSlot.VAlignment != VAlign)
	{
		ChildSlot.VAlignment = VAlign;
		Invalidate(EInvalidateWidget::Layout);
	}
}

void SCustomTitleBar::SetPadding(const TAttribute<FMargin>& InPadding)
{
	if (!ChildSlot.SlotPadding.IdenticalTo(InPadding))
	{
		ChildSlot.SlotPadding = InPadding;
		Invalidate(EInvalidateWidget::LayoutAndVolatility);
	}
}

FVector2D SCustomTitleBar::ComputeDesiredSize(float) const
{
	EVisibility ChildVisibility = ChildSlot.GetWidget()->GetVisibility();

	if (ChildVisibility != EVisibility::Collapsed)
		return FVector2D(ComputeDesiredWidth(), ComputeDesiredHeight());

	return FVector2D::ZeroVector;
}

float SCustomTitleBar::ComputeDesiredWidth() const
{
	const FVector2D& UnmodifiedChildDesiredSize = ChildSlot.GetWidget()->GetDesiredSize() + ChildSlot.SlotPadding.Get().GetDesiredSize();
	return UnmodifiedChildDesiredSize.X;
}

float SCustomTitleBar::ComputeDesiredHeight() const
{
	const FVector2D& UnmodifiedChildDesiredSize = ChildSlot.GetWidget()->GetDesiredSize() + ChildSlot.SlotPadding.Get().GetDesiredSize();
	return UnmodifiedChildDesiredSize.Y;
}
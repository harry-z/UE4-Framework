#pragma once
#include "Widgets/SWidget.h"
#include "Misc/Attribute.h"
#include "Layout/Children.h"
#include "Layout/Margin.h"
#include "Layout/Visibility.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class FArrangedChildren;
class FPaintArgs;
class FSlateWindowElementList;

class SLATEWIDGETS_API SCustomTitleBar : public SWidget
{
public:
	class FTitleBarSlot : public TSupportsOneChildMixin<FTitleBarSlot>, public TSupportsContentAlignmentMixin<FTitleBarSlot>, public TSupportsContentPaddingMixin<FTitleBarSlot>
	{
	public:
		FTitleBarSlot(SWidget* InOwner)
			: TSupportsOneChildMixin<FTitleBarSlot>(InOwner)
			, TSupportsContentAlignmentMixin<FTitleBarSlot>(HAlign_Fill, VAlign_Fill)
		{
		}
	};

	SLATE_BEGIN_ARGS(SCustomTitleBar)
		: _HAlign(HAlign_Fill)
		, _VAlign(VAlign_Fill)
		, _Padding(0.0f)
		{}
	SLATE_ARGUMENT(EHorizontalAlignment, HAlign)
		SLATE_ARGUMENT(EVerticalAlignment, VAlign)
		SLATE_ATTRIBUTE(FMargin, Padding)
		SLATE_DEFAULT_SLOT(FArguments, Content)
		SLATE_END_ARGS()

	SCustomTitleBar();

	void Construct(const FArguments& InArgs);
	virtual EWindowZone::Type GetWindowZoneOverride() const override { return EWindowZone::TitleBar; }
	virtual void OnArrangeChildren(const FGeometry& AllottedGeometry, FArrangedChildren& ArrangedChildren) const override;
	virtual FChildren* GetChildren() override;
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

	void SetContent(const TSharedRef<SWidget>& InContent);
	void SetHAlign(EHorizontalAlignment HAlign);
	void SetVAlign(EVerticalAlignment VAlign);
	void SetPadding(const TAttribute<FMargin>& InPadding);

protected:
	// Begin SWidget overrides.
	virtual FVector2D ComputeDesiredSize(float) const override;
	float ComputeDesiredWidth() const;
	float ComputeDesiredHeight() const;
	// End SWidget overrides.

private:
	FTitleBarSlot ChildSlot;
};
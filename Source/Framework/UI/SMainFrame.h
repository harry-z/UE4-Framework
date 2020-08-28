#pragma once
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SBoxPanel.h"
#include "../Framework.h"

DECLARE_DELEGATE_OneParam(FOnSwitchGameContent, EGameContentType)

class SViewport;
class SMainFrame : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SMainFrame)
		{}
		SLATE_EVENT(FOnSwitchGameContent, OnSwitchGameContent)
	SLATE_END_ARGS()

	void Construct(const FArguments &InArgs);
	void SetViewport(TSharedPtr<SViewport> InViewport);

private:
	SVerticalBox::FSlot *M_ViewportSlot;
	bool M_bMainFrameMaximized = true;
};
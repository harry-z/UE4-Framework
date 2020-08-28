#include "SMainFrame.h"
#include "UIGlobal.h"
#include "SCustomTitleBar.h"
#include "UnrealEngine.h"
#include "Widgets/SViewport.h"
#include "Widgets/SWindow.h"
#include "Widgets/SOverlay.h"
#include "Widgets/Layout/SBackgroundBlur.h"
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"

#define LOCTEXT_NAMESPACE "Framework"

TSharedPtr<SOverlay> G_AppOverlayWidget;

void SMainFrame::Construct(const FArguments &InArgs)
{
	TSharedPtr<SHorizontalBox> Buttons;
	ChildSlot
		[
			SAssignNew(G_AppOverlayWidget, SOverlay)
			+ SOverlay::Slot()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(16.0f, 8.0f, 0.0f, 0.0f)
				[
					SNew(SCustomTitleBar)
					[
						// Test Code
						SAssignNew(Buttons, SHorizontalBox)
					]
				]
				+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				.Padding(0.0f, 8.0f, 0.0f, 0.0f)
				.Expose(M_ViewportSlot)
			]
		];

	// Test code
	for (int32 i = 0, iEnd = (int32)(EGameContentType::TypeNum); i < iEnd; ++i)
	{
		Buttons->AddSlot()
			.AutoWidth()
			.HAlign(HAlign_Left)
			.VAlign(VAlign_Fill)
			.Padding(i * 8.0f, 0.0f, 0.0f, 0.0f)
			[
				SNew(SBox)
				.WidthOverride(48.0f)
				[
					SNew(SButton)
					.Text(GetGameContent((EGameContentType)i)->GetDisplayName())
					.OnClicked_Lambda([OnSwitchGameContent = InArgs._OnSwitchGameContent, i]() -> FReply {
						OnSwitchGameContent.ExecuteIfBound((EGameContentType)i);
						return FReply::Handled();
					})
				]
			];
	}

	Buttons->AddSlot()
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Fill)
		.Padding(8.0f, 0.0f, 8.0f, 0.0f)
		[
			SNew(SBox)
			.WidthOverride(48.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("Framework_Blur", "Blur"))
				.OnClicked_Lambda([]() -> FReply {
					TSharedPtr<SBackgroundBlur> BackgroundBlur;
					TSharedPtr<SButton> BackgroundBlurButton;
					G_AppOverlayWidget->AddSlot()
					[
						SAssignNew(BackgroundBlur, SBackgroundBlur)
						.HAlign(HAlign_Center)
						.VAlign(VAlign_Center)
						.BlurStrength(50.0f)
						[
							SAssignNew(BackgroundBlurButton, SButton)
							.Text(LOCTEXT("Framework_QuitBlur", "Quit Blur"))
						]
					];
					BackgroundBlurButton->SetOnClicked(FOnClicked::CreateLambda([Blur = BackgroundBlur.Get()]()->FReply {
						G_AppOverlayWidget->RemoveSlot(Blur->AsShared());
						return FReply::Handled();
					}));
					return FReply::Handled();
				})
			]
		];

	Buttons->AddSlot()
		.FillWidth(1.0f)
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Fill)
		.Padding(8.0f, 0.0f, 8.0f, 0.0f)
		[
			SNew(SBox)
			.WidthOverride(48.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("Framework_Minimize", "Minimize"))
				.OnClicked_Lambda([]() -> FReply {
					GEngine->GameViewport->GetWindow()->Minimize();
					return FReply::Handled();
				})
			]
		];

	Buttons->AddSlot()
		.AutoWidth()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Fill)
		.Padding(8.0f, 0.0f, 8.0f, 0.0f)
		[
			SNew(SBox)
			.WidthOverride(48.0f)
			[
				SNew(SButton)
				.Text_Lambda([this]() -> FText {
					return M_bMainFrameMaximized ? LOCTEXT("Framework_Restore", "Restore") : LOCTEXT("Framework_Maximize", "Maximize");
				})
				.OnClicked_Lambda([this]() -> FReply {
					auto MainWindow = GEngine->GameViewport->GetWindow();
					if (M_bMainFrameMaximized)
						MainWindow->ReshapeWindow(FVector2D(GSystemResolution.ResX * 0.125f, GSystemResolution.ResY * 0.125f),
							FVector2D(GSystemResolution.ResX * 0.75f, GSystemResolution.ResY * 0.75f));
					else
						MainWindow->Maximize();
					M_bMainFrameMaximized = !M_bMainFrameMaximized;
					return FReply::Handled();
				})
			]
		];

	Buttons->AddSlot()
		.AutoWidth()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Fill)
		.Padding(8.0f, 0.0f, 8.0f, 0.0f)
		[
			SNew(SBox)
			.WidthOverride(48.0f)
			[
				SNew(SButton)
				.Text(LOCTEXT("Framework_Close", "Close"))
				.OnClicked_Lambda([]() -> FReply {
					G_AppOverlayWidget.Reset();
					GEngine->GameViewport->GetWindow()->RequestDestroyWindow();
					return FReply::Handled();
				})
			]
		];
	// Test code end
}

void SMainFrame::SetViewport(TSharedPtr<SViewport> InViewport)
{
	M_ViewportSlot->operator[] (InViewport.ToSharedRef());
}

#undef LOCTEXT_NAMESPACE
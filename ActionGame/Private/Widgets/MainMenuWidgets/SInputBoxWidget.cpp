// Fill out your copyright notice in the Description page of Project Settings.


#include "SInputBoxWidget.h"
#include "SlateOptMacros.h"
#include "SlateTypes.h"
#include "FActionGameStyle.h"
#include "HAIAIMIHelper.h"
#include "GameFramework/PlayerInput.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SInputBoxWidget::Construct(const FArguments& InArgs)
{
	SelectBoxButtonStyle = &FActionGameStyle::Get().GetWidgetStyle<FButtonStyle>(TEXT("SelectBoxButtonStyle"));
	SelectionOnHovered = InArgs._SelectionOnHovered;
	FString InitContent = InArgs._DefaultSelection;
	InputMappingName = InArgs._InputMappingName;
	PlayerInput = InArgs._PlayerInput;
	bCanChangeText = false;

	FText CurKeyName;
	FString Ref;
	GConfig->GetString(TEXT("haiaimi"), *InputMappingName, Ref, GGameIni);
	if (Ref.IsEmpty())
		CurKeyName = FText::FromString(InitContent);
	else
		CurKeyName = FText::FromString(Ref);
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(*InputMappingName, FKey(*CurKeyName.ToString())));
	
	ChildSlot
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	.VAlign(EVerticalAlignment::VAlign_Fill)
	[
		SNew(SBox)
		.HeightOverride(50.f)
		[
			SNew(SButton)
			.ButtonStyle(SelectBoxButtonStyle)
			.OnHovered_Lambda([&]() {
				if (SelectionName.IsValid())
					SelectionName->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 0.f, 0.f, 1.f)));
				if (SelectionText.IsValid())
					SelectionText->SetColorAndOpacity(FSlateColor(FLinearColor(0.f, 0.f, 0.f, 1.f)));
				SelectionOnHovered.ExecuteIfBound();
				bCanChangeText = true;
				})
			.OnUnhovered_Lambda([&]() {
				if (SelectionName.IsValid())
					SelectionName->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));
				if (SelectionText.IsValid())
					SelectionText->SetColorAndOpacity(FSlateColor(FLinearColor(1.f, 1.f, 1.f, 1.f)));
				bCanChangeText = false;
				})
			[
				SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				.FillWidth(2)
				.HAlign(EHorizontalAlignment::HAlign_Left)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SAssignNew(SelectionName, STextBlock)
					.Text(InArgs._SelectName)
					.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),18))
					.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
				]
				+SHorizontalBox::Slot()
				.FillWidth(1)
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SAssignNew(SelectionText, STextBlock)
					.Text(CurKeyName)
					.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),18))
					.ColorAndOpacity(FSlateColor(FLinearColor(1.f,1.f,1.f,1.f)))
				]
			]
		]
	];
}

FReply SInputBoxWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();
	if (bCanChangeText && Key != PreKey && Key != EKeys::Escape)
	{
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(*InputMappingName, Key));
		SelectionText->SetText(Key.GetDisplayName());
		GConfig->SetString(TEXT("haiaimi"), *InputMappingName, *(Key.ToString()), GGameIni);
		PreKey = Key;
	}

	return SWidget::OnKeyDown(MyGeometry, InKeyEvent);
}
END_SLATE_FUNCTION_BUILD_OPTIMIZATION

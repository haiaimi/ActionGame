// Fill out your copyright notice in the Description page of Project Settings.

#include "SHUDWidget.h"
#include "SlateOptMacros.h"
#include "SAbilityIconWidget.h"
#include "SHealthBarWidget.h"
#include "SEnemySignWidget.h"
#include "GameFramework/PlayerController.h"
#include "ActionGameType.h"
#include "ActionGameInstance.h"
#include "../Styles/UIAssetWidgetStyle.h"
#include "../Styles/FActionGameStyle.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHUDWidget::Construct(const FArguments& InArgs)
{
	Owner = InArgs._Owner;
	int32 PlayerIndex = 0;
	int32 EnemyIndex = 0;
	if (Owner.IsValid())
	{
		PlayerIndex = Owner->GetGameInstance<UActionGameInstance>()->PlayerIndex;
		EnemyIndex = Owner->GetGameInstance<UActionGameInstance>()->EnemyIndex;
	}

	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		[
			SNew(SHorizontalBox)
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(100.f)
			]
			+SHorizontalBox::Slot()
			.FillWidth(1.f)
			[
				SAssignNew(HUDContainer, SVerticalBox)
				+SVerticalBox::Slot()
				.FillHeight(1.f)
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				[
					SNew(SHorizontalBox)
					.RenderTransform_Lambda([&]() {
						const float CurLerp = AbilityIconHandle.GetLerp();
						return FSlateRenderTransform(FVector2D(0.f, (CurLerp-1)*500.f));
					})
					+SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(EHorizontalAlignment::HAlign_Left)
					.VAlign(EVerticalAlignment::VAlign_Center)
					[
						SNew(SHealthBarWidget)
						.Owner(Owner)
						.BarPos(EHorizontalAlignment::HAlign_Left)
						.HeroIndex(PlayerIndex)
					]
					+SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						SNew(SSpacer)
					]
					+SHorizontalBox::Slot()
					.AutoWidth()
					.HAlign(EHorizontalAlignment::HAlign_Right)
					.VAlign(EVerticalAlignment::VAlign_Center)
					[
						SNew(SHealthBarWidget)
						.Owner(Owner->GetAIEnemy())
						.BarPos(EHorizontalAlignment::HAlign_Right)
						.HeroIndex(EnemyIndex)
					]
				]
				+SVerticalBox::Slot()
				.FillHeight(4.f)
				+SVerticalBox::Slot()
				.FillHeight(0.8f)
				.HAlign(EHorizontalAlignment::HAlign_Center)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(SHorizontalBox)
					.RenderTransform_Lambda([&]() {
						const float CurLerp = AbilityIconHandle.GetLerp();
						return FSlateRenderTransform(FVector2D(0.f, (1 - CurLerp)*500.f));
					})
					+SHorizontalBox::Slot()
					.Padding(FMargin(0.f,0.f,20.f,0.f))
					[
						SNew(SAbilityIconWidget)
						.Owner(Owner)
						.AbilityType(EAbilityType::QAbility)
						.PlayerIndex(PlayerIndex)
					]
					+SHorizontalBox::Slot()
					.Padding(FMargin(0.f,0.f,20.f,0.f))
					[
						SNew(SAbilityIconWidget)
						.Owner(Owner)
						.AbilityType(EAbilityType::EAbility)
						.PlayerIndex(PlayerIndex)
					]
					+SHorizontalBox::Slot()
					.Padding(FMargin(0.f,0.f,20.f,0.f))
					[
						SNew(SAbilityIconWidget)
						.RenderTransform(FSlateRenderTransform(FVector2D(0.f, -50.f)))
						.Owner(Owner)
						.AbilityType(EAbilityType::RAbility)
						.PlayerIndex(PlayerIndex)
					]
					+SHorizontalBox::Slot()
					[
						SNew(SAbilityIconWidget)
						.Owner(Owner)
						.AbilityType(EAbilityType::FAbility)
						.PlayerIndex(PlayerIndex)
					]
				]
			]
			+SHorizontalBox::Slot()
			.AutoWidth()
			[
				SNew(SBox)
				.WidthOverride(100.f)
			]
		]
		+SOverlay::Slot()
		.HAlign(EHorizontalAlignment::HAlign_Left)
		.VAlign(EVerticalAlignment::VAlign_Top)
		[
			SNew(SEnemySignWidget)
			.Owner(Owner)
			.Visibility_Lambda([&]() {
				auto MC = Owner->GetController<APlayerController>();
				if (Owner.IsValid() && MC)
					return MC->IsPaused() ? EVisibility::Hidden : EVisibility::Visible;
					
				return EVisibility::Visible;
			})
		]
	];

	SetupAnimation();
	AnimSequence.Play(this->AsShared());
}

void SHUDWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (Owner.IsValid())
	{
		const float CurYawSpeed = Owner->YawSpeed;
		const float PreYawSpeed = -AbilityTransform.X / 10.f;
		const float CurPitchSpeed = Owner->PitchSpeed;
		const float PrePitchSpeed = -AbilityTransform.Y / 10.f;

		AbilityTransform.X += (CurYawSpeed > PreYawSpeed) ? -200.f*InDeltaTime : 200.f*InDeltaTime;
		AbilityTransform.Y += (CurPitchSpeed > PrePitchSpeed) ? -200.f*InDeltaTime : 200.f*InDeltaTime;
		if (-AbilityTransform.X / 10.f >= CurYawSpeed || -AbilityTransform.X / 10.f <= CurYawSpeed)
			AbilityTransform.X = -CurYawSpeed * 10.f;
		if (-AbilityTransform.Y / 10.f >= CurPitchSpeed || -AbilityTransform.Y / 10.f <= CurPitchSpeed)
			AbilityTransform.Y = -CurPitchSpeed * 10.f;
		HUDContainer->SetRenderTransform(FSlateRenderTransform(AbilityTransform));
	}
}

void SHUDWidget::ReverseAnim()
{
	AnimSequence.Reverse();

}

void SHUDWidget::SetupAnimation()
{
	AnimSequence = FCurveSequence();
	AbilityIconHandle = AnimSequence.AddCurve(0.f, 0.5f, ECurveEaseFunction::CubicOut);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

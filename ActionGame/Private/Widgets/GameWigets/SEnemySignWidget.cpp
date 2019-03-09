// Fill out your copyright notice in the Description page of Project Settings.

#include "SEnemySignWidget.h"
#include "SlateOptMacros.h"
#include "../Styles/FActionGameStyle.h"
#include "HAIAIMIHelper.h"
#include "GameFramework/PlayerController.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SEnemySignWidget::Construct(const FArguments& InArgs)
{
	AIEnemy = nullptr;
	UIStyle = &FActionGameStyle::Get().GetWidgetStyle<FUIAssetStyle>(TEXT("ActionGameUIAssetStyle"));
	Owner = InArgs._Owner;

	ChildSlot
	[
		SNew(SBox)
		.WidthOverride(50.f)
		.HeightOverride(50.f)
		[
			SNew(SImage)
			.Image(&UIStyle->EnemySign)
		]
	];

	SetRenderTransformPivot(FVector2D(0.5f, 0.5f));
}

void SEnemySignWidget::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SCompoundWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	if (Owner.IsValid() && AIEnemy.IsValid())
	{
		//HAIAIMIHelper::Debug_ScreenMessage(TEXT("See Enemy"));
		//APlayerController* MyController = Owner->GetController<APlayerController>();
		////if (!MyController)return;
		//
		//FVector2D ScreenPos;
		//if (MyController->ProjectWorldLocationToScreen(AIEnemy->GetActorLocation() + FVector(0.f, 0.f, 90.f), ScreenPos))
		//{
		//	SetVisibility(EVisibility::Hidden);
		//}
		//else
		//	SetVisibility(EVisibility::Visible);
		
		const FVector ForwardDir = Owner->GetControlRotation().Vector().GetSafeNormal2D();
		const FVector RightDir = FRotationMatrix(Owner->GetControlRotation()).GetScaledAxis(EAxis::Y);
		const FVector EnemyToPlayer = (AIEnemy->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal2D();
		
		const bool bInRight = FVector::DotProduct(EnemyToPlayer, RightDir) > 0.f;
		const float Radian = FMath::Acos(FVector::DotProduct(ForwardDir, EnemyToPlayer));

		FVector2D WidgetPos;
		FQuat2D WidgetRot;
		
		if (Radian >= 0.f && Radian < 1.06f)
		{
			WidgetPos.Y = 0.f;
			WidgetPos.X = bInRight ? 960.f + FMath::Tan(Radian)*540.f : 960.f - FMath::Tan(Radian)*540.f;
			WidgetRot = FQuat2D(FMath::DegreesToRadians(-90.f));
		}
		else if (Radian >= 1.06f && Radian < 2.08f)
		{
			WidgetPos.X = bInRight ? 1870.f : 0.f;
			if (Radian <= 1.57f)
				WidgetPos.Y = -960.f*FMath::Tan(1.57f - Radian) + 540.f;
			else
				WidgetPos.Y = 960.f*FMath::Tan(Radian - 1.57f) + 540.f;
			WidgetRot = FQuat2D(FMath::DegreesToRadians(bInRight ? 0.f : -180.f));
		}
		else
		{
			WidgetPos.Y = 1030.f;
			WidgetPos.X = bInRight ? 960.f + FMath::Tan(3.14f - Radian)*540.f : 960.f - FMath::Tan(3.14f - Radian)*540.f;
			WidgetRot = FQuat2D(FMath::DegreesToRadians(90.f));
		}

		SetRenderTransform(FSlateRenderTransform(WidgetRot, WidgetPos));
	}
	else
	{
		if (Owner.IsValid())
			AIEnemy = Owner->GetAIEnemy();
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
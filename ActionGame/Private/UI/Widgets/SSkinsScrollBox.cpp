// Fill out your copyright notice in the Description page of Project Settings.

#include "SSkinsScrollBox.h"
#include "SlateOptMacros.h"
#include "Engine/Engine.h"
#include "ActionGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "HAIAIMIHelper.h"

using FRenderTransformParam = TAttribute<TOptional<FSlateRenderTransform>>;

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSkinsScrollBox::Construct(const FArguments& InArgs)
{
	SkinImageBrushs = InArgs._SkinImageBrushs;
	OwnerHUD = InArgs._OwnerHUD;
	CurSelectIndex = 0;
	ChildSlot
	.HAlign(EHorizontalAlignment::HAlign_Fill)
	.VAlign(EVerticalAlignment::VAlign_Fill)
	[
		SAssignNew(SkinContainer, SScrollBox)
		.Orientation(EOrientation::Orient_Horizontal)
		.ScrollBarVisibility(EVisibility::Hidden)
	];

	SetSkinItems();
}

void SSkinsScrollBox::SetupAnimation(int32 ItemCounts)
{
	AnimSequence = FCurveSequence();
	AnimHandles.SetNum(ItemCounts);

	for (int32 i = 0; i < ItemCounts; ++i)
	{
		AnimHandles[i] = AnimSequence.AddCurve(0.1f*i, 0.2f, ECurveEaseFunction::CubicOut);
	}
}

void SSkinsScrollBox::SetSkinImages(const TArray<struct FSlateBrush>* InImages)
{
	SkinImageBrushs = InImages;
	SetSkinItems();
}

void SSkinsScrollBox::SetSkinItems()
{
	if(SkinContainer.IsValid() && SkinImageBrushs)
	{
		SetupAnimation(SkinImageBrushs->Num());
		while ((*SkinImageBrushs).Num() < SkinItems.Num() && SkinItems.Num() > 0)
		{
			SkinContainer->RemoveSlot(SkinItems.Last().ToSharedRef());
			SkinItems.RemoveAt(SkinItems.Num() - 1);
		}
		SkinItems.SetNumZeroed((*SkinImageBrushs).Num());
		for (int32 i = 0; i < SkinItems.Num(); i++)
		{
			if (SkinItems[i].IsValid())
				SkinItems[i]->SetImage(&(*SkinImageBrushs)[i]);
			else
			{
				SkinContainer->AddSlot()
				.HAlign(EHorizontalAlignment::HAlign_Fill)
				.VAlign(EVerticalAlignment::VAlign_Fill)
				[
					SAssignNew(SkinItems[i], SHeroShowItem)
					.BorderScale(FVector2D(0.9f, 0.9f))
					.ImageScale(FVector2D(0.87f, 0.91f))
					.HoverScale(1.1f)
					.Image(&(*SkinImageBrushs)[i])
					.RenderTransform_Lambda([i, this]() {
						const float CurLerp = AnimHandles[i].GetLerp();
						return FSlateRenderTransform(FVector2D((SkinItems.Num() - i)*310.f*(1.f - CurLerp), 0.f));
						})
					.OnPressed_Lambda([i, this]() {
						if(OwnerHUD.IsValid())
						{
							auto MyInstance = OwnerHUD->GetGameInstance<UActionGameInstance>();
							if (MyInstance)MyInstance->PlayerSkinIndex = i;
						}
						CurSelectIndex = i;
						})
					.IsEnabled_Lambda([i, this]() {
							return CurSelectIndex != i;
						})
				];
			}
		}
		AnimSequence.Play(this->AsShared());
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
// Fill out your copyright notice in the Description page of Project Settings.

#include "SSkinsScrollBox.h"
#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SSkinsScrollBox::Construct(const FArguments& InArgs)
{
	SkinImageBrushs = InArgs._SkinImageBrushs;

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

void SSkinsScrollBox::SetSkinImages(TArray<struct FSlateBrush>* InImages)
{
	SkinImageBrushs = InImages;
	SetSkinItems();
}

void SSkinsScrollBox::SetSkinItems()
{
	if(SkinContainer.IsValid() && SkinImageBrushs)
	{
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
					.BorderScale(FVector2D(1.f, 1.f))
					.ImageScale(FVector2D(0.87f, 0.91f))
					.HoverScale(1.1f)
					.Image(&(*SkinImageBrushs)[i])
				];
			}
		}
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
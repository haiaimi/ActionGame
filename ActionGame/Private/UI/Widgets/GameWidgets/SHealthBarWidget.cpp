// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthBarWidget.h"
#include "SlateOptMacros.h"
#include "../Styles/FActionGameStyle.h"
#include "Materials/MaterialInterface.h"
#include "ActionGameType.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION
void SHealthBarWidget::Construct(const FArguments& InArgs)
{
	HealthBarMaterialDInstance = nullptr;
	UIStyle = &FActionGameStyle::Get().GetWidgetStyle<FUIAssetStyle>(TEXT("ActionGameUIAssetStyle"));
	Owner = InArgs._Owner;

	if (InArgs._BarPos == EHorizontalAlignment::HAlign_Left || InArgs._BarPos == EHorizontalAlignment::HAlign_Right)
		BarPos = InArgs._BarPos;
	else
		BarPos = EHorizontalAlignment::HAlign_Left;

	UObject* Resource = UIStyle->HealthBarImage.GetResourceObject();
	UMaterialInterface*  HeathBarMaterial = Cast<UMaterialInterface>(Resource);

	if (HeathBarMaterial)
	{
		HealthBarMaterialDInstance = Cast<UMaterialInstanceDynamic>(HeathBarMaterial);
		if (!HealthBarMaterialDInstance)
		{
			UMaterialInstanceDynamic* DynamicMaterialInstance = UMaterialInstanceDynamic::Create(HeathBarMaterial, Owner.Get());
			if (DynamicMaterialInstance)
			{
				HealthBarMaterialDInstance = DynamicMaterialInstance;
			}
		}
	}
	
#if WITH_EDITOR
	//防止UI材质被回收
	if (!GIsEditor)
	{
		HealthBarMaterialDInstance->AddToCluster(Owner.Get());   
		HealthBarMaterialDInstance->AddToRoot();
	}
#endif // !Editor

	FSlateBrush* MaterialBrush = new FSlateBrush;
	MaterialBrush->SetResourceObject(HealthBarMaterialDInstance);

	TSharedPtr<SBox> Head = 
		SNew(SBox)
		.WidthOverride(110.f)
		.HeightOverride(110.f)
		[
			SNew(SBorder)
			.Padding(5.f)
			[
				SAssignNew(HeroHead, SImage)
				.Image(&UIStyle->HeroHeadImages[InArgs._HeroIndex])
			]
		];
	
	TSharedPtr<SBox> Bar = 
		SNew(SBox)
		.WidthOverride(500.f)
		.HeightOverride(110.f)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.FillHeight(1.f)
			[
				SNew(SBox)
				.HAlign(BarPos)
				.VAlign(EVerticalAlignment::VAlign_Center)
				[
					SNew(STextBlock)
					.Text(HeroNameText[InArgs._HeroIndex])
					.Font(FSlateFontInfo(FPaths::ProjectContentDir()/TEXT("UI/Fonts/NanumGothic.ttf"),25))
					.ColorAndOpacity(FSlateColor(FLinearColor(0.8f,0.8f,0.8f,1.f)))
					.RenderTransform(FSlateRenderTransform(FVector2D(BarPos == EHorizontalAlignment::HAlign_Left ? 50.f : -50.f, 0.f)))
				]
			]
			+SVerticalBox::Slot()
			.FillHeight(1.f)
			[
				SAssignNew(HealthBar, SImage)
				.Image(MaterialBrush)
				.RenderTransformPivot(FVector2D(0.5f, 0.5f))
			]
		];

	ChildSlot
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			BarPos == EHorizontalAlignment::HAlign_Left ? Head.ToSharedRef() : Bar.ToSharedRef()
		]
		+SHorizontalBox::Slot()
		.AutoWidth()
		[
			BarPos == EHorizontalAlignment::HAlign_Left ? Bar.ToSharedRef() : Head.ToSharedRef()
		]
	];

	if (BarPos == EHorizontalAlignment::HAlign_Right && HealthBar.IsValid() && HealthBarMaterialDInstance)
	{
		HealthBarMaterialDInstance->SetScalarParameterValue(TEXT("ProgressSlope"), 0.2);
		HealthBarMaterialDInstance->SetScalarParameterValue(TEXT("ProgressDirection"), 20.f);
		HealthBarMaterialDInstance->SetScalarParameterValue(TEXT("ShapeSlopeX"), 0.1f);
	}
	
	if (Owner.IsValid())
	{
		Owner->OnHealthChanged.BindSP(this, &SHealthBarWidget::SetHealthBar);
	}

	SetHealthBar(0.54f);
}
void SHealthBarWidget::SetHealthBar(float NewHealth)
{
	if (HealthBar.IsValid() && HealthBarMaterialDInstance)
	{
		HealthBarMaterialDInstance->SetScalarParameterValue(TEXT("Progress"), BarPos == EHorizontalAlignment::HAlign_Left ? NewHealth : 1 - NewHealth);
	}
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Styling/SlateWidgetStyle.h"
#include "SlateWidgetStyleContainerBase.h"
#include "ActionGameType.h"

#include "UIAssetWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct FUIAssetStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FUIAssetStyle();
	virtual ~FUIAssetStyle();

	UPROPERTY(EditAnywhere, Category = Appearance)
	struct FSlateBrush DividingLine;

	UPROPERTY(EditAnywhere, Category = Appearance)
	struct FSlateBrush HeroItemBorder;

	UPROPERTY(EditAnywhere, Category = Appearance)
	TArray<struct FSlateBrush> HeroHeadImages;

	UPROPERTY(EditAnywhere, Category = Appearance)
	TArray<struct FBrushesContainer> Skins;

	UPROPERTY(EditAnywhere, Category = Appearance)
	TArray<struct FBrushesContainer> AbilityIcons;

	UPROPERTY(EditAnywhere, Category = Appearance)
	struct FSlateBrush HealthBarImage;

	UPROPERTY(EditAnywhere, Category = Appearance)
	struct FSlateBrush EnemySign;

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FUIAssetStyle& GetDefault();
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UUIAssetWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FUIAssetStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &WidgetStyle );
	}
};

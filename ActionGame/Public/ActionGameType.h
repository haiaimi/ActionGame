// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionGameType.generated.h"

#define COLLISION_ICETRACE ECollisionChannel::ECC_GameTraceChannel1

UENUM(BlueprintType)
namespace EMoveDir
{
	enum Type
	{
		Forward,
		Backward,
		Left,
		Right
	};
}

UENUM(BlueprintType)
namespace EAbilityType
{
	enum Type
	{
		QAbility = 1,
		EAbility,
		RAbility,
		FAbility
	};
}

USTRUCT(BlueprintType)
struct FCharacterInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	FText CharName;

	UPROPERTY(EditDefaultsOnly)
	TArray<class USkeletalMesh*> CharMeshs;

	UPROPERTY(EditDefaultsOnly)
	TArray<class UAnimMontage*> AbilityAnims;

	UPROPERTY(EditDefaultsOnly)
	TArray<FText> MeshNames;
};


USTRUCT(BlueprintType)
struct FBrushesContainer
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Images)
	TArray<struct FSlateBrush> Images;
};

#define LOCTEXT_NAMESPACE "ActionGame.UI.HeroName"

static const TArray<FText> HeroNameText = { LOCTEXT("Aurora","奥拉"),LOCTEXT("Countess","康缇斯") };

#undef LOCTEXT_NAMESPACE
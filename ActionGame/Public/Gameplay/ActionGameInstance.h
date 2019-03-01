// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ActionGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UActionGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UActionGameInstance();

	virtual void StartGameInstance()override;

	TArray<TSubclassOf<class AActionGameCharacter>>& GetCharacterClass() { return ToSpawnCharacter; }

	TSubclassOf<class AActionGameCharacter> GetToSpawnPlayerClass();

public:
	int32 PlayerIndex;

	int32 PlayerSkinIndex;

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class AActionGameCharacter>> ToSpawnCharacter;
};
 
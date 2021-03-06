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

	TSubclassOf<class AActionGameCharacter> GetToSpawnAIClass();

public:
	int32 PlayerIndex;

	int32 PlayerSkinIndex;

	int32 EnemyIndex;

	int32 EnemySkinIndex;

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class AActionGameCharacter>> ToSpawnCharacter;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<class AActionGameCharacter>> ToSpawnAICharacter;
};
 
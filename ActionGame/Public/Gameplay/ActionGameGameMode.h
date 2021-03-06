// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ActionGameGameMode.generated.h"

UCLASS(minimalapi)
class AActionGameGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AActionGameGameMode();

	virtual void RestartPlayer(AController* NewPlayer)override;

	virtual APlayerController* Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)override;

	virtual void PostLogin(APlayerController* NewPlayer)override;
};




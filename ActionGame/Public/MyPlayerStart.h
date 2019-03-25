// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "MyPlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API AMyPlayerStart : public APlayerStart
{
	GENERATED_BODY()
	
public:
	void PostInitializeComponents()override;

	void BeginPlay()override;
};

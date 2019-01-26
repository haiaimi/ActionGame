// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionGameType.generated.h"

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
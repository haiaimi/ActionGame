// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindEnemyPoint.generated.h"

/**
 * 
 */
UCLASS()
class ACTIONGAME_API UBTTask_FindEnemyPoint : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};

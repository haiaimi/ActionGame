// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTask_FindEnemyPoint.h"
#include <BehaviorTree/BehaviorTreeTypes.h>
#include "AI/ActionAIController.h"
#include "ActionGameCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include <NavigationSystem.h>

EBTNodeResult::Type UBTTask_FindEnemyPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActionAIController* MyController = Cast<AActionAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
		return EBTNodeResult::Failed;

	APawn* MyBot = MyController->GetPawn();
	AActionGameCharacter* Enemy = MyController->GetEnemy();

	if(Enemy && MyBot)
	{
		const float SearchRadius = 200.f;
		//const FVector SearchOrigin=Enemy->GetActorLocation()+600.f*()
		FVector Result;
		UNavigationSystemV1::K2_GetRandomReachablePointInRadius(MyController, Enemy->GetActorLocation(), Result, SearchRadius);
		if (Result != FVector::ZeroVector)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), Result);
			return EBTNodeResult::Succeeded;
		}
	}
	
	return EBTNodeResult::Failed;
}

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

	MyController->SetAIFreezedValue();
	APawn* MyBot = MyController->GetPawn();
	AActionGameCharacter* Enemy = MyController->GetEnemy();

	if(Enemy && MyBot)
	{
		const float SearchRadius = 200.f;
		FVector Result = Enemy->GetActorLocation() + (MyBot->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal()*100.f;
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), Result);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

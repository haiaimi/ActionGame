// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTask_FindEnemyPoint.h"
#include <BehaviorTree/BehaviorTreeTypes.h>
#include "AI/ActionAIController.h"
#include "ActionGameCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include <NavigationSystem.h>
#include "ActionGameBot_Aurora.h"

EBTNodeResult::Type UBTTask_FindEnemyPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActionAIController* MyController = Cast<AActionAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
		return EBTNodeResult::Failed;

	//MyController->SetAIFreezedValue();
	APawn* MyBot = MyController->GetPawn();
	AActionGameCharacter* Enemy = MyController->GetEnemy();

	if(Enemy && MyBot)
	{
		FVector Result;
		if(MyController->IsMovingBack())
		{
			const float Distance = (MyBot->GetActorLocation() - Enemy->GetActorLocation()).Size2D();
			if (Distance < 1000)
			{
				Result = MyBot->GetActorLocation() - MyBot->GetActorRotation().Vector()*100.f;
				if (AActionGameBot_Aurora* MyBotA = Cast<AActionGameBot_Aurora>(MyBot))
					MyBotA->SetAIRotation((Enemy->GetActorLocation() - MyBot->GetActorLocation()).GetSafeNormal().ToOrientationRotator());
			}

		}
		else
			Result = Enemy->GetActorLocation() + (MyBot->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal()*100.f;
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), Result);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

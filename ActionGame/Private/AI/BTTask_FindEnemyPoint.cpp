// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/BTTask_FindEnemyPoint.h"
#include <BehaviorTree/BehaviorTreeTypes.h>
#include "AI/ActionAIController.h"
#include "ActionGameCharacter.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include <NavigationSystem.h>
#include "ActionGameBot_Aurora.h"
#include "GameBotInterface.h"
#include "HAIAIMIHelper.h"

EBTNodeResult::Type UBTTask_FindEnemyPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActionAIController* MyController = Cast<AActionAIController>(OwnerComp.GetAIOwner());
	if (MyController == nullptr)
		return EBTNodeResult::Failed;

	APawn* MyBot = MyController->GetPawn();
	AActionGameCharacter* Enemy = MyController->GetEnemy();
	//HAIAIMIHelper::Debug_ScreenMessage(Enemy->GetName());
	if(Enemy && MyBot)
	{
		FVector Result;
		if(MyController->IsMovingBack())
		{
			const float Distance = (MyBot->GetActorLocation() - Enemy->GetActorLocation()).Size2D();
			if (Distance < 1000)
			{
				Result = MyBot->GetActorLocation() - MyBot->GetActorRotation().Vector()*100.f;
				if (IGameBotInterface* MyBotA = Cast<IGameBotInterface>(MyBot))
					MyBotA->SetAIRotation((Enemy->GetActorLocation() - MyBot->GetActorLocation()).GetSafeNormal2D().ToOrientationRotator());
			}
		}
		else
			Result = Enemy->GetActorLocation() + (MyBot->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal()*100.f;
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), Result);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

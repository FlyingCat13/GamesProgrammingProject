// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportTask.h"
#include "Enemy.h"
#include "EnemyAIController.h"

// Behavior tree task to call the random location function in the controller.
EBTNodeResult::Type UTeleportTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBehaviorTreeComponent* Component = &OwnerComp;

	if (!Component)
	{
		return EBTNodeResult::Failed;
	}

	AEnemyAIController* MyController = Cast<AEnemyAIController>(Component->GetOwner());

	if (!MyController)
	{
		return EBTNodeResult::Failed;
	}

	AEnemy* Enemy = Cast<AEnemy>(MyController->GetCharacter());
	
	if (!Enemy)
	{
		return EBTNodeResult::Failed;
	}

	FVector LocationToTeleport = MyController->BlackboardComponent->GetValueAsVector("PatrolPoint");
	LocationToTeleport.Z = 120.f;
	Enemy->SetActorLocation(LocationToTeleport);

	return EBTNodeResult::Succeeded;
}
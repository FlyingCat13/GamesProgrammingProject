// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckMatchLitTask.h"
#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayer.h"

// Task to stop Enemy pursuit when the player has the match light on
EBTNodeResult::Type UCheckMatchLitTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMainPlayer* Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (!Player)
	{
		return EBTNodeResult::Failed;
	}

	// Delibrately fail if the player is in stealth. This will stop the chasing sequence in the blueprint.
	if (Player->IsStealth())
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}

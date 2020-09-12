// Fill out your copyright notice in the Description page of Project Settings.


#include "EndPointTrigger.h"
#include "AssignmentGameMode.h"
#include "MainPlayer.h"

AEndPointTrigger::AEndPointTrigger()
{
	// Add callback to overlap event.
	OnActorBeginOverlap.AddDynamic(this, &AEndPointTrigger::OnOverlapBegin);
}

void AEndPointTrigger::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// Check for win condition, i.e. the other actor is the player and they have the designated item
	// in the inventory.
	if (OtherActor && OtherActor != this)
	{
		AMainPlayer* Player = Cast<AMainPlayer>(OtherActor);
		if (Player && Player->IsInInventory(WinConditionItem))
		{
			AAssignmentGameMode* GameMode = Cast<AAssignmentGameMode>(GetWorld()->GetAuthGameMode());
			GameMode->OnWin();
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoorTrigger.h"
#include "Fuse.h"
#include "GameFramework/Actor.h"
#include "FusedDoorTrigger.generated.h"

UCLASS()
class ASSIGNMENT_API AFusedDoorTrigger : public ADoorTrigger
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFusedDoorTrigger();

	// The fuse that can activate the trigger.
	UPROPERTY(EditAnywhere)
		AFuse* Fuse;

	// Overriding default trigger's Interactable functions
	virtual void Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip) override;
	virtual void Showcase_Implementation(FText& Tooltip) override;

protected:
	bool IsActivated = false;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};

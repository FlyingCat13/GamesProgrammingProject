// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableDoor.h"
#include "DoorKey.h"
#include "DoorTrigger.h"
#include "LockableDoor.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API ALockableDoor : public AInteractableDoor
{
	GENERATED_BODY()
	
public:
	ALockableDoor();

	// The trigger to unlock the door that can be set in the editor later.
	UPROPERTY(EditAnywhere)
		ADoorTrigger* DoorTrigger;

	// The key to unlock the door that can be set in the editor later.
	UPROPERTY(EditAnywhere)
		ADoorKey* UnlockKey;

	// Override interact behaviour from normal door.
	virtual void Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip) override;

	// UFUNCTION so that it can be used with AddDynamic
	UFUNCTION()
		void Unlock();

protected:
	bool IsLocked = true;

	virtual void BeginPlay() override;
};

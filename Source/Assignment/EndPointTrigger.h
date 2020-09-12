// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "EndPointTrigger.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API AEndPointTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	AEndPointTrigger();

	// The item the player needs to hold to win. To be set in the editor.
	UPROPERTY(EditAnywhere)
		AActor* WinConditionItem;

	// Callback for overlap event
	UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
};

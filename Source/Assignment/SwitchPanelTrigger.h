// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DoorTrigger.h"
#include "Switch.h"
#include "SwitchPanelTrigger.generated.h"

#define SWITCH_COUNT 3

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API ASwitchPanelTrigger : public ADoorTrigger
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASwitchPanelTrigger();

	// Overriding Interactable functions
	virtual void Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip) override;
	virtual void Showcase_Implementation(FText& Tooltip) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UMaterialInstanceDynamic* DynamicMaterialInstance;

	// The switches that are linked to the panel.
	UPROPERTY(EditAnywhere)
		ASwitch* Switches[SWITCH_COUNT];
	// The correct switch combination.
	bool UnlockCombination[SWITCH_COUNT] = { false, true, true };

	// Check if the switch combination is correct (checked every tick).
	bool CheckUnlock();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};

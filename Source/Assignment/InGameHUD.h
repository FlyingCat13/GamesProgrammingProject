// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "InGameHUD.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API UInGameHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool Initialize() override;

	// Healthbar
	UPROPERTY(EditAnywhere)
		UProgressBar* HealthBar;

	// Tooltip to indicate and describe an interactable that is within reach.
	UPROPERTY(EditAnywhere)
		UTextBlock* LookAtItemTooltipTextBlock;

	// Tooltip to describe the effect of the interactable after interaction.
	UPROPERTY(EditAnywhere)
		UTextBlock* InteractedItemTooltipTextBlock;

	// Update the inventory array of text blocks
	void UpdateInventoryInformation(TArray<FText> InventoryItemNames, int EquippedItem);

protected:
	const int MAX_INVENTORY_SLOT_COUNT = 10;

	// Array of text blocks that contains the inventory information.
	UPROPERTY()
		TArray<UTextBlock*> InventoryItems;
};

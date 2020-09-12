// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MainPlayer.h"
#include "UObject/Interface.h"
#include "Inventoriable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoriable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASSIGNMENT_API IInventoriable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Apply effect on player on equipping the item.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnEquip(AMainPlayer* InteractingPlayer);

	// Apply effect on player on unequipping the item.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnUnequip(AMainPlayer* InteractingPlayer);
};

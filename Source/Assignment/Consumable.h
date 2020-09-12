// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Consumable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UConsumable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ASSIGNMENT_API IConsumable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Item will initiate consumtion when this function is called.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnConsume(AMainPlayer* InteractingPlayer);
};

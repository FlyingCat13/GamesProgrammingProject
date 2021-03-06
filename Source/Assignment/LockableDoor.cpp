// Fill out your copyright notice in the Description page of Project Settings.


#include "LockableDoor.h"

ALockableDoor::ALockableDoor() : AInteractableDoor()
{

}

void ALockableDoor::BeginPlay()
{
	Super::BeginPlay();

	// By default, locked door glows red.
	if (DoorFrameMaterialInstance && DoorMaterialInstance)
	{
		DoorFrameMaterialInstance->SetVectorParameterValue("Emissive Colour", FLinearColor::Red);
		DoorFrameMaterialInstance->SetScalarParameterValue("Emissive Value", 3.f);

		DoorMaterialInstance->SetVectorParameterValue("Emissive Colour", FLinearColor::Red);
		DoorMaterialInstance->SetScalarParameterValue("Emissive Value", 3.f);
	}

	// If this door is unlockable via a trigger, subscribe to that trigger's unlock event.
	if (DoorTrigger != nullptr)
	{
		DoorTrigger->OnUnlockDoorEvent.AddDynamic(this, &ALockableDoor::Unlock);
	}
}

// When the player is equpping the key to the door, it can be unlocked and opened.
// If locked, the player cannot toggle it.
// If unlocked, the player can toggle normally.
void ALockableDoor::Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip)
{
	if (!IsLocked)
	{
		Tooltip = FText::GetEmpty();
		AInteractableDoor::ToggleOpen();
	}
	else if (UnlockKey != nullptr && UnlockKey == InteractingPlayer->GetEquippedItem())
	{
		// Set tooltip when unlocked
		Tooltip = FText::FromString("Well would you look at that. The key works!");
		Unlock();
		InteractingPlayer->RemoveItemFromInventory(UnlockKey);
		UnlockKey->Destroy();
	}
	else
	{
		// Set tooltip when attempt to open a locked door.
		Tooltip = FText::FromString("The door is locked. A key might be the solution");
	}
}

// Unlock and open the door
void ALockableDoor::Unlock()
{
	if (IsLocked)
	{
		// The door glows green when unlocked.
		if (DoorFrameMaterialInstance && DoorMaterialInstance)
		{
			DoorFrameMaterialInstance->SetVectorParameterValue("Emissive Colour", FLinearColor::Green);
			DoorFrameMaterialInstance->SetScalarParameterValue("Emissive Value", 3.f);

			DoorMaterialInstance->SetVectorParameterValue("Emissive Colour", FLinearColor::Green);
			DoorMaterialInstance->SetScalarParameterValue("Emissive Value", 3.f);
		}
		IsLocked = false;
		AInteractableDoor::ToggleOpen();
	}
}
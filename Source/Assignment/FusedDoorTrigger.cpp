// Fill out your copyright notice in the Description page of Project Settings.


#include "FusedDoorTrigger.h"

// Sets default values
AFusedDoorTrigger::AFusedDoorTrigger() : ADoorTrigger()
{
	// Set custom material for later dynamic use
	ConstructorHelpers::FObjectFinder<UMaterial> BaseMaterial(TEXT("/Game/StarterContent/Materials/TechMaterial.TechMaterial"));
	if (BaseMaterial.Succeeded())
	{
		BaseMesh->SetMaterial(0, BaseMaterial.Object);
	}
}

// Called when the game starts or when spawned
void AFusedDoorTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	// Set up dynamic material instance.
	UMaterialInterface* Material = BaseMesh->GetMaterial(0);
	DynamicMaterialInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);
	if (DynamicMaterialInstance != nullptr)
	{
		DynamicMaterialInstance->SetVectorParameterValue("Emissive Colour", FLinearColor::Red);
	}
}

void AFusedDoorTrigger::Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip)
{
	if (!IsActivated)
	{
		if (Fuse != nullptr && Fuse == InteractingPlayer->GetEquippedItem())
		{
			IsActivated = true;
			// Output the tooltip to display on HUD
			Tooltip = FText::FromString("That was not conFusing at all!\nA door has been unlocked");
			// Unlock the designated door by broadcasting an event.
			OnUnlockDoorEvent.Broadcast();
			// Change colours when triggered.
			if (DynamicMaterialInstance != nullptr)
			{
				DynamicMaterialInstance->SetVectorParameterValue("Emissive Colour", FLinearColor::Green);
			}
			// Remove Fuse from ... everywhere.
			InteractingPlayer->RemoveItemFromInventory(Fuse);
			Fuse->Destroy();
		}
		else
		{
			// Set tooltip when attempt to activate without a fuse.
			Tooltip = FText::FromString("This might need some kinda tool to be activated.");
		}
	}
}

void AFusedDoorTrigger::Showcase_Implementation(FText& Tooltip)
{
	// Output the tooltip to display on HUD
	Tooltip = FText::FromString("Who would wanna put this contraption in the house?.\nPress E to interact");
}
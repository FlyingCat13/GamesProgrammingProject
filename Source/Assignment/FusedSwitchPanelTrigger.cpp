// Fill out your copyright notice in the Description page of Project Settings.


#include "FusedSwitchPanelTrigger.h"

void AFusedSwitchPanelTrigger::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 0; i < SWITCH_COUNT; i++)
	{
		if (Switches[i] != nullptr)
		{
			Switches[i]->SetIsDisabled(true);
		}
	}
}

void AFusedSwitchPanelTrigger::Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip)
{
	// Only have meaningful interactions when it is not activated.
	if (!IsActivated)
	{
		// If the equipped item is indeed a fuse, unlock the door.
		if (Fuse != nullptr && Fuse == InteractingPlayer->GetEquippedItem())
		{
			IsActivated = true;
			// Output the tooltip to display on HUD
			Tooltip = FText::FromString("That was not conFusing at all!\nThe panel is now powered");
			// Make the switches flippable.
			for (int i = 0; i < SWITCH_COUNT; i++)
			{
				if (Switches[i] != nullptr)
				{
					Switches[i]->SetIsDisabled(false);
				}
			}
			// Change colours when triggered.
			if (DynamicMaterialInstance != nullptr)
			{
				DynamicMaterialInstance->SetVectorParameterValue("Emissive Colour", FLinearColor::Yellow);
			}
			// Remove Fuse from ... everywhere.
			InteractingPlayer->RemoveItemFromInventory(Fuse);
			Fuse->Destroy();
		}
		else
		{
			// Set tooltip when attempt to activate without a fuse.
			Tooltip = FText::FromString("This might need some kinda tool to be powered.");
		}
	}
}

void AFusedSwitchPanelTrigger::Showcase_Implementation(FText& Tooltip)
{
	if (!IsActivated) {
		// Output the tooltip to display on HUD
		Tooltip = FText::FromString("The panel seems to have no power.\nPress E to interact");
	} else if (!CheckUnlock())
	{
		Tooltip = FText::FromString("Hmm a switch panel. Try a combination or something!");
	}
	else
	{
		Tooltip = FText::FromString("Panel activated the door right next to it!");
	}

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchPanelTrigger.h"

ASwitchPanelTrigger::ASwitchPanelTrigger() : ADoorTrigger()
{
	// Set a box as mesh
	ConstructorHelpers::FObjectFinder<UStaticMesh> BaseMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (BaseMeshAsset.Succeeded())
	{
		BaseMesh->SetStaticMesh(BaseMeshAsset.Object);
		BaseMesh->SetWorldScale3D(FVector(1.5f, .1f, .8f));
	}

	// Set custom material for later dynamic use
	ConstructorHelpers::FObjectFinder<UMaterial> BaseMaterial(TEXT("/Game/StarterContent/Materials/TechMaterial.TechMaterial"));
	if (BaseMaterial.Succeeded())
	{
		BaseMesh->SetMaterial(0, BaseMaterial.Object);
	}
}

// Called when the game starts or when spawned
void ASwitchPanelTrigger::BeginPlay()
{
	Super::BeginPlay();

	// Set up dynamic material instance.
	UMaterialInterface* Material = BaseMesh->GetMaterial(0);
	DynamicMaterialInstance = BaseMesh->CreateDynamicMaterialInstance(0, Material);
	if (DynamicMaterialInstance != nullptr)
	{
		// Glows red by default.
		DynamicMaterialInstance->SetVectorParameterValue("Emissive Colour", FLinearColor::Red);
	}
}

// Called every frame
void ASwitchPanelTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CheckUnlock())
	{
		// Unlock the designated door by broadcasting an event.
		OnUnlockDoorEvent.Broadcast();
		// Change colours when triggered.
		if (DynamicMaterialInstance != nullptr)
		{
			// Glows green when correctly activated.
			DynamicMaterialInstance->SetVectorParameterValue("Emissive Colour", FLinearColor::Green);
		}

		for (int i = 0; i < SWITCH_COUNT; i++)
		{
			if (Switches[i] != nullptr)
			{
				Switches[i]->SetIsDisabled(true);
			}
		}
	}
}

// Go through all the switches and check it against the correct combination.
bool ASwitchPanelTrigger::CheckUnlock()
{
	for (int i = 0; i < SWITCH_COUNT; i++)
	{
		if (Switches[i] == nullptr || Switches[i]->GetValue() != UnlockCombination[i])
		{
			return false;
		}
	}
	return true;
}

// Base panel is not where to interact.
void ASwitchPanelTrigger::Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip)
{
	Tooltip = FText::GetEmpty();
}

// Tooltips on how to operate the panel.
void ASwitchPanelTrigger::Showcase_Implementation(FText& Tooltip)
{
	if (!CheckUnlock())
	{
		Tooltip = FText::FromString("Hmm a switch panel. Try a combination or something!");
	}
	else
	{
		Tooltip = FText::FromString("Panel activated the door right next to it!");
	}
}
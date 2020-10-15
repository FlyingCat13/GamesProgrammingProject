// Fill out your copyright notice in the Description page of Project Settings.


#include "Switch.h"

// Sets default values
ASwitch::ASwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set cube as mesh.
	SwitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwitchMesh"));
	RootComponent = SwitchMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> SwitchMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (SwitchMeshAsset.Succeeded())
	{
		SwitchMesh->SetStaticMesh(SwitchMeshAsset.Object);
		SwitchMesh->SetWorldScale3D(FVector(.2f, .05f, .4f));
	}

	// Set custom material for later dynamic use
	ConstructorHelpers::FObjectFinder<UMaterial> SwitchMaterial(TEXT("/Game/StarterContent/Materials/ItemMaterial.ItemMaterial"));
	if (SwitchMaterial.Succeeded())
	{
		SwitchMesh->SetMaterial(0, SwitchMaterial.Object);
	}
}

// Called when the game starts or when spawned
void ASwitch::BeginPlay()
{
	Super::BeginPlay();
	
	// Set up dynamic material instance.
	UMaterialInterface* Material = SwitchMesh->GetMaterial(0);
	DynamicMaterialInstance = SwitchMesh->CreateDynamicMaterialInstance(0, Material);
	if (DynamicMaterialInstance != nullptr)
	{
		DynamicMaterialInstance->SetVectorParameterValue("Colour 1", FLinearColor::Red);
	}
}

// Called every frame
void ASwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//
void ASwitch::Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip)
{
	// Update tooltip
	Tooltip = FText::GetEmpty();
	SetValue(!Value);
}

// Description
void ASwitch::Showcase_Implementation(FText& Tooltip)
{
	if (!IsDisabled)
	{
		Tooltip = FText::FromString("Press E to flip the switch");
	}
	else
	{
		Tooltip = FText::GetEmpty();
	}
}

bool ASwitch::GetValue()
{
	return Value;
}

void ASwitch::SetValue(bool NewValue)
{
	if (IsDisabled)
	{
		return;
	}

	Value = NewValue;
	if (Value)
	{
		if (DynamicMaterialInstance != nullptr)
		{
			DynamicMaterialInstance->SetVectorParameterValue("Colour 1", FLinearColor::Green);
		}
	}
	else
	{
		if (DynamicMaterialInstance != nullptr)
		{
			DynamicMaterialInstance->SetVectorParameterValue("Colour 1", FLinearColor::Red);
		}
	}
}

void ASwitch::SetIsDisabled(bool NewStatus)
{
	IsDisabled = NewStatus;
}
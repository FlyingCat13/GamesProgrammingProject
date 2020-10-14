// Fill out your copyright notice in the Description page of Project Settings.


#include "Match.h"

// Sets default values
AMatch::AMatch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set copper cube as mesh.
	MatchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MatchMesh"));
	RootComponent = MatchMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MatchMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (MatchMeshAsset.Succeeded())
	{
		MatchMesh->SetStaticMesh(MatchMeshAsset.Object);
		MatchMesh->SetWorldScale3D(FVector(0.25f));
	}
}

// Called when the game starts or when spawned
void AMatch::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInterface* Material = MatchMesh->GetMaterial(0);
	DynamicMaterialInstance = MatchMesh->CreateDynamicMaterialInstance(0, Material);
	if (DynamicMaterialInstance != nullptr)
	{
		DynamicMaterialInstance->SetVectorParameterValue("Colour 1", FLinearColor(1.f, 1.f, 0.f));
		DynamicMaterialInstance->SetVectorParameterValue("Colour 2", FLinearColor(1.f, 0.f, .2f));
	}
}

// Called every frame
void AMatch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DynamicMaterialInstance != nullptr)
	{
		float Blend = 0.5f + FMath::Cos(GetWorld()->TimeSeconds * 5) / 2;
		DynamicMaterialInstance->SetScalarParameterValue(TEXT("Blend"), Blend);
	}
}

// Add to inventory upon interaction
void AMatch::Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip)
{
	// Update tooltip
	Tooltip = FText::FromString("This light might keep you sane for a while, but it runs out fast. Use with caution...");
	InteractingPlayer->AddItemToInventory(this);
	// Hide from game world because it is in inventory now.
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

// Description
void AMatch::Showcase_Implementation(FText& Tooltip)
{
	Tooltip = FText::FromString("Match to light this dark, creepy night up\nPress E to pick it up");
}

// When consumed, trigger light match effect on the player.
void AMatch::OnConsume_Implementation(AMainPlayer* InteractingPlayer)
{
	InteractingPlayer->LightMatch(MATCH_DURATION);
	// Remove from inventory and destroy as it is no longer needed.
	InteractingPlayer->RemoveItemFromInventory(this);
	Destroy();
}

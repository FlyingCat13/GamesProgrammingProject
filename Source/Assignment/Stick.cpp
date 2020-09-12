// Fill out your copyright notice in the Description page of Project Settings.


#include "Stick.h"

// Sets default values
AStick::AStick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StickMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StickMesh"));
	RootComponent = StickMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> StickMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Pipe.Shape_Pipe"));
	if (StickMeshAsset.Succeeded())
	{
		StickMesh->SetStaticMesh(StickMeshAsset.Object);
		StickMesh->SetWorldScale3D(FVector(.25f, 2.5f, .25f));
	}

	ConstructorHelpers::FObjectFinder<UMaterial> MatchMaterial(TEXT("/Game/StarterContent/Materials/M_Wood_Oak.M_Wood_Oak"));
	if (MatchMaterial.Succeeded())
	{
		StickMesh->SetMaterial(0, MatchMaterial.Object);
	}
}

// Called when the game starts or when spawned
void AStick::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Add to inventory when interacted.
void AStick::Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip)
{
	Tooltip = FText::GetEmpty();
	InteractingPlayer->AddItemToInventory(this);
	SetActorHiddenInGame(true);
}

// Description for the player when looked at.
void AStick::Showcase_Implementation(FText& Tooltip)
{
	Tooltip = FText::FromString("A solution for stuff that are out of reach! Now available for free\nPress E to pick it up");
}

// Extend player's reach length when equipped.
void AStick::OnEquip_Implementation(AMainPlayer* InteractingPlayer)
{
	InteractingPlayer->SetReachLength(REACH_LENGTH);
}

// Reset reach length when unequipped.
void AStick::OnUnequip_Implementation(AMainPlayer* InteractingPlayer)
{
	InteractingPlayer->ResetReachLength();
}

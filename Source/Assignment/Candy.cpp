// Fill out your copyright notice in the Description page of Project Settings.


#include "Candy.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

// Sets default values
ACandy::ACandy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set mesh as root component
	CandyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CandyMesh"));
	RootComponent = CandyMesh;

	// Find sphere mesh.
	ConstructorHelpers::FObjectFinder<UStaticMesh> CandyMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (CandyMeshAsset.Succeeded())
	{
		CandyMesh->SetStaticMesh(CandyMeshAsset.Object);
		CandyMesh->SetWorldScale3D(FVector(0.25f));
	}

	// Set custom material for later dynamic use
	ConstructorHelpers::FObjectFinder<UMaterial> CandyMaterial(TEXT("/Game/StarterContent/Materials/ItemMaterial.ItemMaterial"));
	if (CandyMaterial.Succeeded())
	{
		CandyMesh->SetMaterial(0, CandyMaterial.Object);
	}
}

// Called when the game starts or when spawned
void ACandy::BeginPlay()
{
	Super::BeginPlay();

	// Set up dynamic material instance.
	UMaterialInterface* Material = CandyMesh->GetMaterial(0);
	DynamicMaterialInstance = CandyMesh->CreateDynamicMaterialInstance(0, Material);
	if (DynamicMaterialInstance != nullptr)
	{
		DynamicMaterialInstance->SetVectorParameterValue("Colour 1", FLinearColor(.2f, .1f, .6f));
		DynamicMaterialInstance->SetVectorParameterValue("Colour 2", FLinearColor(1.f, .1f, .3f));
	}
}

// Called every frame
void ACandy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called when player interacts with it.
void ACandy::Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip)
{
	// Output the tooltip to display on HUD
	Tooltip = FText::FromString("The candy healed your mind and keep you sane for a bit");
	// Reset debuff's counter in player
	InteractingPlayer->ResetDebuff();
	// Heal the specified amount
	InteractingPlayer->Heal(HEAL_AMOUNT);
	// Destroy from world as it is instant consumable
	Destroy();
}

void ACandy::Showcase_Implementation(FText& Tooltip)
{
	// Output the tooltip to display on HUD
	Tooltip = FText::FromString("Delicious rejuvenating candy\nPress E to eat it");
}
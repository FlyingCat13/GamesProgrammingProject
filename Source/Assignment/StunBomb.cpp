// Fill out your copyright notice in the Description page of Project Settings.


#include "StunBomb.h"
#include "Enemy.h"

// Sets default values
AStunBomb::AStunBomb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set cone as mesh.
	BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
	RootComponent = BombMesh;
	ConstructorHelpers::FObjectFinder<UStaticMesh> MatchMeshAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cone.Shape_Cone"));
	if (MatchMeshAsset.Succeeded())
	{
		BombMesh->SetStaticMesh(MatchMeshAsset.Object);
		BombMesh->SetWorldScale3D(FVector(0.25f));
	}

	// Set custom material for later dynamic use
	ConstructorHelpers::FObjectFinder<UMaterial> MatchMaterial(TEXT("/Game/StarterContent/Materials/ItemMaterial.ItemMaterial"));
	if (MatchMaterial.Succeeded())
	{
		BombMesh->SetMaterial(0, MatchMaterial.Object);
	}
}

// Called when the game starts or when spawned
void AStunBomb::BeginPlay()
{
	Super::BeginPlay();

	ParticleFX = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/NiagaraFX/SpriteEffect/StunBombParticle.StunBombParticle"), nullptr, LOAD_None, nullptr);
	
	// Set up dynamic material instance.
	UMaterialInterface* Material = BombMesh->GetMaterial(0);
	DynamicMaterialInstance = BombMesh->CreateDynamicMaterialInstance(0, Material);
	if (DynamicMaterialInstance != nullptr)
	{
		DynamicMaterialInstance->SetVectorParameterValue("Colour 1", FLinearColor(1.f, 1.f, 1.f));
		DynamicMaterialInstance->SetVectorParameterValue("Colour 2", FLinearColor(0.f, 0.f, 0.f));
	}
}

// Called every frame
void AStunBomb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Add to inventory upon interaction
void AStunBomb::Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip)
{
	// Update tooltip
	Tooltip = FText::FromString("Okay ghosts are frozen, run!!!!!");
	InteractingPlayer->AddItemToInventory(this);
	// Hide from game world because it is in inventory now.
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

// Description
void AStunBomb::Showcase_Implementation(FText& Tooltip)
{
	Tooltip = FText::FromString("An offensive relic that only affects the undead\nPress E to pick it up");
}

void AStunBomb::OnConsume_Implementation(AMainPlayer* InteractingPlayer)
{
	UNiagaraComponent* ParticleEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		ParticleFX,
		InteractingPlayer->GetActorLocation(),
		FRotator(1),
		FVector(1.5f, 1.5f, 1.5f),
		true,
		true,
		ENCPoolMethod::AutoRelease,
		true
	);

	if (ParticleEffect)
	{
		UE_LOG(LogClass, Warning, TEXT("Particle"));
	}

	FVector StartTrace = InteractingPlayer->GetActorLocation();
	FVector EndTrace = StartTrace;
	EndTrace.Z += 300.0f;

	TArray<FHitResult> HitActors;

	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(STUN_RADIUS);

	if (GetWorld()->SweepMultiByChannel(HitActors, StartTrace, EndTrace, FQuat::FQuat(), ECC_WorldStatic, CollisionShape))
	{
		for (auto Actor = HitActors.CreateIterator(); Actor; Actor++)
		{
			AEnemy* Enemy = Cast<AEnemy>((*Actor).GetActor());
			if (Enemy)
			{
				Enemy->OnStunned(STUN_DURATION);
			}
		}
	}

	// Remove from inventory and destroy as it is no longer needed.
	InteractingPlayer->RemoveItemFromInventory(this);
	Destroy();
}
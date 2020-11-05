// Fill out your copyright notice in the Description page of Project Settings.
// Majority of this code is from the labs so the comments are only there for the parts that are different.

#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayer.h"

AEnemyAIController::AEnemyAIController()
{
	// Set this to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Code from the labs
	SightConfiguration = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Configuration"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));

	SightConfiguration->SightRadius = SightRadius;
	SightConfiguration->LoseSightRadius = LoseSightRadius;
	SightConfiguration->PeripheralVisionAngleDegrees = FieldOfView;
	SightConfiguration->SetMaxAge(SightAge);

	SightConfiguration->DetectionByAffiliation.bDetectEnemies = true;
	SightConfiguration->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfiguration->DetectionByAffiliation.bDetectNeutrals = true;

	GetPerceptionComponent()->SetDominantSense(*SightConfiguration->GetSenseImplementation());
	GetPerceptionComponent()->ConfigureSense(*SightConfiguration);
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnSensesUpdated);

	TargetPlayer = nullptr;
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// Code from the labs
	NavigationSystem = Cast<UNavigationSystemV1>(GetWorld()->GetNavigationSystem());
	if (NavigationSystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nav System Found"));
	}

	if (!AIBlackboard)
	{
		return;
	}
	if (!ensure(BehaviorTree))
	{
		return;
	}
	UseBlackboard(AIBlackboard, BlackboardComponent);
	RunBehaviorTree(BehaviorTree);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void AEnemyAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (TargetPlayer)
	{
		BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
	}
}

FRotator AEnemyAIController::GetControlRotation() const
{
	if (GetPawn())
	{
		return FRotator(0.f, GetPawn()->GetActorRotation().Yaw, 0.f);
	}
	return FRotator(0.f, 0.f, 0.f);
}

void AEnemyAIController::GenerateNewRandomLocation()
{
	if (NavigationSystem)
	{
		FNavLocation ReturnLocation;
		NavigationSystem->GetRandomPointInNavigableRadius(GetPawn()->GetActorLocation(), 2000, ReturnLocation);
		BlackboardComponent->SetValueAsVector("PatrolPoint", ReturnLocation.Location);
	}
}

void AEnemyAIController::SwitchPatrolPoint()
{
	// Switch patrol point and make that the next point to travel to in the blackboard.
	TargetPatrolPoint = 1 - TargetPatrolPoint;
	BlackboardComponent->SetValueAsVector("PatrolPoint", PatrolLocations[TargetPatrolPoint]);
}

void AEnemyAIController::OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("Sensed"));
	// Use a reference of the MainPlayer class instead of Pawn.
	AMainPlayer* Player = Cast<AMainPlayer>(UpdatedActor);
	if (Player)
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Warning, TEXT("Set Actor Location"));
			TargetPlayer = Player;
			BlackboardComponent->SetValueAsBool("InSight", true);
			BlackboardComponent->SetValueAsVector("PlayerPosition", TargetPlayer->GetActorLocation());
		}
		else
		{
			TargetPlayer = nullptr;
			BlackboardComponent->ClearValue("InSight");
		}
	}
}

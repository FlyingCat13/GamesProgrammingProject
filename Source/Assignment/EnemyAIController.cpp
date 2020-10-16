// Fill out your copyright notice in the Description page of Project Settings.


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

	AMainPlayer* Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// Check if player is using stealth and if player is close (player will have increased drain rate if so)
	if (Player)
	{
		// Squared, so actual distance is 400.
		if ((GetPawn()->GetActorLocation() - Player->GetActorLocation()).SizeSquared() < 40000.f)
		{
			Player->SetDecayRate(20.f);
		}
		else
		{
			Player->ResetDecayRate();
		}
	}

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
	if (TargetPatrolPoint == 1)
	{
		TargetPatrolPoint = 0;
	}
	else
	{
		TargetPatrolPoint = 1;
	}
	BlackboardComponent->SetValueAsVector("PatrolPoint", PATROL_LOCATION[TargetPatrolPoint]);
}

void AEnemyAIController::OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("Sensed"));
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

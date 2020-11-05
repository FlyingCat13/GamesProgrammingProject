// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AEnemyAIController();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float SightRadius = 500.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float SightAge = 3.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float LoseSightRadius = SightRadius + 30.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		float FieldOfView = 180.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		UAISenseConfig_Sight* SightConfiguration;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
		UBlackboardData* AIBlackboard;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
		UBehaviorTree* BehaviorTree;

	UPROPERTY(EditDefaultsOnly, Category = "Blackboard")
		UBlackboardComponent* BlackboardComponent;

	// Two locations of the fuse patroller to go around.
	UPROPERTY(EditAnywhere)
		TArray<FVector> PatrolLocations = { FVector(-800.f, 1400.f, 33.f), FVector(0.f, 1400.f, 33.f) };

protected:
	int TargetPatrolPoint = 1;

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;

	UNavigationSystemV1* NavigationSystem;
	APawn* TargetPlayer;

public:
	virtual void Tick(float DeltaTime) override;
	virtual FRotator GetControlRotation() const override;

	void GenerateNewRandomLocation();
	// Switch patrol spot function for the fuse patroller.
	void SwitchPatrolPoint();

	UFUNCTION()
		void OnSensesUpdated(AActor* UpdatedActor, FAIStimulus Stimulus);
};

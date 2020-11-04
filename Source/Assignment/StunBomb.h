// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Consumable.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interactable.h"
#include "Inventoriable.h"
#include "MainPlayer.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "StunBomb.generated.h"

UCLASS()
class ASSIGNMENT_API AStunBomb : public AActor, public IConsumable, public IInteractable, public IInventoriable
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStunBomb();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UStaticMeshComponent* BombMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
		UNiagaraSystem* ParticleFX;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	const float STUN_DURATION = 5.f;
	const float STUN_RADIUS = 500.f;

	// Animated dynamic material.
	UMaterialInstanceDynamic* DynamicMaterialInstance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Interactable functions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void Interact(AMainPlayer* InteractingPlayer, FText& Tooltip);
	virtual void Interact_Implementation(AMainPlayer* InteractingPlayer, FText& Tooltip) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void Showcase(FText& Tooltip);
	virtual void Showcase_Implementation(FText& Tooltip) override;

	// Consumable function
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void OnConsume(AMainPlayer* InteractingPlayer);
	virtual void OnConsume_Implementation(AMainPlayer* InteractingPlayer) override;
};

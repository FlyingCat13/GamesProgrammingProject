// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoseScreen.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API ULoseScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	// Buttons are exposed publicly to hook callbacks to it
	UPROPERTY(EditAnywhere)
		class UButton* RestartButton;

	UPROPERTY(EditAnywhere)
		class UButton* QuitButton;
};

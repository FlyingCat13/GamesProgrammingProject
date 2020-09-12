// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Button.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinScreen.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API UWinScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;

	// Publicly expose buttons to hook callbacks in Game Mode.
	UPROPERTY(EditAnywhere)
		class UButton* RestartButton;

	UPROPERTY(EditAnywhere)
		class UButton* QuitButton;
};

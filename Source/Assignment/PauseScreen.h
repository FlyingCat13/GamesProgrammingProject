// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseScreen.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_API UPauseScreen : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual bool Initialize() override;
};

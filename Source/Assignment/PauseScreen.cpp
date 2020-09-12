// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseScreen.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateColor.h"

bool UPauseScreen::Initialize()
{
	bool Result = Super::Initialize();

    // Base panel
    UCanvasPanel* Panel = WidgetTree->ConstructWidget<UCanvasPanel>();
    WidgetTree->RootWidget = Panel;

    // Background
    UBorder* Background = WidgetTree->ConstructWidget<UBorder>();
    Background->BrushColor = FLinearColor(FColor(0.f, 0.f, 0.f, 0.6f));
    // Position using slot
    UCanvasPanelSlot* BackgroundSlot = Panel->AddChildToCanvas(Background);
    BackgroundSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
    BackgroundSlot->SetOffsets(FMargin(0.f, 0.f));

    // Huge pause text block
    UTextBlock* PauseTextBlock = WidgetTree->ConstructWidget<UTextBlock>();
    PauseTextBlock->SetText(FText::FromString("Paused"));
    PauseTextBlock->Font.Size = 100;
    // Position using slot
    UCanvasPanelSlot* PauseTextSlot = Panel->AddChildToCanvas(PauseTextBlock);
    PauseTextSlot->SetAnchors(FAnchors(0.5f));
    PauseTextSlot->SetPosition(FVector2D(0.f));
    PauseTextSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    PauseTextSlot->bAutoSize = true;

	return Result;
}
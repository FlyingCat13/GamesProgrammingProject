// Fill out your copyright notice in the Description page of Project Settings.


#include "LoseScreen.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Fonts/SlateFontInfo.h"
#include "Styling/SlateColor.h"

bool ULoseScreen::Initialize()
{
    bool Result = Super::Initialize();

    // Base panel
    UCanvasPanel* Panel = WidgetTree->ConstructWidget<UCanvasPanel>();
    WidgetTree->RootWidget = Panel;

    // Background
    UBorder* Background = WidgetTree->ConstructWidget<UBorder>();
    Background->BrushColor = FLinearColor::Black;
    // Positioning using slot.
    UCanvasPanelSlot* BackgroundSlot = Panel->AddChildToCanvas(Background);
    BackgroundSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
    BackgroundSlot->SetOffsets(FMargin(0.f, 0.f));

    // You lost text
    UTextBlock* WinTextBlock = WidgetTree->ConstructWidget<UTextBlock>();
    WinTextBlock->SetText(FText::FromString("You lost"));
    // Big red letter
    WinTextBlock->Font.Size = 100;
    WinTextBlock->SetColorAndOpacity(FSlateColor(FLinearColor(0.6f, 0.f, 0.f)));
    UCanvasPanelSlot* WinTextSlot = Panel->AddChildToCanvas(WinTextBlock);
    // Positioning using slot.
    WinTextSlot->SetAnchors(FAnchors(0.5f));
    WinTextSlot->SetPosition(FVector2D(0.f, -200.f));
    WinTextSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    WinTextSlot->bAutoSize = true;

    // Restart button
    RestartButton = WidgetTree->ConstructWidget<UButton>();
    UTextBlock* RestartTextBlock = WidgetTree->ConstructWidget<UTextBlock>();
    RestartTextBlock->SetText(FText::FromString("Restart"));
    RestartButton->AddChild(RestartTextBlock);
    // Positioning using slot
    UCanvasPanelSlot* RestartButtonSlot = Panel->AddChildToCanvas(RestartButton);
    RestartButtonSlot->SetAnchors(FAnchors(0.5f));
    RestartButtonSlot->SetPosition(FVector2D(0.f, 0.f));
    RestartButtonSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    RestartButtonSlot->SetSize(FVector2D(300.f, 100.f));

    // Quit button.
    QuitButton = WidgetTree->ConstructWidget<UButton>();
    UTextBlock* QuitTextBlock = WidgetTree->ConstructWidget<UTextBlock>();
    QuitTextBlock->SetText(FText::FromString("Quit"));
    QuitButton->AddChild(QuitTextBlock);
    // Positioning using slot 
    UCanvasPanelSlot* QuitButtonSlot = Panel->AddChildToCanvas(QuitButton);
    QuitButtonSlot->SetAnchors(FAnchors(0.5f));
    QuitButtonSlot->SetPosition(FVector2D(0.f, 150.f));
    QuitButtonSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    QuitButtonSlot->SetSize(FVector2D(300.f, 100.f));

    return Result;
}
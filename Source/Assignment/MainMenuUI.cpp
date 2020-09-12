// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUI.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"

bool UMainMenuUI::Initialize()
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

    // Start button
    StartButton = WidgetTree->ConstructWidget<UButton>();
    UTextBlock* StartTextBlock = WidgetTree->ConstructWidget<UTextBlock>();
    StartTextBlock->SetText(FText::FromString("Start"));
    StartButton->AddChild(StartTextBlock);
    // Positioning using slot.
    UCanvasPanelSlot* StartButtonSlot = Panel->AddChildToCanvas(StartButton);
    StartButtonSlot->SetAnchors(FAnchors(0.5f));
    StartButtonSlot->SetPosition(FVector2D(0.f, -200.f));
    StartButtonSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    StartButtonSlot->SetSize(FVector2D(300.f, 100.f));

    // Quit button
    QuitButton = WidgetTree->ConstructWidget<UButton>();
    UTextBlock* QuitTextBlock = WidgetTree->ConstructWidget<UTextBlock>();
    QuitTextBlock->SetText(FText::FromString("Quit"));
    QuitButton->AddChild(QuitTextBlock);
    // Positioning using slot.
    UCanvasPanelSlot* QuitButtonSlot = Panel->AddChildToCanvas(QuitButton);
    QuitButtonSlot->SetAnchors(FAnchors(0.5f));
    QuitButtonSlot->SetPosition(FVector2D(0.f, -50.f));
    QuitButtonSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    QuitButtonSlot->SetSize(FVector2D(300.f, 100.f));

    // Help text
    UTextBlock* HelpTextBlock = WidgetTree->ConstructWidget<UTextBlock>();
    HelpTextBlock->SetText(FText::FromString(
        "WASD to move around\n"
        "E to interact with / comsume items\n"
        "TAB to change equipped item\n"
        "P to pause / resume game"
    ));
    // Positioning using slot.
    UCanvasPanelSlot* HelpTextSlot = Panel->AddChildToCanvas(HelpTextBlock);
    HelpTextSlot->SetAnchors(FAnchors(0.5f, 0.5f));
    HelpTextSlot->SetPosition(FVector2D(0.f, 150.f));
    HelpTextSlot->SetAlignment(FVector2D(0.5f, 0.5f));
    HelpTextSlot->bAutoSize = true;

    return Result;
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameHUD.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetTree.h"

bool UInGameHUD::Initialize()
{
	bool Result = Super::Initialize();

	// Base panel
	UCanvasPanel* Panel = WidgetTree->ConstructWidget<UCanvasPanel>();
	WidgetTree->RootWidget = Panel;

	// Progress bar as health bar
	// Create the widget
	HealthBar = WidgetTree->ConstructWidget<UProgressBar>();
	// Set color to red
	HealthBar->SetFillColorAndOpacity(FLinearColor(.6f, 0.f, 0.f));
	// Use slot to position the widget.
	UCanvasPanelSlot* HealthBarSlot = Panel->AddChildToCanvas(HealthBar);
	HealthBarSlot->SetPosition(FVector2D(100.f, 100.f));
	HealthBarSlot->SetSize(FVector2D(600.f, 40.f));

	// Create the look at item widget
	LookAtItemTooltipTextBlock = WidgetTree->ConstructWidget<UTextBlock>();
	// Use slot to position the widget
	UCanvasPanelSlot* LookAtItemTooltipSlot = Panel->AddChildToCanvas(LookAtItemTooltipTextBlock);
	LookAtItemTooltipSlot->SetAnchors(FAnchors(0.5f));
	LookAtItemTooltipSlot->SetPosition(FVector2D(0.f, 100.f));
	LookAtItemTooltipSlot->bAutoSize = true;

	// Create the interacted item widget
	InteractedItemTooltipTextBlock = WidgetTree->ConstructWidget<UTextBlock>();
	// Use slot to position the widget
	UCanvasPanelSlot* InteractedItemTooltipSlot = Panel->AddChildToCanvas(InteractedItemTooltipTextBlock);
	InteractedItemTooltipSlot->SetAnchors(FAnchors(0.5f));
	InteractedItemTooltipSlot->SetPosition(FVector2D(0.f, 200.f));
	InteractedItemTooltipSlot->bAutoSize = true;

	// Create an array of text block to display inventory items
	FVector2D CurrentItemTextBlockPosition = FVector2D(100, 225);
	for (int i = 0; i < MAX_INVENTORY_SLOT_COUNT; i++) {
		// Create the text block
		UTextBlock* NewItemTextBlock =  WidgetTree->ConstructWidget<UTextBlock>();
		// Empty so that it will not show up on screen when nothing is there
		NewItemTextBlock->SetText(FText::GetEmpty());
		// Use slot to position the widget
        UCanvasPanelSlot* NewItemSlot = Panel->AddChildToCanvas(NewItemTextBlock);
		NewItemSlot->SetPosition(CurrentItemTextBlockPosition);
		NewItemSlot->bAutoSize = true;

		// Add to array
		InventoryItems.Add(NewItemTextBlock);

		CurrentItemTextBlockPosition.Y += 50.f;
	}

	return Result;
}

void UInGameHUD::UpdateInventoryInformation(TArray<FText> InventoryItemNames, int EquippedItem)
{
	// For names that are input, update on the screen
	for (int i = 0; i < InventoryItemNames.Num(); i++)
	{
		FLinearColor InventoryItemColor = FLinearColor::White;
		// Equipped item will be yellow
		if (i == EquippedItem)
		{
			InventoryItemColor = FLinearColor(1.f, 0.65f, 0.f);
		}
		InventoryItems[i]->SetText(InventoryItemNames[i]);
		InventoryItems[i]->SetColorAndOpacity(FSlateColor(InventoryItemColor));
	}

	// The rest of the slot will be set to empty just in case an item was consumed or something.
	for (int i = InventoryItemNames.Num(); i < MAX_INVENTORY_SLOT_COUNT; i++)
	{
		InventoryItems[i]->SetText(FText::GetEmpty());
	}
}
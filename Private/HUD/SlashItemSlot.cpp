// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashItemSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/Item.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/SlateWrapperTypes.h"
#include "Components/KnapsackComponent.h"
#include "HUD/SlashIconDragDrop.h"
#include "HUD/SlashIcon.h"
#include "HUD/KnapsackWidget.h"
#include "Components/PanelWidget.h"
void USlashItemSlot::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (Item)
	{
		if (Item->GetItemName() == FName(TEXT("EmptyItem")))
		{

			SetItemNumber(0);
		}
		else
		{
			//SetItemTexture(Item->GetItemTexture());
			SetItemNumber(Item->GetItemNumber());
		}
	}
	else
	{

		SetItemNumber(0);
	}
	if (KnapsackComponentClass && GetOwningPlayerPawn())
	{
		UActorComponent* ActorComponent = GetOwningPlayerPawn()->GetComponentByClass(KnapsackComponentClass);
		KnapsackComponent = Cast<UKnapsackComponent>(ActorComponent);
	}
}
FReply USlashItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FReply Reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	if (Item && Item->GetItemName() != FName(TEXT("EmptyItem")))
	{
		FEventReply EventReply;
		EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		//UE_LOG(LogTemp, Warning, TEXT("1"));
		if (KnapsackComponent)
		{

			if (KnapsackComponent->GetIsThrowAll())
			{
				Throw = Item->GetItemNumber();
			}
			else
			{
				Throw = 1;
			}
		}
		return EventReply.NativeReply;
	}
	//UE_LOG(LogTemp, Warning, TEXT("2"));
	return 	Reply;
}
void USlashItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (IconDragDrop && IconClass && Item)
	{
		UDragDropOperation* NewDrag = UWidgetBlueprintLibrary::CreateDragDropOperation(IconDragDrop);
		if (NewDrag)
		{
			USlashIconDragDrop* SlashIconDragDrop = Cast<USlashIconDragDrop>(NewDrag);
			USlashIcon* SlashIcon = CreateWidget<USlashIcon>(GetOwningPlayer(), IconClass);
			if (SlashIconDragDrop && SlashIcon)
			{
				UE_LOG(LogTemp, Warning, TEXT("CreateDrag"));
				SlashIconDragDrop->Payload = this;
				SlashIconDragDrop->DefaultDragVisual = SlashIcon;
				SlashIcon->SetItem(Item);
				SlashIcon->SetTexture(Item->GetItemTexture());
				OutOperation = SlashIconDragDrop;
			}

		}
	}
}
bool USlashItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	USlashItemSlot* ItemSlot = Cast<USlashItemSlot>(InOperation->Payload);
	/*
	if (KnapsackComponentClass && GetOwningPlayerPawn())
	{
		UActorComponent* ActorComponent = GetOwningPlayerPawn()->GetComponentByClass(KnapsackComponentClass);
		KnapsackComponent = Cast<UKnapsackComponent>(ActorComponent);
	}
	*/
	if (ItemSlot && KnapsackComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Exchange"));
		KnapsackComponent->SwapItem(ItemSlot->GetIndex(), Index);
	}
	if (KnapsackWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("KnapsackWidgetUpdate"));
		KnapsackWidget->UpdateList();
	}

	return true;
}
void USlashItemSlot::SetItemTexture(UTexture2D* Texture)
{
	ItemTexture->SetBrushFromTexture(Texture);
}
void USlashItemSlot::SetItemNumber(int32 Number)
{
	FText NumberText = FText::FromString(FString::FromInt(Number));
	ItemNumber->SetText(NumberText);
	if (Number <= 0)
	{
		ItemNumber->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		ItemNumber->SetVisibility(ESlateVisibility::Visible);
	}
}
void USlashItemSlot::SetItem(AItem* NewItem)
{
	Item = NewItem;
}
void USlashItemSlot::SetIndex(int32 Number)
{
	Index = Number;
}

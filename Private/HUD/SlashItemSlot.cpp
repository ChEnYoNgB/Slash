// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashItemSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/Item.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/SlateWrapperTypes.h"
void USlashItemSlot::NativePreConstruct()
{
	UE_LOG(LogTemp, Warning, TEXT("USlashItemSlot"));
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
}
FReply USlashItemSlot::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (Item && Item->GetItemName() != FName(TEXT("EmptyItem")))
	{
		FEventReply EventReply;
		EventReply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
		UE_LOG(LogTemp, Warning, TEXT("1"));
		return EventReply.NativeReply;
	}
	UE_LOG(LogTemp, Warning, TEXT("2"));
	return 	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
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
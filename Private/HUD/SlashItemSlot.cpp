// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashItemSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Items/Item.h"
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
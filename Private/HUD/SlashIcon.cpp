// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashIcon.h"
#include "Components/Image.h"
#include "Items/Item.h"
void USlashIcon::NativePreConstruct()
{
	Super::NativePreConstruct();
	if (Item && Item->GetItemTexture())
	{
		IconTexture->SetBrushFromTexture(Item->GetItemTexture());
	}
}
void USlashIcon::SetItem(AItem* NewItem)
{
	Item = NewItem;
}
void USlashIcon::SetTexture(UTexture2D* Texture)
{
	if (Texture)
	{
		IconTexture->SetBrushFromTexture(Texture);
	}
}
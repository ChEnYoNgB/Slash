// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/KnapsackWidget.h"
#include "HUD/SlashItemSlot.h"
#include "Items/Item.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/KnapsackComponent.h"
#include "GameFramework/Pawn.h"

void UKnapsackWidget::NativeConstruct()
{
	UE_LOG(LogTemp, Warning, TEXT("UKnapsackWidget"));
	Super::NativeConstruct();
	if (KnapsackComponentClass && GetOwningPlayerPawn())
	{
		UActorComponent* ActorComponent = GetOwningPlayerPawn()->GetComponentByClass(KnapsackComponentClass);
		KnapsackComponent = Cast<UKnapsackComponent>(ActorComponent);
	}
	UpdateList();
}
void UKnapsackWidget::UpdateList()
{
	if (ItemList && KnapsackComponent && ItemSlotClass)
	{
		ItemList->ClearChildren();
		TArray<AItem*> ItemArray = KnapsackComponent->GetItemArray();
		for (AItem* Item : ItemArray)
		{
			USlashItemSlot* SlashItemSlot = CreateWidget<USlashItemSlot>(GetOwningPlayer(), ItemSlotClass);
			if (SlashItemSlot)
			{
				SlashItemSlot->SetItem(Item);
				SlashItemSlot->SetItemTexture(Item->GetItemTexture());
				SlashItemSlot->SetIndex(ItemArray.Find(Item));
				SlashItemSlot->SetItemNumber(Item->GetItemNumber());
				ItemList->AddChild(SlashItemSlot);
			}
		}
	}
}

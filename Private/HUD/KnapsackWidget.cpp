// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/KnapsackWidget.h"
#include "HUD/SlashItemSlot.h"
#include "Items/Item.h"
#include "Components/WrapBox.h"
#include "Components/WrapBoxSlot.h"
#include "Components/KnapsackComponent.h"
#include "GameFramework/Pawn.h"
#include "HUD/SlashIconDragDrop.h"
#include "Items/Weapons/Weapon.h"

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
bool UKnapsackWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
	if (KnapsackComponent)
	{
		USlashItemSlot* ItemSlot = Cast<USlashItemSlot>(InOperation->Payload);
		AWeapon* Weapon = Cast<AWeapon>(ItemSlot->GetItem());
		if (Weapon)
		{
			if (Weapon->CanThrow())
			{
				//TODO:Throw a weapon
				UE_LOG(LogTemp, Warning, TEXT("Throw a weapon"));
				return true;
			}
			else
			{
				return true;
			}
		}
		if (ItemSlot)
		{

			const int32 Index = ItemSlot->GetIndex();
			KnapsackComponent->RemoveItem(Index, ItemSlot->GetThrowNumber());
			UpdateList();
		}

	}
	return true;
}
void UKnapsackWidget::UpdateList()
{
	if (ItemList && KnapsackComponent && ItemSlotClass)
	{
		ItemList->ClearChildren();
		TArray<AItem*> ItemArray = KnapsackComponent->GetItemArray();
		int32 Index = 0;
		for (AItem* Item : ItemArray)
		{
			USlashItemSlot* SlashItemSlot = CreateWidget<USlashItemSlot>(GetOwningPlayer(), ItemSlotClass);
			if (SlashItemSlot)
			{
				SlashItemSlot->SetKnapsackWidget(this);
				SlashItemSlot->SetItem(Item);
				SlashItemSlot->SetItemTexture(Item->GetItemTexture());
				SlashItemSlot->SetIndex(Index++);
				SlashItemSlot->SetItemNumber(Item->GetItemNumber());
				ItemList->AddChild(SlashItemSlot);
			}
		}
	}
}

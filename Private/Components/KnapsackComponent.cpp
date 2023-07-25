// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/KnapsackComponent.h"
#include "Items/Item.h"
#include "GameFramework/Pawn.h"
#include "Engine/Texture2D.h"
#include "UObject/UObjectGlobals.h"

UKnapsackComponent::UKnapsackComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

	EmetyItem = CreateDefaultSubobject<AItem>(TEXT("Empty"));
	EmetyItem->SetItemName(FName(TEXT("EmptyItem")));
	EmetyItem->SetItemTexture(nullptr);
}



void UKnapsackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwner())
	{
		PlayerCharacter = Cast<APawn>(GetOwner());
	}
	if (PlayerCharacter)
	{
		PlayerController = PlayerCharacter->GetController();
	}

}



void UKnapsackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


}

void UKnapsackComponent::InitKnapsack()
{
	for (int32 i = 0; i < MaxSize; ++i)
	{
		ItemArray.Add(EmetyItem);
	}
}
void UKnapsackComponent::AddItem(AItem* NewItem)
{
	if (NewItem == nullptr || bIsFull)return;
	if (NewItem->GetbIsStack())
	{
		int32 FindResult = -1;
		FName ItemName = NewItem->GetItemName();
		AItem** FindArray = ItemArray.FindByPredicate([ItemName](const AItem* Element) { return Element->GetItemName() == ItemName; });
		if (FindArray == nullptr)
		{
			int32 EmptyIndex = ItemArray.Find(EmetyItem);
			if (EmptyIndex == -1)
			{
				bIsFull = true;
				UE_LOG(LogTemp, Warning, TEXT("Full"));
			}
			else
			{
				ItemArray[EmptyIndex] = NewItem;
				ItemArray[EmptyIndex]->SetItemNumber(ItemArray[EmptyIndex]->GetItemNumber() + 1);
			}
		}
		else
		{
			FindArray[0]->SetItemNumber(FindArray[0]->GetItemNumber() + 1);
		}
		//FindResult = ItemArray.Find(NewItem);
		/*
		if (FindResult == -1)
		{
			int32 EmptyIndex = ItemArray.Find(EmetyItem);
			if (EmptyIndex == -1)
			{
				bIsFull = true;
				UE_LOG(LogTemp, Warning, TEXT("Full"));
			}
			else
			{
				ItemArray[EmptyIndex] = NewItem;
				ItemArray[EmptyIndex]->SetItemNumber(ItemArray[EmptyIndex]->GetItemNumber() + 1);
			}

		}
		else
		{
			ItemArray[FindResult]->SetItemNumber(ItemArray[FindResult]->GetItemNumber() + 1);
		}
		*/
	}
	else
	{
		int32 EmptyIndex = ItemArray.Find(EmetyItem);
		if (EmptyIndex == -1)
		{
			bIsFull = true;
			UE_LOG(LogTemp, Warning, TEXT("Full"));
		}
		else
		{
			ItemArray[EmptyIndex] = NewItem;
			ItemArray[EmptyIndex]->SetItemNumber(ItemArray[EmptyIndex]->GetItemNumber() + 1);
		}

	}
}
void UKnapsackComponent::RemoveItem(int32 Index, int32 Number)
{
	if (Index >= MaxSize || Index < 0 || Number <= 0)return;
	if (ItemArray[Index] && ItemArray[Index] != EmetyItem)
	{
		const int32 Result = ItemArray[Index]->GetItemNumber() - Number;
		AItem* throwItem = ItemArray[Index];
		if (Result <= 0)
		{
			ItemArray[Index] = EmetyItem;
		}
		else
		{
			ItemArray[Index]->SetItemNumber(Result);
		}
		ThrowItem(throwItem);
	}
}
void UKnapsackComponent::SwapItem(int32 Index1, int32 Index2)
{
	if (Index1 >= MaxSize || Index1 < 0 || Index2 >= MaxSize || Index2 < 0)return;
	ItemArray.Swap(Index1, Index2);
}

void UKnapsackComponent::ThrowItem(AItem* Item)
{
	UWorld* World = GetWorld();
	if (World && Item)
	{
		AItem* SpawnItem = World->SpawnActor<AItem>(Item->GetClass(), PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorForwardVector() * 100.f, PlayerCharacter->GetActorRotation());
		if (SpawnItem)
		{
			SpawnItem = DuplicateObject<AItem>(Item, SpawnItem);
			SpawnItem->GetStaticMesh()->SetPhysicsLinearVelocity(FVector(0.f, 0.f, 100.f));
		}
	}
}
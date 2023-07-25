// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KnapsackComponent.generated.h"

class AItem;
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SLASH_API UKnapsackComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UKnapsackComponent();

protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	FORCEINLINE TArray<AItem*> GetItemArray()const { return ItemArray; }
	void InitKnapsack();
	void AddItem(AItem* NewItem);
	void RemoveItem(int32 Index, int32 Number);
	void SwapItem(int32 Index1, int32 Index2);
	void ThrowItem(AItem* Item);

private:
	UPROPERTY(EditAnywhere, Category = "Knapsack")
		TArray<AItem*> ItemArray;
	UPROPERTY(VisibleAnywhere, Category = "Knapsack")
		int32 MaxSize = 30;
	UPROPERTY(VisibleAnywhere, Category = "Knapsack")
		AItem* EmetyItem;
	UPROPERTY(VisibleAnywhere, Category = "Knapsack")
		bool bIsFull = false;
	UPROPERTY(VisibleAnywhere, Category = "Knapsack")
		class APawn* PlayerCharacter;
	UPROPERTY(VisibleAnywhere, Category = "Knapsack")
		class AController* PlayerController;
};

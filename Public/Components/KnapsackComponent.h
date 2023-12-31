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
	FORCEINLINE bool GetIsThrowAll()const { return bIsThrowAll; }
	FORCEINLINE bool GetIsFull()const { return bIsFull; }
	FORCEINLINE void SetIsThrowAll(bool Value) { bIsThrowAll = Value; }

private:
	UPROPERTY(EditAnywhere, Category = "Knapsack")
		TArray<AItem*> ItemArray;
	UPROPERTY(EditAnywhere, Category = "Knapsack")
		int32 MaxSize = 5;
	UPROPERTY(VisibleAnywhere, Category = "Knapsack")
		AItem* EmetyItem;
	UPROPERTY(VisibleAnywhere, Category = "Knapsack")
		bool bIsFull = false;
	UPROPERTY(VisibleAnywhere, Category = "Knapsack")
		class APawn* PlayerCharacter;
	UPROPERTY(VisibleAnywhere, Category = "Knapsack")
		class AController* PlayerController;
	UPROPERTY(EditAnywhere, Category = "Knapsack")
		class UTexture2D* EmptyTexture;
	UPROPERTY()
		bool bIsThrowAll = false;
};

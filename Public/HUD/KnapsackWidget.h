// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KnapsackWidget.generated.h"

class UWrapBox;
class UKnapsackComponent;
class USlashItemSlot;
/**
 *
 */
UCLASS()
class SLASH_API UKnapsackWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct()override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	void UpdateList();
protected:
	UPROPERTY(EditAnywhere)
		TSubclassOf<UKnapsackComponent> KnapsackComponentClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<USlashItemSlot> ItemSlotClass;
private:
	UPROPERTY(meta = (BindWidget))
		UWrapBox* ItemList;
	UPROPERTY(VisibleAnywhere)
		UKnapsackComponent* KnapsackComponent;
};

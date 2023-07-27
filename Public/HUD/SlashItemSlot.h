// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlashItemSlot.generated.h"
class AItem;
class UImage;
class UTextBlock;
class UTexture2D;
/**
 *
 */
UCLASS()
class SLASH_API USlashItemSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct()override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)override;
	void SetItemTexture(UTexture2D* Texture);
	void SetItemNumber(int32 Number);
	void SetItem(AItem* NewItem);
	void SetIndex(int32 Number);
	FORCEINLINE class UKnapsackWidget* GetKnapsackWidget()const { return KnapsackWidget; }
	FORCEINLINE void SetKnapsackWidget(UKnapsackWidget* Widget) { KnapsackWidget = Widget; }
	FORCEINLINE AItem* GetItem()const { return Item; }
	FORCEINLINE int32 GetIndex()const { return Index; }
	FORCEINLINE int32 GetThrowNumber()const { return Throw; }

private:
	UPROPERTY(EditAnywhere)
		TSubclassOf<class USlashIconDragDrop> IconDragDrop;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class USlashIcon> IconClass;
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UKnapsackComponent> KnapsackComponentClass;
	UPROPERTY(meta = (BindWidget))
		UImage* ItemTexture;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ItemNumber;
	UPROPERTY()
		AItem* Item;
	UPROPERTY()
		int32 Index;
	UPROPERTY()
		int32 Throw = 0;
	UPROPERTY()
		UKnapsackComponent* KnapsackComponent;
	UPROPERTY()
		UKnapsackWidget* KnapsackWidget;
};

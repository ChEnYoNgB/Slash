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
	void SetItemTexture(UTexture2D* Texture);
	void SetItemNumber(int32 Number);
	void SetItem(AItem* NewItem);
	void SetIndex(int32 Number);
private:
	UPROPERTY(meta = (BindWidget))
		UImage* ItemTexture;
	UPROPERTY(meta = (BindWidget))
		UTextBlock* ItemNumber;
	UPROPERTY()
		AItem* Item;
	UPROPERTY()
		int32 Index;
};

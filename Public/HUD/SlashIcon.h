// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlashIcon.generated.h"
class AItem;
class UImage;
/**
 *
 */
UCLASS()
class SLASH_API USlashIcon : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativePreConstruct()override;
	void SetItem(AItem* NewItem);
	void SetTexture(class UTexture2D* Texture);
private:
	UPROPERTY(meta = (BindWidget))
		UImage* IconTexture;
	UPROPERTY()
		AItem* Item;
};

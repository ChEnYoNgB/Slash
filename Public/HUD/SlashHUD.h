// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SlashHUD.generated.h"

/**
 *
 */
UCLASS()
class SLASH_API ASlashHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay()override;
private:
	UPROPERTY(EditDefaultsOnly, Category = Slash)
		TSubclassOf<class USlashOverlay> SlashOverlayClass;
	UPROPERTY(VisibleAnywhere, Category = Slash)
		USlashOverlay* SlashOverlay;
	UPROPERTY(EditDefaultsOnly, Category = Slash)
		TSubclassOf<class UKnapsackWidget> SlashKnapsackClass;
	UPROPERTY(VisibleAnywhere, Category = Slash)
		UKnapsackWidget* SlashKnapsack;
public:
	FORCEINLINE USlashOverlay* GetSlashOverlay()const { return SlashOverlay; }
	FORCEINLINE UKnapsackWidget* GetSlashKnapsack()const { return SlashKnapsack; }
	void SetKnapsackVisibility(bool Visbility);
	bool GetKnapsackVisibility();
	void CreateKnapsackWidget();
};

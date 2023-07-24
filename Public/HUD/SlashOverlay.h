// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SlashOverlay.generated.h"

class UProgressBar;
/**
 *
 */
UCLASS()
class SLASH_API USlashOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetHealthPercent(float Percent);
	void SetStaminaPercent(float Percent);
private:
	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthProgressBar;
	UPROPERTY(meta = (BindWidget))
		UProgressBar* StaminaProgressBar;
};

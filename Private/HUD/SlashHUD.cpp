// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/SlashHUD.h"
#include "HUD/SlashOverlay.h"
#include "HUD/KnapsackWidget.h"

void ASlashHUD::BeginPlay()
{
	UE_LOG(LogTemp, Warning, TEXT("ASlashHUD"));
	Super::BeginPlay();
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && SlashOverlayClass)
		{
			SlashOverlay = CreateWidget<USlashOverlay>(Controller, SlashOverlayClass);
			SlashOverlay->AddToViewport();
		}
	}
}
void ASlashHUD::SetKnapsackVisibility(bool Visbility)
{
	if (!SlashKnapsack)return;
	if (Visbility)
	{
		SlashKnapsack->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SlashKnapsack->SetVisibility(ESlateVisibility::Hidden);
	}
}
bool ASlashHUD::GetKnapsackVisibility()
{
	if (!SlashKnapsack)return false;
	if (SlashKnapsack->GetVisibility() == ESlateVisibility::Visible)
	{
		return true;
	}
	return false;
}
void ASlashHUD::CreateKnapsackWidget()
{
	UWorld* World = GetWorld();
	APlayerController* Controller = World->GetFirstPlayerController();
	if (World && Controller && SlashKnapsackClass)
	{
		SlashKnapsack = CreateWidget<UKnapsackWidget>(Controller, SlashKnapsackClass);
		SlashKnapsack->AddToViewport();
		SlashKnapsack->SetVisibility(ESlateVisibility::Hidden);
	}
}
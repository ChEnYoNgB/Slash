// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Treasures.h"
#include "Interfaces/PickupInterface.h"
#include "Character/SlashCharacter.h"
#include "Components/KnapsackComponent.h"

void ATreasures::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	/*
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	if (PickupInterface)
	{
		SpawnPickUpSound();
		Destroy();
	}
	ASlashCharacter* SlashCharacter = Cast<ASlashCharacter>(OtherActor);
	if (SlashCharacter && SlashCharacter->GetKnapsackComponent())
	{
		UKnapsackComponent* KnapsackComponent = SlashCharacter->GetKnapsackComponent();
		KnapsackComponent->AddItem(this);
	}
	*/
}

void ATreasures::PickUp()
{
	SpawnPickUpSound();
	Destroy();
}

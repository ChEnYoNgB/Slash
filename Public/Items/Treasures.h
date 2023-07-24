// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Treasures.generated.h"

class USoundBase;
/**
 *
 */
UCLASS()
class SLASH_API ATreasures : public AItem
{
	GENERATED_BODY()
public:
	virtual void PickUp()override;
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;

	UPROPERTY(EditAnywhere, Category = Gold)
		int32 Gold;
};

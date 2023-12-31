// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Soul.generated.h"

class UNiagaraSystem;

/**
 *
 */
UCLASS()
class SLASH_API ASoul : public AItem
{
	GENERATED_BODY()
protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
private:
	UPROPERTY(Editanywhere, Category = "Soul Properties")
		int32 Soul;
public:
	FORCEINLINE int32 GetSoul()const { return Soul; }
	FORCEINLINE void SetSoul(int32 NumberOfSoul) { Soul = NumberOfSoul; }
};
